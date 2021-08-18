/*
 * Copyright (C) 2010-2021, Benjamin Drung <bdrung@posteo.de>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "noinline.h"

#ifdef __GNUC__
#define likely(x)   __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

// format_size_base2 ripped out of libkibi
#define malloc_error(size) fprintf(stderr, \
                                   "\nmalloc(%zu) failed in %s.\n\n", \
                                   size, __func__)
#define PREFIX_COUNT 7
static const struct lconv *locale = NULL;
const char *prefix[PREFIX_COUNT] = {"", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei"};
static char *format_size_base2(size_t size, const char *suffix) {
    char *formatted_size = NULL;
    size_t carry_10 = 0;
    size_t carry_100 = 0;
    size_t rounded_size;
    int i;
    size_t length;

    if(size < 1024) {
        // strlen("1023") = 4
        length = 4 + 1 + strlen(suffix) + 1;
        formatted_size = malloc(length);
        if(unlikely(formatted_size == NULL)) {
            malloc_error(length);
            return NULL;
        }
        snprintf(formatted_size, length, "%zu %s", size, suffix);
    } else {
        i = 1;
        // 1023.5 * 1024 = 1,048,064 will be rounded to 1024 * 1024 = 2 ^ 20.
        while(size >= 1048064) {
            i++;
            carry_10 = (carry_10 + 10 * (size & 0x3FF)) >> 10;
            carry_100 = (carry_100 + 100 * (size & 0x3FF)) >> 10;
            size >>= 10;
        }

        if(locale == NULL) {
            locale = localeconv();
        }

        rounded_size = (size * 100 + carry_100 + 512) >> 10;
        if(rounded_size < 1000) { // rounded_size < 10.00
            // strlen("9.99") = 4
            length = 4 + 1 + strlen(prefix[i]) + strlen(suffix) + 1;
            formatted_size = malloc(length);
            if(unlikely(formatted_size == NULL)) {
                malloc_error(length);
                return NULL;
            }
            snprintf(formatted_size, length, "%zu%s%02zu %s%s",
                     rounded_size / 100, locale->decimal_point,
                     rounded_size % 100, prefix[i], suffix);
        } else { // rounded_size >= 10.00
            rounded_size = (size * 10 + carry_10 + 512) >> 10;
            if(likely(rounded_size < 10000)) { // rounded_size < 1,000.0
                // strlen("999.9") = 5
                length = 5 + 1 + strlen(prefix[i]) + strlen(suffix) + 1;
                formatted_size = malloc(length);
                if(unlikely(formatted_size == NULL)) {
                    malloc_error(length);
                    return NULL;
                }
                snprintf(formatted_size, length, "%zu%s%01zu %s%s",
                         rounded_size / 10, locale->decimal_point,
                         rounded_size % 10, prefix[i], suffix);
            } else { // rounded_size >= 1,000.0
                rounded_size = (size + 512) >> 10;
                // strlen("1023") = 4
                length = 4 + 1 + strlen(prefix[i]) + strlen(suffix) + 1;
                formatted_size = malloc(length);
                if(unlikely(formatted_size == NULL)) {
                    malloc_error(length);
                    return NULL;
                }
                snprintf(formatted_size, length, "%zu %s%s", rounded_size,
                         prefix[i], suffix);
            }
        }
    }

    return formatted_size;
}


noinline void leak_memory(size_t chunk_size, size_t max_memory_size, size_t progress_steps) {
    size_t allocated = 0;
    size_t reported = 0;
    size_t *chunk = NULL;
    size_t *prev_chunk = NULL;

    char *chunk_size_str = format_size_base2(chunk_size, "B");
    char *max_memory_size_str = format_size_base2(max_memory_size, "B");
    printf("Starting memory consumption in %s steps to maximum %s.\n", chunk_size_str, max_memory_size_str);
    fflush(stdout);
    free(chunk_size_str);

    while (allocated < max_memory_size) {
        allocated += chunk_size;
        chunk = malloc(chunk_size);
        *chunk = (size_t)prev_chunk;
        for (size_t *p = chunk + 1; p < chunk + chunk_size / sizeof(size_t); p++) {
            // Fill memory with data to avoid optimization preventing the leak
            *p = allocated;
        }
        prev_chunk = chunk;

        if (allocated - reported >= progress_steps) {
            char *allocated_str = format_size_base2(allocated, "B");
            printf("Allocated %s.\n", allocated_str);
            free (allocated_str);
            fflush(stdout);
            reported = allocated;
        }
    }

    // Free all the memory (if you ever reach this point)
    printf("Successfully allocated %s. Freeing it again.\n", max_memory_size_str);
    free(max_memory_size_str);
    while(chunk != NULL) {
        prev_chunk = (size_t*)*chunk;
        free(chunk);
        chunk = prev_chunk;
    }
}


void print_usage(char *prog) {
    fprintf(stderr,
        "Usage: %s [max_memory_size]\n\nPositional arguments:\n"
        "  max_memory_size      Maximum memory size to allocate in MiB (42 TiB by default)\n",
        prog);
}


int main(int argc, char *argv[]) {
    size_t chunk_size = 1048576; // 1 MiB
    size_t max_memory_size = 46179488366592; // 42 TiB
    size_t progress_steps = 10485760; // 10 MiB

    if(unlikely(argc > 2)) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if(argc > 1) {
        if(sscanf(argv[1], "%zu", &max_memory_size) != 1) {
            if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            }
            fprintf(stderr, "%s: error: The first parameter must be an unsigned integer.\n", argv[0]);
            return EXIT_FAILURE;
        }
        max_memory_size *= 1048576;
    }

    leak_memory(chunk_size, max_memory_size, progress_steps);

    return EXIT_SUCCESS;
}

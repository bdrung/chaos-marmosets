---
date: 2021-04-23
footer: chaos-marmosets
header: "leak-memory's Manual"
layout: page
license: 'Licensed under the ISC license'
section: 1
title: LEAK-MEMORY
---

# NAME

leak-memory - eat your memory like Obelix eats cake

# SYNOPSIS

**leak-memory** [*max_memory_size*]

# DESCRIPTION

**leak-memory** will eat your memory like Obelix eats cake until it will
eventually trigger the out of memory (OOM) killer. This program is useful to
test configured cgroups memory limitations and the OOM killer behavior.

**leak-memory** will use *malloc* to allocate 1 MiB blocks of memory in a loop
until the given maximum memory size (by default 42 TiB) is reached.  The
allocated memory blocks will be filled with data to avoid optimization
preventing the leak. The program will report its progress to make it easy to
spot when **leak-memory** hits the OOM killer.

# OPTIONS

*max_memory_size*
:   Maximum memory size to allocate in MiB (42 TiB by default)

# AUTHOR

Benjamin Drung <bdrung@posteo.de>

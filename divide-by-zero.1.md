---
date: 2021-04-23
footer: chaos-marmosets
header: "divide-by-zero's Manual"
layout: page
license: 'Licensed under the ISC license'
section: 1
title: DIVIDE-BY-ZERO
---

# NAME

divide-by-zero - try to divide a number by zero and crash

# SYNOPSIS

**divide-by-zero**

# DESCRIPTION

**divide-by-zero** tries to divide a number by zero. It will immediately exit
with signal 4 (SIGILL). This program is useful to test the process core dump
configuration. See the Linux kernel sysctl setting *kernel.core_pattern* and
*fs.suid_dumpable* for more information. In case of using a bug reporting
system like apport, this program allows testing reporting the crash and
retracing it.

# OPTIONS

This program takes no options.

# AUTHOR

Benjamin Drung <bdrung@posteo.de>

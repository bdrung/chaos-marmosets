---
date: 2021-04-23
footer: chaos-marmosets
header: "seg-fault's Manual"
layout: page
license: 'Licensed under the ISC license'
section: 1
title: SEG-FAULT
---

# NAME

seg-fault - try to write to address zero and crash

# SYNOPSIS

**seg-fault**

# DESCRIPTION

**seg-fault** tries write to address zero and will trigger a segmentation fault
(signal 11). It is useful to test the process core dump configuration. See the
Linux kernel sysctl setting *kernel.core_pattern* and *fs.suid_dumpable* for
more information. In case of using a bug reporting system like apport, this
program allows testing reporting the crash and retracing it.

# OPTIONS

This program takes no options.

# AUTHOR

Benjamin Drung <bdrung@posteo.de>

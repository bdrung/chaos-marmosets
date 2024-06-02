---
date: 2024-06-02
footer: chaos-marmosets
header: "divide-by-zero-python's Manual"
layout: page
license: 'Licensed under the ISC license'
section: 1
title: DIVIDE-BY-ZERO-PYTHON
---

# NAME

divide-by-zero-python - try to divide a number by zero and crash

# SYNOPSIS

**divide-by-zero-python**

# DESCRIPTION

**divide-by-zero-python** tries to divide a number by zero. It will immediately
exit with a Python stack trace. This program is useful to test Python's
`sys.excepthook` to handle crashes. In case of using a bug reporting system
like Apport, this program allows testing reporting the crash.

# OPTIONS

This program takes no options.

# AUTHOR

Benjamin Drung <bdrung@posteo.de>

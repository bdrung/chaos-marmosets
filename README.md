Chaos marmosets
===============

This project contains small programs that behave badly. They can be used for
[chaos engineering](https://en.wikipedia.org/wiki/Chaos_engineering) to test
the system behavior and the infrastructure setup for those cases.

divide-by-zero
--------------

This program tries to divide a number by zero. It will immediately exit with
signal 4 (SIGILL). This program is useful to test the process core dump
configuration. See the Linux kernel sysctl setting `kernel.core_pattern` and
`fs.suid_dumpable` for more information. In case of using a bug reporting
system like [apport](https://wiki.ubuntu.com/Apport), this program allows
testing reporting the crash and retracing it.

divide-by-zero-python
---------------------

This program tries to divide a number by zero. It will immediately exit with a
Python stack trace. This program is useful to test Python's `sys.excepthook` to
handle crashes. In case of using a bug reporting system like
[Apport](https://github.com/canonical/apport), this program allows testing
reporting the crash.

leak-memory
-----------

This program will eat your memory like Obelix eats cake until it will
eventually trigger the out of memory (OOM) killer. This program is useful to
test configured cgroups memory limitations and the OOM killer behavior.
leak-memory will use `malloc` to allocate 1 MiB blocks of memory in a loop
until the given maximum memory size (by default 42 TiB) is reached. The
allocated memory blocks will be filled with data to avoid optimization
preventing the leak. The program will report its progress to make it easy to
spot when leak-memory hits the OOM killer.

seg-fault
---------

This program tries write to address zero and will trigger a segmentation fault
(signal 11). It is useful to test the process core dump configuration. See the
Linux kernel sysctl setting `kernel.core_pattern` and `fs.suid_dumpable` for
more information. In case of using a bug reporting system like
[apport](https://wiki.ubuntu.com/Apport), this program allows testing reporting
the crash and retracing it.

Contributing
============

Contributions are welcome. Please follow
[How to Write a Git Commit Message](https://chris.beams.io/posts/git-commit/)
for writing good commit messages.

Creating releases
=================

To create a release, increase the version in `Makefile`, document the
noteworthy changes in `NEWS`, commit and tag the release:

```
git commit -s -m "Release chaos-marmosets $(make version)" Makefile NEWS
git tag v$(make version)
```

The xz-compressed release tarball can be generated by running:
```
make dist
```

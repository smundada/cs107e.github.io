---
layout: page
title: Guide to the CS107E Directory Structure
---

In this course, we will be using multiple `git` repositories to manage the
required files for the labs and assignments.  Our build system expects these
repositories to be laid out in a specific directory structure. Failure to
follow this structure will lead to headaches when trying to compile your code.

## Top level directory
You should create a directory to hold all the files used during this course.
We will call this `cs107e_home`. Note that the examples in this guide will
have the prefix `/Users/student`, this prefix will be different depending on
where you decide to create the `cs107e_home` directory.

```
$ pwd
/Users/student
$ mkdir cs107e_home
$ cd cs107e_home
```

## Courseware Repository
The `cs107e.github.io` repository contains many important files, particularly
the `cs107e` directory which includes the header files and compiled binaries
that we will be using in course assignments and labs.  It is crucial that this
repository (and the `cs107e` subdirectory) be in the right location relative to
your lab and assignment repositories, so that our build system can find the
files it needs.

```
$ pwd
/Users/student/cs107e_home
$ git clone https://github.com/cs107e/cs107e.github.io
Cloning into 'cs107e.github.io'...
$ ls
cs107e.github.io
```

A particularly important subdirectory of the `cs107e.github.io` repository is
`cs107e`. It contains the following subdirectories:

```
$ ls cs107e.github.io/cs107e
etc    include    lib    src
```

* `include` contains the header files that we will be using throughout the
  course. Each header contains the declarations and documentation for each
  function in the module.  Our `Makefile`s will tell our compiler to include
  this directory when searching for header files.
* `lib` contains a compiled `libpi.a` binary that you will link to use our
  implementations of certain functions.  Our `Makefile`'s will tell our linker
  to include this directory when searching for libraries to link.



## Lab and Assignment Repositories
Cloning lab and assignment repositories should be done in the `cs107e_home` directory.

```
$ git clone https://github.com/cs107e/lab1
Cloning into 'lab1'...
$ ls
cs107e.github.io lab1
$ git clone -b assign1 https://github.com/cs107e/[YOUR-GITHUB-USERNAME]-assignments assign1
$ ls
assign1    cs107e.github.io    lab1
```

## Summary
You should have a top-level directory called `cs107e_home` that contains all
the files used in this course.  This directory should contain a clone of the
`cs107e.github.io` repository, as well as your lab and assignment repositories.
Here is an overview of what your directory structure should look like:

```
cs107e_home
├── assign1
├── assign2
├── ...
├── cs107e.github.io
│   ├── ...
│   ├── cs107e
│   │   ├── etc
│   │   ├── include
│   │   ├── lib
│   │   └── src
│   └── ...
├── lab1
├── lab2
└── ...
```

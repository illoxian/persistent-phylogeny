# Persistent Phylogeny

An implementation for the persistent phylogeny polynomial-time algorithm.

## Prerequisites

- gcc 5.0 and above
- [Boost libraries](http://www.boost.org/more/getting_started/index.html)

## Compiling

### Get Boost

> N.B. Skip this block if you already have Boost on your system

#### From repository

Popular Linux and Unix distributions such as Fedora, Debian, and NetBSD include pre-built Boost packages.

Debian example:

```
# apt install libboost-all-dev
```

#### From source

Download Boost.

- Unix: follow the steps at [boost.org](http://www.boost.org/doc/libs/1_65_1/more/getting_started/unix-variants.html)
- Windows: follow the steps at [boost.org](http://www.boost.org/doc/libs/1_65_1/more/getting_started/windows.html)

##### Boost.ProgramOptions

> N.B. Skip this step if you already built Boost on your system

This implementation uses `Boost.ProgramOptions` to parse command line arguments and options, which means it needs separately-compiled library binaries to work.

- Unix: follow the steps at [boost.org](http://www.boost.org/doc/libs/1_65_1/more/getting_started/unix-variants.html#prepare-to-use-a-boost-library-binary)
- Windows: follow the steps at [boost.org](http://www.boost.org/doc/libs/1_65_1/more/getting_started/windows.html#prepare-to-use-a-boost-library-binary)

> NB. Skip this step if you already updated your platform's environment variable

Moreover, on Unix systems, you may need to prepare plaform-specific settings (since `Boost.ProgramOptions` is a shared library).

- Follow the steps at [boost.org](http://www.boost.org/doc/libs/1_65_1/more/getting_started/unix-variants.html#test-your-program)

### Clone the repository

```
$ git clone https://github.com/vezzo95/persistent-phylogeny.git
$ cd persistent-phylogeny
```

### Compile and link

```
$ make
```

## Usage

```
ppp [OPTION...] FILE...
```

**ppp** corresponds to the executable binary.

**OPTION** is a list of options - can be omitted.

**FILE** is a list of file paths (1 or more).

### Options

```
-h or  --help
```

Display this message.

___

```
-v or --verbose
```

Display the operations performed by the program.

___

## Running

```
$ ./bin/ppp FILE...
```

The program will call the `reduce` function on each graph read from the files.

Examples:

Listing the single files

```
$ ./bin/ppp file1 dir1/file2 dir1/file3
```

Or with globs (if your shell supports it)

```
$ ./bin/ppp dir1/*
```

Or with options (order doesn't matter - still supports globs)

```
$ ./bin/ppp -m -v file1
```

## Input file structure

The first line must contain the size of the matrix and the list of characters to be set as active. 
Empty lines (after the first) are ignored.  
Subsequent lines represent the matrix itself.

```
N M c0 c1 c2... ci...  

Matrix made up of space-separated boolean values
```

Example which has no active characters by default:

```
6 3

0 0 1
0 1 0
0 1 1
1 0 0
1 0 1
1 1 0
```

Example which has the characters c0 and c2 active:

```
6 3 0 2

0 0 1
0 1 0
0 1 1
1 0 0
1 0 1
1 1 0
```

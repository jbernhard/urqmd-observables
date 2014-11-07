# UrQMD observables

Read UrQMD output and calculate basic event observables.

## Building

Simply run `make`.  A recent version of `g++` or `icpc` with C++11 support is required.  `icpc` will be used
if available and appears to result in a ~20% faster binary.

## Usage

`urqmd-observables` reads UrQMD f13 files from stdin and outputs results to stdout in the format

    Nch_mid_rapidity Nch_flow Q2re Q2im Q3re Q3im ...

where

- `Nch_mid_rapidity` is the number of charged particles in |eta| < 0.5;
- `Nch_flow` is the number of charged particles in the kinematic cut for flow;
- `Qnre`, `Qnim` are the real and imaginary parts of the flow vector Qn = sum(exp(i\*n\*phi)).

## Why?

Wouldn't this be easier in Python?  Absolutely, but:

1. This has zero dependencies&mdash;it can run on the OSG.
2. This is faster than Python.  Probably not much (I/O is the bottleneck),
   but little differences add up when one is processing ten million events.

## Limitations

Currently the kinematic cuts (pT and eta range) can only be changed at compile time.  There are
self-explanatory variables at the top of the source file.

Only UrQMD version 2 is supported.  UrQMD 3 produces a slightly different format.

Basically, this is a stopgap until I have time to write a UrQMD to HDF5 converter.

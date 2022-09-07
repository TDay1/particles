# Particles - COSC3500 Major Project Milestone 1

A serial implementation of a 2D fluid (particle) simulation.
---

## Summary
This repository contains a 2D fluid simulation in C++ developed for the Major project Milestone 1.
If you have any issues building or running please don't hesitate to reach out: `thomas.day@uqconnect.edu.au`.
All code in this project is POSIX-compliant C++11.

## Building
Building and incremental builds is all set up with a Makefile. simply run `make`.

Note: simulation parametres are hardcoded at build-time, these may all be found and modified in `src/main.cpp`

## Running
There are two components to run. The first is the particle simulation itself which can be run with `./particle-sim`.
Once particle-sim has run, some out files will be generated in your current working directory, these will be used by the second component - the visualisation scripts.

Note: if you run this on the Getafix console node (which you shouldn't) file logging doesn't work (I assume due to some kind of disk rate limit)
Note: The directory which this log file is written to is configurable on `src/output.cpp:6`. If you do change this make sure you update the scripts in `debug/` to reflect this new path

To run the visualisation scripts:
    * Generate a video (mp4) of the simulation: `python3 debug/visualise.py`
    * Generate a plot of the centre of gravity throughout the sim: `python3 debug/cg.py`

Note: the visualisation tools have dependencies such as python, numpy, matplotlib and ffmpeg

## Project structure
.
├── debug       # Simulation visualisation scripts
├── headers     # C++ header files
├── src         # Program source code
├── obj         # Compiled objects (only used during build)
├── Makefile
└── README.md
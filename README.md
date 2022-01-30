## Molecular system model
### Decription
This is just cool model of molecules that interact with [Lennard-Jones potential](https://en.wikipedia.org/wiki/Lennard-Jones_potential). There are a bunch of particles and interaction is counted only from particles in the neighbouring cells. 

Made it as a programming project for the university in May of 2020.

Inspired by [this (much more cool)](https://github.com/kongaskristjan/PhaseTransition) project. Though it does not share any code with it. 

### How to run
Install `sdl2`, `sdl2_ttf` and `sdl2_image` libraries. On MacOS it is as easy as 
```
brew install sdl2 sdl2_image sdl2_ttf
```
Then just type `make` and launch the `result` executable.

To interact with particles use buttons in the right top of the window. You can remove or add particles, heat or cool them, pause and resume and enable or disable grid (it is only visual). Note that sometimes there might be self-heating gas or similar non-physical things. Remember, this is the simplest realization of this model.

To clean build files 
```
make clean
```

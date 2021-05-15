#!/bin/bash
g++ $1 -lSDL2 -lSDL2_ttf -o $(basename $1 .cpp)

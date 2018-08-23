#!/bin/bash

# Threads
for threads in 1 2 4 6 8
do
	g++ -DNUMTHREADS=$threads cosmic.cpp -o cosmo -lm -fopenmp
	./cosmo
done

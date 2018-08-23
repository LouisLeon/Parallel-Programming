#!/bin/bash

# Threads
for threads in 1 2 4 6 8
do
	# Subdivisions
	for nodes in 50 100 500 1000 5000 7500 10000 15000
	do
		g++ -DNUMT=$threads -DNUMNODES=$nodes bezierVolume.cpp -o proj -lm -fopenmp
		./proj
	done
done

#!/bin/bash

# Threads
for threads in 1 2 4 6 8
do
	# Subdivisions
	for nodes in 1024 16384 262144 4194304
	do
		icpc -DNUMT=$threads -DNUMS=$nodes -o simd simd.cpp -lm -openmp -align -qopt-report=3 -qopt-report-phase=vec -diag-disable remark
		./simd
	done
done

#!/bin/bash

echo -e "\nGPU MULTIPLY-REDUCE"

# Global Work Size
for global_size in 1 2 4 6 8 10 16 24 32 64 128
do
	g++ -DNMB=$global_size -o firstP2 firstPart2.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp -w
	./firstP2
done


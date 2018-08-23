#!/bin/bash

echo "		USING FIX 2! FOLLOWED BY FIX 1"
#FIX 2
# Threads
for threads in 1 2 4 6 
do
	g++ -DNUMT=$threads -DNUM=$pads falseSharingFix2.cpp -o falseSharingFix2 -lm -fopenmp
	./falseSharingFix2
done

echo ""
echo ""

echo "		USING FIX 1!"
# FIX 1
# Threads
for threads in 1 2 4 6 
do
	#Padding
	for pads in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
	do
		g++ -DNUMT=$threads -DNUM=$pads falseSharingFix1.cpp -o falseSharingFix1  -lm -fopenmp
		./falseSharingFix1
	done
done


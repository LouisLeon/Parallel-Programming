all: test 

test: 
	sh script.sh

reduce: 
	sh reduce.sh

proj: 
	clear
	clear
	make clean
	g++ -o first firstPart1.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp
	./first

clean:
	rm -f firstP1 firstP2

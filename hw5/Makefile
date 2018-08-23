all: test

test: 
	sh script.sh

proj: 
	clear
	clear
	make clean
	icpc -o simd simd.cpp -lm -openmp -align -qopt-report=3 -qopt-report-phase=vec 
	./simd

clean:
	rm -f simd *.optrpt

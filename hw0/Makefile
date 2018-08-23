all: one four
	
four:
	/usr/local/common/gcc-7.3.0/bin/g++ project_0_four_thread.cpp -o four -O3 -lm -fopenmp
one:
	/usr/local/common/gcc-7.3.0/bin/g++ project_0_one_thread.cpp -o one -O3 -lm -fopenmp

clean: 
	rm -f one four
all: test 

test: 
	sh script.sh

proj: 
	g++ -o falseSharingFix1 falseSharingFix1.cpp -lm -fopenmp
	g++ -o falseSharingFix2 falseSharingFix2.cpp -lm -fopenmp

clean:
	rm -f falseSharingFix1 falseSharingFix2

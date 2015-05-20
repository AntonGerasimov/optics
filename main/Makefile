all: clean
	g++ main.cc -o a -Wall -lpthread -fopenmp
clean:
	rm -r -f a OpticLabServer
debug:  clean
	g++ main.cc -o a -Wall -lpthread -g
pdebug: clean
	g++ main.cc -o a -Wall -lpthread -fopenmp -g
release: clean
	g++ main.cc -o OpticLabServer -Wall -lpthread -fopenmp -O3

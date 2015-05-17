all: clean
	g++ main.cc -o a -Wall -lpthread
clean:
	rm -r -f a OpticLabServer
debug: clean
	g++ main.cc -o a -Wall -lpthread -g
release: clean
	g++ main.cc -o OpticLabServer -Wall -lpthread -O3

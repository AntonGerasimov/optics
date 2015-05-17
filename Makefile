all:
	g++ main.cc -o a -Wall -lpthread
clean:
	rm -r -f a
debug:
	g++ main.cc -o a -Wall -lpthread -g

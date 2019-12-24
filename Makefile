compile: dragonshell.cc execute
	g++ -Wall -std=c++11 dragonshell.o -o dragonshell

execute: dragonshell.cc
	g++ -c -Wall -std=c++11 dragonshell.cc

clean:
	rm -f dragonshell dragonshell.0

compress:
	tar -cvf ahaammed-dragonshell.tar.gz dragonshell.cc readme.md Makefile
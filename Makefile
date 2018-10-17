all:
	rm -rf bin/
	mkdir bin
	gcc src/main.c -o bin/transpose

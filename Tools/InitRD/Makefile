CC=gcc
CFLAGS= -std=c99 -w  -I ../../Shared/headers
LDFLAGS=

main: main.o md5.o md5use.o
	gcc -o ./bin/RamdiskCompiler main.o md5.o md5use.o

clean:
	rm ./bin/RamdiskCompiler *.o

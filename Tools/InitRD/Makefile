CC=gcc
CFLAGS= -w  -I ../../Shared/headers
LDFLAGS=

main: main.o md5.o md5use.o
	gcc -o ./Build/RamdiskCompiler main.o md5.o md5use.o

clean:
	rm ./Build/RamdiskCompiler *.o

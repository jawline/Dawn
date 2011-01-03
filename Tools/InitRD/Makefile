CC=gcc
CFLAGS= -w  -I ../../Shared/headers -m32
LDFLAGS=-melf_i386

main: main.o md5.o md5use.o
	gcc -o ./Build/RamdiskCompiler main.o md5.o md5use.o -m32

clean:
	rm ./Build/RamdiskCompiler *.o

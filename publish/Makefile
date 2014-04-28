all: publish

clean:
	rm -f publish publish.o

mario: publish.c \
	gcc -g -std=c99 -O3 -Wall -fPIC -DLINUX32 -D__DEBUG__  -gstabs+ \
	-o publish \
	publish.c \
	-I/usr/local/include/ \
	-L/usr/local/lib/ -L/usr/lib/ \
	-lpthread

all: subscribe

clean:
	rm -f subscribe subscribe.o

mario: subscribe.c \
	gcc -g -std=c99 -O3 -Wall -fPIC -DLINUX32 -D__DEBUG__  -gstabs+ \
	-o subscribe \
	subscribe.c \
	-I/usr/local/include/ \
	-L/usr/local/lib/ -L/usr/lib/ \
	-lpthread

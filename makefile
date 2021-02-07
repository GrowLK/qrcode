all: qrcode.o main.o
	g++ -o main qrcode.o main.o

qrcode.o: qrcode.h qrcode.cpp
	g++ -g -c -w -o qrcode.o qrcode.cpp
main.o: qrcode.h main.cpp
	g++ -g -c -w -o main.o main.cpp

clean:
	rm -rf *.o main
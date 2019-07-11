CC:=mipsel-openwrt-linux-gcc
	
main:main.o database.o
	$(CC) -o $@ $^ -g
clean:
	rm *.o main -rf

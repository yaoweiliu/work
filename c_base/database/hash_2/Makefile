CC:=mipsel-openwrt-linux-gcc

main:hash.o main.o
	$(CC) -std=c99 -o $@ $^
clean:
	rm *.o main -rf


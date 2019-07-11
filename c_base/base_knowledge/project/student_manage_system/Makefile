CC:=mipsel-openwrt-linux-gcc

main:display.o main.o file.o
	$(CC) -o $@ $^ -g

clean:
	rm *.o main -f

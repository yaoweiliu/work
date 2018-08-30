CFLAGS += -lpthread
main:server.o
	gcc -o $@ $^ $(CFLAGS)
clean:
	rm *.o main -rf

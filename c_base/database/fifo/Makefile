TARGET:=main
OBJECT:=main.o fifo.o
GCC:=gcc

$(TARGET):$(OBJECT)
	$(GCC) -o $@ $^

clean:
	rm $(TARGET) $(OBJECT) -rf
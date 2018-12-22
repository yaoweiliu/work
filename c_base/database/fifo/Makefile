TARGET:=main
OBJECT:=main.o fifo.o
GCC:=gcc

$(TARGET):$(OBJECT)
	$(GCC) -o $@ $^ -g

clean:
	rm $(TARGET) $(OBJECT) core -rf

GCC:=gcc
CFLAGS = -lrt
TARGET:=timer
OBJECT:=timer.o

$(TARGET):$(OBJECT)
	$(GCC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJECT) $(TARGET)

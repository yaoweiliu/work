GCC:=gcc
CFLAGS = -lrt
TARGET:=main
OBJECT:=main.o multi_button.o

$(TARGET):$(OBJECT)
	$(GCC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJECT) $(TARGET)

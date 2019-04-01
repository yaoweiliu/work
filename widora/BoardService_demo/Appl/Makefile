CFLAG := -pthread
CFLAG += -lrt

OBJS:=DaemonService.o Dht22Mgr.o Main.o MainService.o MgrBase.o Module.o NetworkMgr.o Process.o ProcessMgr.o

boardService:$(OBJS)
	g++ -o $@ $^ $(CFLAG)

clean:
	rm *.o boardService


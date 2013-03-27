#
# Makefile for database module
#
include ../Make.public
all: IStartServer.o Server.o ThreadMod.o ListenThread.o ConnectThread.o Handler.o TreatorBase.o ConnectThread.o Utility.o ConfigReader.o Item.o ItemMgr.o
# MessageProtocol.o 

$(DEPS):
	$(CC) -c $^ $(CPPFLAGS)

clean:
	rm -f *.o


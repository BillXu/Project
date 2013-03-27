#
include ../Make.public

TARGET = GameServer
SRC= StartServer.cpp ServerTreator.cpp GameTreator.cpp User.cpp UserMgr.cpp Task.cpp ItemBase.cpp General.cpp Equip.cpp CommItem.cpp PlrItemMgr.cpp TimeUpdater.cpp Relation.cpp RelationMgr.cpp shopHistory.cpp TopUser.cpp Challenge.cpp ChallTask.cpp Battle.cpp

all :
	$(CC) -c $(SRC) $(CPPFLAGS)

clean :
	rm -f ./*.o ./$(TARGET)


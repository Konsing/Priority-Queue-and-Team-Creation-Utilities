//MakeFile
CC=g++
DEV=-Wall -g -std=c++14
OPT=-O3 -std=c++14
FLAGS=$(DEV)
JSON=json.hpp

all: buildheap.exe createteams.exe createteamdata.exe createheapoperationdata.exe

priorityqueue.o: priorityqueue.cpp priorityqueue.h
	$(CC) $(FLAGS) -c priorityqueue.cpp

teamdata.o: teamdata.cpp teamdata.h $(JSON)
	$(CC) $(FLAGS) -c teamdata.cpp

buildheap.exe: buildheap.cpp priorityqueue.o $(JSON)
	$(CC) $(FLAGS) buildheap.cpp priorityqueue.o -o buildheap.exe

createheapoperationdata.exe: createheapoperationdata.cpp priorityqueue.o $(JSON)
	$(CC) $(FLAGS) createheapoperationdata.cpp priorityqueue.o -o createheapoperationdata.exe

createteamdata.exe: createteamdata.cpp $(JSON)
	$(CC) $(FLAGS) createteamdata.cpp -o createteamdata.exe

createteams.exe: createteams.cpp priorityqueue.o $(JSON)
	$(CC) $(FLAGS) createteams.cpp priorityqueue.o -o createteams.exe

clean:
	rm -f *.o
	rm -f *.o3
	rm -f *.exe
	rm -rf *.exe.dSYM

update:
	make clean
	make all

CFLAGS= -lWs2_32 -w -shared 
LIBPATH=../lib/
FILEPATH=-I. ./funclib/ ./src/

CXX=-std=c++11 -Wall
THREAD=-lpthread
DEBUG= -g
OFILE=$(wildcard $(LIBPATH)*.o)

function=initParser.o dataQueue.o logwriter.o simplefunc.o

%.o: %.cpp %.h
	g++ -std=c++11 -c -fPIC $< -o $(LIBPATH)$@ 

initParser:
	g++ -c -DBDLL $(LIB)initParser.cpp -o $(LIBPATH)initParser.o

dataQueue:
	g++ -c -DBDLL $(LIB)dataQueue.cpp -o $(LIBPATH)dataQueue.o

logwriter:
	g++ -c -DBDLL $(LIB)logwriter.cpp -o $(LIBPATH)logwriter.o

simplefunc:
	g++ -c -DBDLL $(LIB)logwriter.cpp -o $(LIBPATH)simplefunc.o

%.a: 
	ar crsv $(LIBPATH)$@ $(wildcard $(LIBPATH)*.o)

%.so: 
	g++ -shared -o $(LIBPATH)$@ $(wildcard $(LIBPATH)*.o)
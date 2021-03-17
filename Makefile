# CFLAGS= -lWs2_32 -w 
# -lWs2_32 use for socket
# -w ignore warning
SERVER=server.cpp
CLIENT=client.cpp
FILEPATH=./funclib/
LIBPATH=./lib/
CXX=-std=c++11 -Wall -W -pthread
THREAD=-lpthread
DEBUG= -g -w
BIN=./lib/
LIB=-lcommonLib -lstdc++ -lpthread

CXXFILE=main.cpp
TARGET=-o test.exe
SERVER_TARGET=-o server.out
CLIENT_TARGET=-o client.out

LIBS=$(wildcard $(LIB)*.cpp)
OBJS=$(LIBS:.cpp=.o )

OFILE=$(wildcard $(LIBPATH)*.o)

main: $(CXXFILE)
	g++ $(DEBUG) $(CXXFILE) ./lib/libcommon.so ./funclib/dataQueue.cpp connection.cpp  $(SERVER) \
	$(CXX) $(SERVER_TARGET)

initParser:
	g++ -std=c++11 -fPIC -c $(FILEPATH)initParser.cpp -o $(BIN)initParser.o

dataQueue:
	g++ -std=c++11 -fPIC -c $(FILEPATH)dataQueue.cpp -o $(BIN)dataQueue.o

logwriter:
	g++ -std=c++11 -fPIC -c $(FILEPATH)logwriter.cpp -o $(BIN)logwriter.o

%.so: 
	g++ $(OFILE) -shared -o $(LIBPATH)$@
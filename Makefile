# CFLAGS= -lWs2_32 -w 
# -lWs2_32 use for socket
# -w ignore warning
SERVER=server.cpp
TRADER=trader.cpp
CLIENT=client.cpp
FILEPATH=./funclib/
LIBPATH=./lib/
CXX=-std=c++11 -Wall -W -pthread
THREAD=-lpthread
SSLFLAG=-lcrypto
DBFLAG=-lmysqlclient
DEBUG= -g -w
BIN=./lib/
LIB=-lcommonLib -lstdc++ -lpthread

CXXFILE=main.cpp
TARGET=-o test.exe
TRADER_SERVER=-o trader.out
SERVER_TARGET=-o server.out
CLIENT_TARGET=-o client.out

LIBS=$(wildcard $(LIB)*.cpp)
OBJS=$(LIBS:.cpp=.o )

OFILE=$(wildcard $(LIBPATH)*.o)

main: $(CXXFILE)
	g++ $(DEBUG) $(CXXFILE) $(DBFLAG) $(SSLFLAG) ./lib/libcommon.so ./funclib/dataQueue.cpp ./funclib/simplefunc.cpp \
	connection.cpp ./database/mariaDBHandler.cpp ./database/tradingDataHandler.cpp $(SERVER) $(TRADER) \
	$(CXX) $(TRADER_SERVER)

testclient: $(CXXFILE)
	g++ $(DEBUG) ./lib/libcommon.so ./funclib/dataQueue.cpp ./testing_client/$(CLIENT) \
	$(CXX) $(CLIENT_TARGET)

initParser:
	g++ -std=c++11 -fPIC -c $(FILEPATH)initParser.cpp -o $(BIN)initParser.o

dataQueue:
	g++ -std=c++11 -fPIC -c $(FILEPATH)dataQueue.cpp -o $(BIN)dataQueue.o

logwriter:
	g++ -std=c++11 -fPIC -c $(FILEPATH)logwriter.cpp -o $(BIN)logwriter.o

%.so: 
	g++ $(OFILE) -shared -o $(LIBPATH)$@
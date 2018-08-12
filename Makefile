
LIBOTE_PATH = ../libOTe

INC_PATH = -I $(LIBOTE_PATH) -I $(LIBOTE_PATH)/cryptoTools -I $(LIBOTE_PATH)/cryptoTools/thirdparty/linux

LIBOTE_LIB = -L $(LIBOTE_PATH)/lib -l libOTe -l cryptoTools
BOOST_LIB = -L $(LIBOTE_PATH)/cryptoTools/thirdparty/linux/boost/stage/lib -l boost_system -l boost_thread
MIRACL_LIB = -L $(LIBOTE_PATH)/cryptoTools/thirdparty/linux/miracl/miracl/source -l miracl
OTHER_LIB = -l pthread

LIB_PATH = $(LIBOTE_LIB) $(BOOST_LIB) $(MIRACL_LIB) $(OTHER_LIB)

CC = g++
CFLAGS = -Wall -std=c++14 $(INC_PATH)

SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
NAME = ot

all: sender receiver

sender: sender.o
	$(CC) $(CFLAGS) sender.o $(LIB_PATH) -o sender

receiver: receiver.o
	$(CC) $(CFLAGS) receiver.o $(LIB_PATH) -o receiver

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f sender receiver *.o



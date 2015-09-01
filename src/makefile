all: llama

rlwrap: llama
	cd rlwrap*;./configure;make;cd ..

OBJS = getCPUTime.o utils.o node.o parser.o main.o tokens.o reduction.o

CC = g++
CPPFLAGS = -O2
#CPPFLAGS = -std=c++11
# LDFLAGS = -lpthread -ldl -lz -rdynamic
LIBS = -lfl -lrt

parser.cpp: parser.y
	bison -d -o $@ $^

parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	flex $^;mv lex.yy.c tokens.cpp

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

llama: $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -rf llama $(OBJS)
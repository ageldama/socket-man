.PHONY: all clean

CXX ?= g++

CXXFLAGS=-g -Wall -fexceptions -std=c++17 -I./json/include -I./spdlog/include 
LDFLAGS=


%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

TCP_SERV_OBJS=tcp_serv.o

tcp_serv: $(TCP_SERV_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

all: tcp_serv

clean:
	rm -vf tcp_serv $(TCP_SERV_OBJS)

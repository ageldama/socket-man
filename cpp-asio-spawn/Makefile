.PHONY: all clean

all: tcp_serv

CXX ?= g++

BOOST_INCLUDE_DIR ?= /usr/include
BOOST_LIB_DIR ?= /usr/lib

CXXFLAGS=-g -Wall -fexceptions -std=c++17 \
	-DSPDLOG_FMT_EXTERNAL \
	-DFMT_HEADER_ONLY \
	$(pkg-config --cflags spdlog) \
	$(pkg-config --cflags fmt) \
	-I$(BOOST_INCLUDE_DIR) -I.

LDFLAGS=-L$(BOOST_LIB_DIR) -lboost_coroutine -lboost_context -lboost_system -lboost_json \
	$(pkg-config --libs spdlog)

TCP_SERV_OBJS=tcp_serv.o

TCP_SERV_GCHS=pch.hpp.gch


%.hpp.gch: %.hpp
	$(CXX) -x c++-header $(CXXFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

tcp_serv.o: pch.hpp.gch

tcp_serv: $(TCP_SERV_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<


clean:
	rm -vf tcp_serv $(TCP_SERV_OBJS) $(TCP_SERV_GCHS)

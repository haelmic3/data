C=g++ -Wall --std=c++11 -O2 -Wshadow
F=-Wall --std=c++11 -O2 -Wshadow
S=BigInt.opp
T=Framerate.opp
P=xen hash comp
O=$(wildcard *.opp)
SFML=-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network
all: $P
comp: comp.opp $S
	$C -o $@ $^
hash: hash.opp $S
	$C -o $@ $^ -pthread
xen: xen.opp $S $T
	$C -o $@ $^ $(SFML) `Magick++-config --ldflags --libs`
xen.opp:xen.cpp
	$C -fopenmp -c $<  -o $@

%.opp:%.cpp
	$C -c $<  -o $@
clean:
	rm $O $P
.PHONY: clean all

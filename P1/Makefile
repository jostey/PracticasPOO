CXX = g++-5
CXXFLAGS = -std=c++14 -pedantic -Wall
LDFLAGS = -IFecha -ICadena -I..

STD = c++14
EXES = test-P1-auto test-P1-consola
OBJS = test-P1-auto.o test-P1-consola.o fecha.o cadena.o

all: $(EXES)

test-P1-auto: test-P1-auto.o fecha.o cadena.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test-P1-consola: test-P1-consola.o fecha.o cadena.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test-P1-auto.o: test-P1-auto.cpp cadena.hpp fecha.hpp
	$(CXX) -c $(CXXFLAGS) $< $(LDFLAGS) -o $@

test-P1-consola.o: test-P1-consola.cpp cadena.hpp fecha.hpp
	$(CXX) -c $(CXXFLAGS) $< $(LDFLAGS) -o $@

fecha.o: fecha.cpp fecha.hpp
	$(CXX) -c $(CXXFLAGS) $< $(LDFLAGS) -o $@

cadena.o: cadena.cpp cadena.hpp
	$(CXX) -c $(CXXFLAGS) $< $(LDFLAGS) -o $@

# Hace comprobaciones en el código fuente
cadena_check fecha_check: cadena_check.cpp fecha_check.cpp
	make -f Make_check.mk

check: cadena_check fecha_check
	./cadena_check cadena.cpp -- -std=${STD}
	./fecha_check fecha.cpp -- -std=${STD}

clean:
	$(RM) $(OBJS) $(EXE) *~ */*~

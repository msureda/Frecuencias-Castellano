all: calculo_frecuencias

clean:
	rm -f calculo_frecuencias *.o *% *~

calculo_frecuencias: calculo_frecuencias.o
	g++ -g -o $@ calculo_frecuencias.o

calculo_frecuencias.o: calculo_frecuencias.cpp calculo_frecuencias.hpp
	g++ -g -c -o $@ calculo_frecuencias.cpp

CC = g++
CXXFLAGS = -Wall -std=c++11 -Iinclude

EXEC = bin/tp2.out

OBJS = obj/main.o obj/Dicionario.o obj/Grafo.o obj/Sistema.o obj/Tema.o obj/Usuario.o
ZIP = tp2.zip

all: directories $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $(EXEC)

obj/main.o: src/main.cpp include/Sistema.hpp include/Grafo.hpp include/Lista.hpp
	$(CC) $(CXXFLAGS) -c src/main.cpp -o obj/main.o

obj/Dicionario.o: src/Dicionario.cpp include/Dicionario.hpp include/Usuario.hpp include/Tema.hpp include/Grafo.hpp
	$(CC) $(CXXFLAGS) -c src/Dicionario.cpp -o obj/Dicionario.o

obj/Grafo.o: src/Grafo.cpp include/Grafo.hpp include/Lista.hpp
	$(CC) $(CXXFLAGS) -c src/Grafo.cpp -o obj/Grafo.o

obj/Sistema.o: src/Sistema.cpp include/Sistema.hpp include/Dicionario.hpp include/Grafo.hpp
	$(CC) $(CXXFLAGS) -c src/Sistema.cpp -o obj/Sistema.o

obj/Tema.o: src/Tema.cpp include/Tema.hpp
	$(CC) $(CXXFLAGS) -c src/Tema.cpp -o obj/Tema.o

obj/Usuario.o: src/Usuario.cpp include/Usuario.hpp
	$(CC) $(CXXFLAGS) -c src/Usuario.cpp -o obj/Usuario.o

directories:
	@mkdir -p obj bin

clean:
	rm -f obj/*.o bin/tp2.out ZIP

zip:
	zip -r $(ZIP) Makefile src/ include/ bin/ obj/ -x "bin/*" -x "obj/*" -x "*/.git*"
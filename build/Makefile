SRC = ../


all:$(OBJ)
	g++ -std=c++11 -O2 -o ${EXE} $(OBJ) -fopenmp

%.o: ../src/%.cpp
	g++ -std=c++11 -O2 -c $^ -o $@ -I${SRC}/include -fopenmp

.PHONY: clean
clean:
	rm -f *.o


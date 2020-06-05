SRC = ../


all:$(OBJ)
	g++ ${ENABLE} -std=c++11 -O2 -o ${EXE} $(OBJ) -fopenmp

%.o: ../src/%.cpp
	g++ ${ENABLE} -std=c++11 -O2 -c $^ -o $@ -I${SRC}/include -fopenmp

.PHONY: clean
clean:
	rm -f *.o


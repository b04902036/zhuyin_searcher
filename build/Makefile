SRC = ../


all:$(OBJ)
	${CXX} ${CXXFLAGS} ${ENABLE} -std=c++11 -O2 -o ${EXE} $(OBJ)

%.o: ${SRC}/src/%.cpp
	${CXX} ${CXXFLAGS} ${ENABLE} -std=c++11 -O2 -c $^ -o $@ -I${SRC}/include/ 

.PHONY: clean
clean:
	rm -f *.o


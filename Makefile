obj:=search.o util.o methods.o

all:$(obj)
	mkdir -p result
	g++ -std=c++11 -O2 -o search $(obj) -fopenmp
%.o:src/%.cpp
	g++ -std=c++11 -O2 -c $^ -o $@ -I./include -fopenmp

.PHONY: clean
clean:
	rm -f ./*.o


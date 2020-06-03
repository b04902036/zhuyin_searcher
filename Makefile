obj:=search.o util.o methods.o

all:$(obj)
	g++ -std=c++11 -O2 -o search $(obj)
%.o:src/%.cpp
	g++ -std=c++11 -O2 -c $^ -o $@ -I./include

.PHONY: clean
clean:
	rm -f ./*.o


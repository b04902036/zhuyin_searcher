OBJ:=search.o util.o notype.o onlychinese.o
EXE:=search


all:
	$(MAKE) -C ./build all
	cp ./build/${EXE} ./

.PHONY: clean
clean:
	rm -f ./build/*.o ./build/${EXE} ./${EXE}

export OBJ
export EXE

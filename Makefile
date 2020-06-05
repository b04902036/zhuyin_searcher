OBJ:=search.o util.o noType.o onlyChinese.o
EXE:=search
BUILD:=./build
ENABLE+=-DNOTYPE -DONLYCHINESE


all:
	$(MAKE) -C ${BUILD} all
	cp ${BUILD}/${EXE} ./

.PHONY: clean
clean:
	rm -f ${BUILD}/*.o ${BUILD}/${EXE} ./${EXE}




export OBJ
export EXE
export BUILD
export ENABLE

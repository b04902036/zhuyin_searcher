CC?=gcc
CXX?=g++
CFLAGS+=-fopenmp
CXXFLAGS+=${CFLAGS}
OBJ:=search.o util.o noType.o onlyChinese.o atLeastTwo.o stateMachine.o
EXE:=search
BUILD:=./build
# ENABLE+=-DATLEASTTWO
# ENABLE+=-DNOTYPE 
ENABLE+=-DONLYCHINESE 
ENABLE+=-DSTATEMACHINE


all:
	$(MAKE) -C ${BUILD} all
	cp ${BUILD}/${EXE} ./

.PHONY: clean
clean:
	rm -f ${BUILD}/*.o ${BUILD}/${EXE} ./${EXE}



export CC
export CXX
export CFLAGS
export CXXFLAGS
export OBJ
export EXE
export BUILD
export ENABLE

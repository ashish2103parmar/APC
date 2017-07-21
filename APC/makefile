TARGET := ../Lib/libapc.so
SRC := $(wildcard *.c)
Cflag := -I ../Include
LFLAG := -L ../Lib 

all: ${TARGET}

${TARGET} : ${SRC}
		gcc -fPIC -shared $^ ${LFLAG} ${Cflag} -o $@

clean:
		rm ${TARGET}

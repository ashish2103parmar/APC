SRCS  := $(wildcard *.c) $(wildcard APC/*.c)
TARGET := APCTEST
CFLAG := -I Include/

all : ${TARGET} 

${TARGET}:${SRCS}
	gcc  $^ ${LFLAG} ${CFLAG} -o $@

clean:
		rm ${TARGET}

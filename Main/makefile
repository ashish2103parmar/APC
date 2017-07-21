SRCS  := $(wildcard *.c)
TARGET := $(SRCS:.c=.out)
LFLAG := -L ../Lib -lfun -lbitwise -llinklist -lstack -lqueue -lq2s -lInfix -lsearch -lsort -ltree -lapc -lhtable
CFLAG := -I ../Include

all : ${TARGET} 

%.out:%.c
	gcc  $^ ${LFLAG} ${CFLAG} -o $@

clean:
		rm ${TARGET}

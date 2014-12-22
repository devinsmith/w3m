CFLAGS+=	-Wall -I../../include -DDUMMY

all: dummy.o

dummy.o:
	${CC} -c ${CFLAGS} ../../src/entity.c -o dummy.o

clean:
	rm dummy.o

install:

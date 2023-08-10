CC=gcc
SRC=src/uploader
INCLUDE=${SRC}/include
BIN=bin

CFLAGS=-Wall -fPIC
LIB=-lcurl

dev:
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/core.c			-o	${BIN}/core.o
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/misc/logs.c		-o	${BIN}/logs.o
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/misc/utils.c	-o	${BIN}/utils.o
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/misc/cJSON.c	-o	${BIN}/cJSON.o
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/server/upload.c	-o	${BIN}/upload.o
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/server/users.c	-o	${BIN}/users.o

	${CC} -shared ${BIN}/core.o ${BIN}/logs.o ${BIN}/utils.o ${BIN}/cJSON.o ${BIN}/upload.o ${BIN}/users.o -o ${BIN}/core.so ${LIB}
	del bin\*.o

test_download:
	cc -Wall src/updater/download.c -o bin/download.exe -I src/updater/include -lcurl

test_extract:
	cc -Wall -c src/updater/extract.c -o bin/extract.o -I src/updater/include
	cc -Wall -c src/updater/zip.c -o bin/zip.o -I src/updater/include

	cc -o bin/extract.exe bin/extract.o bin/zip.o

	del bin\*.o
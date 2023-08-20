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

prod:
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/core.c			-o	${BIN}/core.o	-O2
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/misc/logs.c		-o	${BIN}/logs.o	-O2
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/misc/utils.c	-o	${BIN}/utils.o	-O2
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/misc/cJSON.c	-o	${BIN}/cJSON.o	-O2
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/server/upload.c	-o	${BIN}/upload.o	-O2
	${CC} ${CFLAGS} -I ${INCLUDE} -c ${SRC}/server/users.c	-o	${BIN}/users.o	-O2

	${CC} -shared ${BIN}/core.o ${BIN}/logs.o ${BIN}/utils.o ${BIN}/cJSON.o ${BIN}/upload.o ${BIN}/users.o -o ${BIN}/core.so ${LIB} -O2
	del bin\*.o




test_launcher:
	gcc -Wall src/updater/extract.c src/updater/zip.c src/updater/download.c src/updater/launcher.c -I src/updater/include -o launcher.exe -lcurl

test_download:
	cc -Wall src/updater/download.c -o bin/download.exe -I src/updater/include -lcurl

test_extract:
	cc -Wall -c src/updater/extract.c -o bin/extract.o -I src/updater/include
	cc -Wall -c src/updater/zip.c -o bin/zip.o -I src/updater/include

	cc -o bin/extract.exe bin/extract.o bin/zip.o

	del bin\*.o
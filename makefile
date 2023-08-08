dev:
	cc -Wall -fPIC -c src/c/core.c -o			bin/core.o		-I src/c/include
	cc -Wall -fPIC -c src/c/misc/logs.c -o		bin/logs.o		-I src/c/include
	cc -Wall -fPIC -c src/c/misc/utils.c -o		bin/utils.o		-I src/c/include
	cc -Wall -fPIC -c src/c/misc/cJSON.c -o		bin/cJSON.o		-I src/c/include
	cc -Wall -fPIC -c src/c/server/upload.c -o	bin/upload.o	-I src/c/include
	cc -Wall -fPIC -c src/c/server/users.c -o	bin/users.o		-I src/c/include

	cc -shared -o bin/core.so bin/core.o bin/logs.o bin/utils.o bin/cJSON.o bin/upload.o bin/users.o -lcurl

	del bin\*.o

test_download:
	cc -Wall src/updater/download.c -o bin/download.exe -I src/updater/include -lcurl

test_extract:
	cc -Wall -c src/updater/extract.c -o bin/extract.o -I src/updater/include
	cc -Wall -c src/updater/zip.c -o bin/zip.o -I src/updater/include

	cc -o bin/extract.exe bin/extract.o bin/zip.o

	del bin\*.o
dev:
	cc -Wall -fPIC -c src/c/core.c -o			bin/core.o		-I src/c/include
	cc -Wall -fPIC -c src/c/misc/logs.c -o		bin/logs.o		-I src/c/include
	cc -Wall -fPIC -c src/c/misc/utils.c -o		bin/utils.o		-I src/c/include
	cc -Wall -fPIC -c src/c/misc/cJSON.c -o		bin/cJSON.o		-I src/c/include
	cc -Wall -fPIC -c src/c/server/upload.c -o	bin/upload.o	-I src/c/include
	cc -Wall -fPIC -c src/c/server/users.c -o	bin/users.o		-I src/c/include

	cc -shared -o bin/core.so bin/core.o bin/logs.o bin/utils.o bin/cJSON.o bin/upload.o bin/users.o -lcurl

	del bin\*.o

updater:
	cc -Wall src/updater/auto_updater.c -o bin/updater.exe -I src/updater/include -L lib/curl -lcurl

prova:
	cc -Wall -c src/updater/replacer.c -o bin/replacer.o
	cc -Wall -c src/updater/zip.c -o bin/zip.o -I src/updater/include

	cc -o bin/extractor.exe bin/replacer.o bin/zip.o
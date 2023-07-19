dev:
	cc -Wall -fPIC -c src/c/core.c -o bin/core.o -I src/c/include
	cc -Wall -fPIC -c src/c/server.c -o bin/server.o -I src/c/include
	cc -Wall -fPIC -c src/c/utils.c -o bin/utils.o -I src/c/include
	cc -fPIC -c src/c/cJSON.c -o bin/cJSON.o -I src/c/include

	cc -shared -o bin/core.so bin/core.o bin/server.o bin/utils.o bin/cJSON.o -lcurl
	del bin\core.o bin\server.o bin\utils.o bin\cJSON.o

updater:
	cc -Wall src/updater/auto_updater.c -o bin/updater.exe -I src/updater/include -L lib/curl -lcurl

prova:
	cc -Wall -c src/updater/replacer.c -o bin/replacer.o
	cc -Wall -c src/updater/zip.c -o bin/zip.o -I src/updater/include

	cc -o bin/extractor.exe bin/replacer.o bin/zip.o
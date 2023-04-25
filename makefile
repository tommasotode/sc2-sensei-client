dev:
	cc -Wall -fPIC -c src/c/core.c -o bin/core.o -I src/c/include
	cc -Wall -fPIC -c src/c/server.c -o bin/server.o -I src/c/include
	cc -Wall -fPIC -c src/c/utils.c -o bin/utils.o -I src/c/include
	cc -fPIC -c src/c/cJSON.c -o bin/cJSON.o -I src/c/include

	cc -shared -o bin/core.so bin/core.o bin/server.o bin/utils.o bin/cJSON.o -L lib/curl -lcurl
	del bin\core.o bin\server.o bin\utils.o bin\cJSON.o

updater:
	cc -Wall src/updater/auto_updater.c -o bin/updater.exe -I src/updater/include -L lib/curl -lcurl
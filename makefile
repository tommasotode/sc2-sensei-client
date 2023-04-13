dev:
	cc -Wall -fPIC -c src/core.c -o bin/core.o -I src/include
	cc -Wall -fPIC -c src/server.c -o bin/server.o -I src/include
	cc -Wall -fPIC -c src/utils.c -o bin/utils.o -I src/include
	cc -fPIC -c src/cJSON.c -o bin/cJSON.o -I src/include

	cc -shared -o bin/core.so bin/core.o bin/server.o bin/utils.o bin/cJSON.o -L lib/curl/lib -lcurl
	del bin\core.o bin\server.o bin\utils.o bin\cJSON.o
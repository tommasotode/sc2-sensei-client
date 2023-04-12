dev:
	cc -Wall -fPIC -c src/core.c -o bin/core.o -I src/include
	cc -Wall -fPIC -c src/server.c -o bin/server.o -I src/include
	cc -fPIC -c src/cJSON.c -o bin/cJSON.o -I src/include
	cc -shared -o bin/core.so bin/core.o bin/cJSON.o bin/server.o -L lib/curl/lib -lcurl
	del bin\core.o bin\cJSON.o bin\server.o
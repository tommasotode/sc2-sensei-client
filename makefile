dev:
	cc -fPIC -shared -Wall src/core.c src/cJSON.c -I src/include -o bin/core.so -L lib/curl/lib -lcurl
dev:
	cc -Wall -fPIC -c src\core.c -o bin\core.o -I"src\include"
	cc -fPIC -c src\cJSON.c -o bin\cJSON.o -I"src\include"
	cc -shared -o bin\core.so bin\core.o bin\cJSON.o -L"lib\curl\lib" -lcurl

# cc -fPIC -shared -Wall src/core.c src/cJSON.c -I src/include -o bin/core.so -L lib/curl/lib -lcurl
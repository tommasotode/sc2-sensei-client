main:
	cc -fPIC -shared client/core.c -I lib/curl/include -o lib/core.so -L lib/curl/lib -lcurl
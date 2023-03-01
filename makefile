main:
	cc -fPIC -shared client/core.c -L lib/curl/lib -l curl -o lib/core.so
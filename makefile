main1:
	cc -fPIC -shared -o core.so core.c -L"C:/Users/tomma/sc2-sensei/upload_client/curl-7.86.0-win64-mingw/lib" -lcurl

main2:
	cc -fPIC -shared -o core.so core.c -L"C:/Users/Tommaso/sc2-sensei/upload_client/curl-7.86.0-win64-mingw/lib" -lcurl
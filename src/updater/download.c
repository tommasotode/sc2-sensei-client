#include <auto_updater.h>

int download()
{
	printf("Updating...\n\n");
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL *handle = curl_easy_init();
	if(!handle)
	{
		perror("\n[!] Curl init failed\n");
		goto cleanup;
	}
	FILE *outfile;
	outfile = fopen("temp.zip", "wb");
	
	// Just for now we are gonna pretend that there will always be an update

	curl_easy_setopt(handle, CURLOPT_URL, UPDATE_ENDPOINT);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, outfile);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
	CURLcode res = curl_easy_perform(handle);
	
	fclose(outfile);
	if(res != CURLE_OK)
	{
		fprintf(stderr, "[!] Curl perform failed: %s\n", curl_easy_strerror(res));
		goto cleanup;
	}
	
	long http_code = 0;
	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);
	if(http_code != 200)
	{
		printf("\n[!] HTTP failure - %ld\n", http_code);
		goto cleanup;
	}

	cleanup:
	curl_global_cleanup();
	curl_easy_cleanup(handle);

	return 0;
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}
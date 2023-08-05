#include <core.h>

bool check_user(char username[MAX_USERNAME])
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	bool isValid = false;
	Response response = {.text = malloc(1), .size = 0};

	char name_check[MAX_USERNAME + 10] = "username: ";
	strcat_s(name_check, sizeof(name_check), username);
	struct curl_slist *header = NULL;
	header = curl_slist_append(header, name_check);

	CURL *handle = curl_easy_init();
	if (!handle)
	{
		perror("\n[!] Curl init failed\n");
		goto cleanup;
	}

	curl_easy_setopt(handle, CURLOPT_URL, USERNAME_ENDPOINT);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&response);
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);
	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);

	CURLcode res = curl_easy_perform(handle);
	if (res != CURLE_OK)
	{
		fprintf(stderr, "[!] Curl perform failed: %s\n", curl_easy_strerror(res));
		goto cleanup;
	}

	long http_code = 0;
	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);
	if (http_code != 200)
	{
		printf("\n[!] HTTP failure - %ld\n", http_code);
		goto cleanup;
	}

	// TODO: In the future, the server will also search for similar names
	cJSON *response_json = cJSON_ParseWithLength(response.text, response.size);
	const cJSON *state = cJSON_GetObjectItem(response_json, "result");
	cJSON_Delete(response_json);
	isValid = state->valueint;

cleanup:
	curl_global_cleanup();
	curl_slist_free_all(header);
	curl_easy_cleanup(handle);
	free(response.text);

	return isValid;
}
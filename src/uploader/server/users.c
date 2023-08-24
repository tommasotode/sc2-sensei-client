#include <core.h>

char *check_valid_user(char username[MAX_USERNAME], char password[MAX_PASSWORD])
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	bool isValid = false;
	Response response = {.text = malloc(1), .size = 0};

	CURL *handle = curl_easy_init();
	if (!handle)
	{
		perror("\n[!] Curl init failed\n");
		goto cleanup;
	}

	curl_easy_setopt(handle, CURLOPT_URL, USERNAME_ENDPOINT);

	char request_content[MAX_USERNAME+MAX_PASSWORD+30];
	sprintf(request_content, "user=%s&pass=%s", username, password);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, request_content);

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&response);
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

	char *id;
	if(isValid)
	{
		const cJSON *user_id = cJSON_GetObjectItem(response_json, "user_id");
		id = strdup(cJSON_GetStringValue(user_id));
	}

cleanup:
	curl_easy_cleanup(handle);
	curl_global_cleanup();
	free(response.text);

	return id;
}
#include "include/core.h"

size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	// read from a file (size specified outside)

	FILE *readhere = (FILE *)userdata;
	size_t retcode = fread(ptr, size, nmemb, readhere);
	curl_off_t nread = (curl_off_t)retcode;
	printf("[CURL] %lli bytes read\n", nread);
	
	return retcode;
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	// create memory block and put contents in it
	
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(!ptr)
	{
		perror("Not enough memory");
		return 0;
	}
	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size = mem->size + realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

short check_username(char username[MAX_USERNAME])
{
	struct MemoryStruct response;
	response.memory = malloc(1);
	response.size = 0;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	struct curl_slist *header = NULL;
	header = curl_slist_append(header, username);
	CURL *handle = curl_easy_init();
	if(!handle)
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
	if(res != CURLE_OK)
	{
		fprintf(stderr, "[!] Curl perform failed: %s\n", curl_easy_strerror(res));
		goto cleanup;
	}
	cleanup:
	curl_global_cleanup();
	curl_easy_cleanup(handle);
	curl_slist_free_all(header);
	free(response.memory);

	return 0;
}

Replay upload_replay(FILE *replay, char replay_name[MAX_PATH], char username[MAX_USERNAME])
{
	// Unsuccessful replay initialization
	Replay current;
	strcpy_s(current.name, sizeof(current.name), replay_name);
	strcpy_s(current.id, sizeof(current.id), "");
	strcpy_s(current.parse_rslt, sizeof(current.parse_rslt), "Connection error");
	struct stat info;
	fstat(fileno(replay), &info);
	current.play_date = info.st_mtime;
	current.upload_date = time(NULL);
	current.connection = FAILURE;

	// Rsponse initialization
	struct MemoryStruct response;
	response.memory = malloc(1);
	response.size = 0;

	// Header initialization
	// TODO: Add mimetype to the header
	char rep_name[MAX_PATH + 10] = "replay_name: ";
	char player_name[MAX_USERNAME + 10] = "username: ";
	strcat_s(rep_name, sizeof(replay), replay_name);
	strcat_s(player_name, sizeof(player_name), username);
	struct curl_slist *header = NULL;
	header = curl_slist_append(header, rep_name);
	header = curl_slist_append(header, player_name);

	CURL *handle = curl_easy_init();
	if(!handle)
	{
		perror("\n[!] Curl init failed\n");
		goto cleanup;
	}
	// Set upload information and replay to read
	curl_easy_setopt(handle, CURLOPT_URL, UPLOAD_ENDPOINT);
	curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(handle, CURLOPT_READDATA, replay);
	curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)info.st_size);

	// Get server output and write it to a string
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&response);

	// Set header
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);
	
	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
	CURLcode res = curl_easy_perform(handle);
	if(res != CURLE_OK)
	{
		fprintf(stderr, "[!] Curl perform failed: %s\n\n", curl_easy_strerror(res));
		goto cleanup;
	}
	curl_off_t upload_speed, total_time;
	curl_easy_getinfo(handle, CURLINFO_SPEED_UPLOAD_T, &upload_speed);
	curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME_T, &total_time);

	fprintf(stderr, "Speed: %lu bytes/sec during %lu.%06lu seconds\n",
		(unsigned long)upload_speed,
		(unsigned long)(total_time / 1000000),
		(unsigned long)(total_time % 1000000));
	
	printf("[%luB] - %s\n\n", (unsigned long)response.size, response.memory);

	cJSON *response_json = cJSON_ParseWithLength(response.memory, response.size);
	const cJSON *parse = cJSON_GetObjectItem(response_json, "parse");
	const cJSON *replay_id = cJSON_GetObjectItem(response_json, "replay_id");
	cJSON_Delete(response_json);

	// Successful update replay
	strcpy_s(current.id, ID_LEN, replay_id->valuestring);
	current.upload_date = time(NULL);
	current.connection = SUCCESS;
	if(response.size != 0)
		strcpy_s(current.parse_rslt, sizeof(current.parse_rslt), parse->valuestring);
	
	cleanup:
	curl_slist_free_all(header);
	curl_easy_cleanup(handle);
	free(response.memory);
	
	return current;
}
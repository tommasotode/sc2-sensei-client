#include "include/core.h"

bool check_user(char username[MAX_USERNAME])
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	bool isValid = false;
	Response response = { .text = malloc(1), .size = 0 };
	
	char name_check[MAX_USERNAME + 10] = "username: ";
	strcat_s(name_check, sizeof(name_check), username);
	struct curl_slist *header = NULL;
	header = curl_slist_append(header, name_check);
	
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

	long http_code = 0;
	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);
	if(http_code != 200)
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


Response upload(Replay replay)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	Response response = { .size = 0, .text = malloc(1)};
	CURL *handle = curl_easy_init();
	if(!handle)
	{
		perror("\n[!] Curl init failed\n");
		curl_global_cleanup();
		curl_easy_cleanup(handle);
		return response;
	}
	
	// File and file descriptor
	FILE *data = fopen(replay.path, "rb");
	struct stat info;
	stat(fileno(data), &info);

	struct curl_slist *header = NULL;
	char rep_name[MAX_PATH + 10] = "replay_name: ";
	char player_name[MAX_USERNAME + 10] = "username: ";
	strcat_s(rep_name, sizeof(rep_name), replay.path);
	strcat_s(player_name, sizeof(player_name), replay.username);
	header = curl_slist_append(header, rep_name);
	header = curl_slist_append(header, player_name);

	// Set upload information and replay to read
	curl_easy_setopt(handle, CURLOPT_URL, UPLOAD_ENDPOINT);
	curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(handle, CURLOPT_READDATA, data);
	curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)info.st_size);
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);

	// Get server output and write it to a string
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&response);

	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
	CURLcode res = curl_easy_perform(handle);
	if(res != CURLE_OK)
	{
		fprintf(stderr, "[!] Curl perform failed: %s\n\n", curl_easy_strerror(res));
		free(response.text);
		response.size = 0;
		response.text = NULL;
	}

	curl_global_cleanup();
	curl_easy_cleanup(handle);
	curl_slist_free_all(header);
	fclose(data);

	return response;
}


ReplayLog parse(char replay_path[MAX_PATH], char username[MAX_USERNAME])
{
	// Unsuccessful replay initialization
	struct stat info;
	stat(replay_path, &info);
	Replay rep = { .log.connection = false, .log.connection = time(NULL), .log.play_date = info.st_mtime};
	strcpy_s(rep.path, sizeof(rep.path), replay_path);
	strcpy_s(rep.log.parse_result, sizeof(rep.log.parse_result), "CONNECTION FAILURE");
	strcpy_s(rep.log.id, sizeof(rep.log.id), "");

	Response response = { .text = malloc(1), .size = 0 };

	// Header initialization
	// TODO: Add mimetype to the header
	struct curl_slist *header = NULL;
	char rep_name[MAX_PATH + 10] = "replay_name: ";
	char player_name[MAX_USERNAME + 10] = "username: ";
	strcat_s(rep_name, sizeof(rep_name), replay_path);
	strcat_s(player_name, sizeof(player_name), username);
	header = curl_slist_append(header, rep_name);
	header = curl_slist_append(header, player_name);

	curl_global_init(CURL_GLOBAL_DEFAULT);
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
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header);

	// Get server output and write it to a string
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&response);

	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
	CURLcode res = curl_easy_perform(handle);
	if(res != CURLE_OK)
	{
		fprintf(stderr, "[!] Curl perform failed: %s\n\n", curl_easy_strerror(res));
		goto cleanup;
	}
	printf("[%luB] - %s\n", (unsigned long)response.size, response.text);

	long http_code = 0;
	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);
	if(http_code != 200)
	{
		printf("\n[!] HTTP failure - %ld\n", http_code);
		goto cleanup;
	}

	cJSON *response_json = cJSON_ParseWithLength(response.text, response.size);
	const cJSON *parse = cJSON_GetObjectItem(response_json, "parse");
	const cJSON *rep_id = cJSON_GetObjectItem(response_json, "replay_id");
	
	// Apparently, if the string is too long i can't just get it with valuestring
	// So, i need to get the string value like this
	char parse_result[MAX_PARSE];
	char replay_id[ID_LEN];
	strcpy_s(parse_result, sizeof(parse_result), cJSON_GetStringValue(parse));
	strcpy_s(replay_id, sizeof(replay_id), cJSON_GetStringValue(rep_id));
	cJSON_Delete(response_json);

	// Successful update replay
	rep.log.upload_date = time(NULL);
	rep.log.connection = true;

	// TODO: Maybe in the future I will need to change this (I don't think so)
	if(is_utf8(parse_result) && is_utf8(replay_id))
	{
		strcpy_s(rep.log.parse_result, sizeof(rep.log.parse_result), parse_result);
		strcpy_s(rep.log.id, sizeof(rep.log.id), replay_id);
	}else
	{
		strcpy_s(rep.log.parse_result, sizeof(rep.log.parse_result), "Error decoding response");
	}

	cleanup:
	curl_global_cleanup();
	curl_easy_cleanup(handle);
	curl_slist_free_all(header);
	free(response.text);
	
	return rep;
}

char *upload_group(unsigned short max, time_t old_date, char dir_path[MAX_PATH], char username[MAX_USERNAME])
{
	cJSON *log_json = cJSON_CreateObject();
	cJSON *replay_block = cJSON_AddArrayToObject(log_json, "Replays");
	DIR *rep_dir = opendir(dir_path);
	struct dirent *entry;
	short rep_count = 0;
	while((entry = readdir(rep_dir)) && rep_count < max)
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			char rep_path[MAX_PATH];
			strcpy_s(rep_path, sizeof(rep_path), dir_path);
			strcat_s(rep_path, sizeof(rep_path), "\\");
			strcat_s(rep_path, sizeof(rep_path), entry->d_name);

			struct stat info;
			stat(rep_path, &info);
			if(info.st_mtime > old_date)
			{
				Replay rep = upload_replay(rep_path, username);
				cJSON *replay_obj = get_replay_json(rep);
				cJSON_AddItemToArray(replay_block, replay_obj);
			}
			fclose(replay);
			rep_count++;
		}
	}
	char *log_raw = NULL;
	log_raw = cJSON_Print(log_json);
	if(log_raw == NULL)
		perror("\n[JSON] Failure in printing object\n");
	
	cJSON_Delete(log_json);
	closedir(rep_dir);

	return log_raw;
}
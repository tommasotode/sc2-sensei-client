#include "include/core.h"

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
	strcat_s(rep_name, sizeof(rep_name), replay_name);
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
	strcpy_s(current.id, sizeof(current.id), replay_id->valuestring);
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

char *upload_group(unsigned short max, time_t old_date, char dir_path[MAX_PATH], char username[MAX_USERNAME])
{
	cJSON *json = cJSON_CreateObject();
	cJSON *replay_block = cJSON_AddArrayToObject(json, "Replays");
	DIR *rep_dir = opendir(dir_path);
	struct dirent *entry;
	unsigned short rep_count = 0;
	while((entry = readdir(rep_dir)) && rep_count < max)
	{
		if(strcmp(entry->d_name, ".") != 0 || strcmp(entry->d_name, "..") != 0)
		{
			char rep_path[MAX_PATH];
			strcpy_s(rep_path, sizeof(rep_path), dir_path);
			strcat_s(rep_path, sizeof(rep_path), "\\");
			strcat_s(rep_path, sizeof(rep_path), entry->d_name);

			struct stat info;
			FILE *replay = fopen(rep_path, "rb");
			fstat(fileno(replay), &info);
			if(info.st_mtime > old_date)
			{
				Replay rep = upload_replay(replay, entry->d_name, username);
				cJSON *replay_obj = get_replay_json(rep);
				cJSON_AddItemToArray(replay_block, replay_obj);
			}
			fclose(replay);
			rep_count++;
		}
	}
	closedir(rep_dir);

	char *log = NULL;
	log = cJSON_Print(json);
	if(log == NULL)
		perror("\n[JSON] Failure in printing object\n");
	cJSON_Delete(json);

	return log;
}
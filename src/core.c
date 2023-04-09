#include "include/core.h"

// TODO: Make a function to create them if they're not found
__declspec(dllexport) check check_files(char settings_path[MAX_PATH], char dir_path[MAX_PATH])
{
	check state = SUCCESS;
	FILE *setcheck;
	DIR *dircheck;
	if(!(setcheck = fopen(settings_path, "rb")))
	{
		perror("[!] Unable to open settings file");
		state = FAILURE;
	}
	if(!(dircheck = opendir(dir_path)))
	{
		perror("[!] Unable to open replays directory");
		state = FAILURE;
	}
	fclose(setcheck);
	closedir(dircheck);
	
	return state;
}

__declspec(dllexport) void wrt_file_date(char dat_path[MAX_PATH], time_t date)
{
	FILE *data_file = fopen(dat_path, "wb");
	fwrite(&date, sizeof(time_t), 1, data_file);
	fclose(data_file);
}

__declspec(dllexport) time_t get_file_date(char dat_path[MAX_PATH])
{
	time_t output;
	FILE *datf = fopen(dat_path, "rb");
	fread(&output, sizeof(time_t), 1, datf);
	fclose(datf);
	
	return output;
}

__declspec(dllexport) time_t get_dir_date(char dir_path[MAX_PATH])
{
	DIR *repl_dir = opendir(dir_path);
	readdir(repl_dir);
	time_t output = repl_dir->dd_dta.time_write;
	closedir(repl_dir);
	
	return output;
}

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

Replay upload_replay(FILE *replay, char name[MAX_PATH])
{

	struct stat info;
	fstat(fileno(replay), &info);
	
	struct MemoryStruct response;
	response.memory = malloc(1);
	response.size = 0;

	char replay_name[MAX_PATH + 10] = "name: ";
	char player_name[40] = "username: ";
	char player_id[] = "gengiskhan";
	strcat_s(replay_name, sizeof(replay_name), name);
	strcat_s(player_name, sizeof(player_name), player_id);


	Replay current;
	strcpy_s(current.name, MAX_PATH, name);
	strcpy_s(current.id, ID_LEN, "");
	strcpy_s(current.parse_rslt, MAX_RESPONSE, "Connection error");
	current.play_date = info.st_mtime;
	current.upload_date = time(NULL);
	current.connection = FAILURE;
	

	struct curl_slist *list = NULL;
	list = curl_slist_append(list, replay_name);
	list = curl_slist_append(list, player_name);

	// TODO: Add mimetype to the header

	CURL *handle = curl_easy_init();
	if(!handle)
	{
		perror("\n[!] Unable to initialize curl, aborting upload\n");
		current.connection = FAILURE;
		goto cleanup;
	}
	// Setting URL to upload to and how
	curl_easy_setopt(handle, CURLOPT_URL, "localhost:5000/auto_upload");
	curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);

	// Setting where to read the file from and how
	curl_easy_setopt(handle, CURLOPT_READDATA, replay);
	curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)info.st_size);

	// Get server output and write it to a string
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&response);

	// Set header
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, list);
	
	// Verbose mode
	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);

	if(curl_easy_perform(handle) != CURLE_OK)
	{
		printf("\n[!] Unable to upload [%s]\n", name);
		current.connection = FAILURE;
		goto cleanup;
	}
	printf("[SERVER] %lu bytes retrieved\n", (unsigned long)response.size);
	printf("[SERVER] %s\n", response.memory);

	cJSON *response_json = cJSON_ParseWithLength(response.memory, response.size);
	const cJSON *parse = cJSON_GetObjectItem(response_json, "parse");
	const cJSON *replay_id = cJSON_GetObjectItem(response_json, "replay_id");
	cJSON_Delete(response_json);

	// Fill replay log
	strcpy_s(current.name, MAX_PATH, name);
	strcpy_s(current.id, ID_LEN, replay_id->valuestring);
	current.play_date = info.st_mtime;
	current.upload_date = time(NULL);
	current.connection = SUCCESS;
	if(response.size == 0)
		strcpy_s(current.parse_rslt, 17, "Connection error");
	else
		strcpy_s(current.parse_rslt, MAX_RESPONSE, parse->valuestring);
	
	cleanup:

	curl_slist_free_all(list);
	curl_easy_cleanup(handle);
	free(response.memory);
	return current;
}

cJSON *get_replay_json(Replay rep)
{
	cJSON *replay_object = cJSON_CreateObject();
	cJSON_AddStringToObject(replay_object, "name", rep.name);
	cJSON_AddStringToObject(replay_object, "id", rep.id);
	cJSON_AddNumberToObject(replay_object, "play_date", rep.play_date);
	cJSON_AddNumberToObject(replay_object, "upload_date", rep.upload_date);
	cJSON_AddBoolToObject(replay_object, "connection", (cJSON_bool)rep.connection);
	cJSON_AddStringToObject(replay_object, "parse", rep.parse_rslt);

	return replay_object;
}

char *upload_group(unsigned short max, time_t old_date, char dir_path[MAX_PATH])
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
			strcpy_s(rep_path, MAX_PATH, dir_path);
			strcat_s(rep_path, MAX_PATH, "\\");
			strcat_s(rep_path, MAX_PATH, entry->d_name);

			struct stat info;
			FILE *replay = fopen(rep_path, "rb");
			fstat(fileno(replay), &info);
			if(info.st_mtime > old_date)
			{
				Replay rep = upload_replay(replay, entry->d_name);
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

__declspec(dllexport) char *upload_last_n(unsigned short n, char dir_path[MAX_PATH])
{
	if(n > 20 || n < 1)
	{
		printf("Invalid replay number (VALID 1 - 20)\n");
		return NULL;
	}
	char *log = upload_group(n, 0, dir_path);
	
	return log;
}

__declspec(dllexport) char *upload_all_new(time_t old_date, char dir_path[MAX_PATH])
{
	if(old_date <= 0)
		printf("[WARNING] Old date <= 0\n");
	char *log = upload_group(MAX_UP, old_date, dir_path);
	
	return log;
}

__declspec(dllexport) check debug_mode()
{
	short mode;
	printf("DEBUG MODE\n\n");
	printf("0 - Quit\n1 - Upload single replay\n");
	scanf("%hd", &mode);	
	if(mode == 1)
	{
		char path[MAX_PATH];
		char name[MAX_PATH] = "debug_replay";
		FILE *rep;
		printf("Insert the replay path\n");
		scanf("%s", path);
		if(!(rep = fopen(path, "rb")))
		{
			perror("Failed to open the replay");
			fclose(rep);
			return FAILURE;
		}
		Replay result = upload_replay(rep, name);
		if(result.connection == FAILURE)
		{
			perror("Failed to upload the replay");
			fclose(rep);
			return FAILURE;
		}
		printf("%s", result.parse_rslt);
		fclose(rep);
	}	
	
	return SUCCESS;
}
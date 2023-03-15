#include "include/core.h"

//	Use this to check the files before using other functions
__declspec(dllexport) check check_files(char dat_rt[MAX_PATH], char dir_rt[MAX_PATH])
{
	check state = SUCCESS;
	FILE *datcheck;
	DIR *dircheck;
	if(!(datcheck = fopen(dat_rt, "rb")))
	{
		perror("[!] Unable to open data file");
		state = FAILURE;
	}
	if(!(dircheck = opendir(dir_rt)))
	{
		perror("[!] Unable to open replays directory");
		state = FAILURE;
	}
	fclose(datcheck);
	closedir(dircheck);
	
	return state;
}

__declspec(dllexport) void wrt_file_date(char dat_rt[MAX_PATH], time_t date)
{
	FILE *data_file = fopen(dat_rt, "wb");
	fwrite(&date, sizeof(time_t), 1, data_file);
	fclose(data_file);
}

__declspec(dllexport) time_t get_file_date(char dat_rt[MAX_PATH])
{
	time_t output;
	FILE *datf = fopen(dat_rt, "rb");
	fread(&output, sizeof(time_t), 1, datf);
	fclose(datf);
	
	return output;
}

__declspec(dllexport) time_t get_dir_date(char dir_rt[MAX_PATH])
{
	DIR *repl_dir = opendir(dir_rt);
	readdir(repl_dir);
	time_t output = repl_dir -> dd_dta.time_write;
	closedir(repl_dir);
	
	return output;
}

size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *readhere = (FILE *)userdata;
	size_t retcode = fread(ptr, size, nmemb, readhere);
	curl_off_t nread = (curl_off_t)retcode;
	printf("[CURL] %lli bytes read\n", nread);
	
	return retcode;
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	// contents is realsize long, so we must get realsize and resize
	// then, we copy contents to our reallocated memory block
	
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
}


// TODO: Use the parse result to send it for the logging

// Warning: Do not use this function alone (doesn't handle the files)
Replay upload_replay(FILE *replay, char name[MAX_PATH])
{
	Replay current;
	struct stat info;
	fstat(fileno(replay), &info);

	char replay_name[270] = "Name: ";
	strcat_s(replay_name, sizeof(replay_name), name);
	struct curl_slist *list = NULL;
	list = curl_slist_append(list, replay_name);

	// TODO: Add mimetype to the header

	CURL *handle = curl_easy_init();
	if(!handle)
	{
		perror("[!] Unable to initialize curl, aborting upload");
		current.state = FAILURE;
		return current;
	}
	// Setting URL to upload to and how
	curl_easy_setopt(handle, CURLOPT_URL, "localhost:5000/auto_upload");
	curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);

	// Setting where to read the file from and how
	curl_easy_setopt(handle, CURLOPT_READDATA, replay);
	curl_easy_setopt(handle, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)info.st_size);

	// Adding replay name to headers
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, list);

	//(Verbose mode)
	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);

	if(curl_easy_perform(handle) != CURLE_OK)
	{
		printf("\n[!] Unable to upload [%s]\n", name);
		current.state = FAILURE;
	}
	curl_slist_free_all(list);
	curl_easy_cleanup(handle);

	//	Filling replay object
	strcpy_s(current.name, MAX_PATH, name);
	current.play_date = info.st_mtime;
	current.upload_date = time(NULL);
	current.state = SUCCESS;
	
	return current;
}


// TODO: Try to implement multithreading for uploads
__declspec(dllexport) char *upload_all_new(time_t old_dt, char dir_rt[MAX_PATH])
{
	char *output = NULL;
	short rep_count = 0;
	struct stat info;
	struct dirent *entry;
	DIR *rep_dir = opendir(dir_rt);
	check result = SUCCESS;

	cJSON *json = cJSON_CreateObject();
	cJSON *replay_block = cJSON_AddArrayToObject(json, "Replays");

	//maybe a for could be better
	while((entry = readdir(rep_dir)) 	&& rep_count < 10)
	// strcmp(entry->d_name,".") != 0 		&& strcmp(entry->d_name, "..") != 0)
	{
		char rep_path[MAX_PATH];
		strcpy_s(rep_path, MAX_PATH, dir_rt);
		strcat_s(rep_path, MAX_PATH, "\\");
		strcat_s(rep_path, MAX_PATH, entry->d_name);
		
		FILE *replay = fopen(rep_path, "rb");
		fstat(fileno(replay), &info);
		if(info.st_mtime > old_dt)
		{
			Replay rep = upload_replay(replay, entry->d_name);
			if(rep.state == FAILURE)
				result = FAILURE;
			
			cJSON *replay_object = cJSON_CreateObject();

			cJSON_AddStringToObject(replay_object, "name", rep.name);
			cJSON_AddNumberToObject(replay_object, "play_date", rep.play_date);
			cJSON_AddNumberToObject(replay_object, "upload_date", rep.upload_date);
			cJSON_AddBoolToObject(replay_object, "result", (cJSON_bool)rep.state);

			cJSON_AddItemToArray(replay_block, replay_object);

		}
		fclose(replay);
		rep_count+=1;
	}
	closedir(rep_dir);
	cJSON_AddBoolToObject(json, "success", (cJSON_bool)result);
	
	output = cJSON_Print(json);
	if(output == NULL)
		perror("Failure in printing object");
	cJSON_Delete(json);
	
	return output;
}



//						Debug mode								//
__declspec(dllexport) check debug_mode()
{
	short mode;
	printf("DEBUG MODE\n\n");
	printf("0 - Quit\n1 - Upload single replay\n");
	scanf("%hd", &mode);	
	if (mode == 1)
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
		if(upload_replay(rep, name).state == FAILURE)
		{
			perror("Failed to upload the replay");
			return FAILURE;
		}
		fclose(rep);
	}	
	
	return SUCCESS;
}
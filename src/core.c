#include "include/core.h"

//	Use this to check the files before using other functions
check check_files(char dat_rt[MAX_PATH], char dir_rt[MAX_PATH])
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

void wrt_file_date(char dat_rt[MAX_PATH], time_t date)
{
	FILE *data_file = fopen(dat_rt, "wb");
	fwrite(&date, sizeof(time_t), 1, data_file);
	fclose(data_file);
}

time_t get_file_date(char dat_rt[MAX_PATH])
{
	time_t output;
	FILE *datf = fopen(dat_rt, "rb");
	fread(&output, sizeof(time_t), 1, datf);
	fclose(datf);
	
	return output;
}

time_t get_dir_date(char dir_rt[MAX_PATH])
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
	curl_off_t nread;
	size_t retcode = fread(ptr, size, nmemb, readhere);
	nread = (curl_off_t)retcode;
	printf("[CURL] %lli bytes read\n", nread);
	
	return retcode;
}

char *hello()
{
	return "Hello world";
}

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

	CURL *handle = curl_easy_init();
	if(!handle)
	{
		perror("[!] Unable to initialize curl, aborting upload");
		current.state = FAILURE;
		return current;
	}
	// Setting URL to upload to and how
	curl_easy_setopt(handle, CURLOPT_URL, "https://sc2sensei.top/auto_upload");
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
	time_t raw_time;
	time(&raw_time);
	struct tm * local_time = localtime(&raw_time);
	strftime(current.upload_date, MAX_PATH, "%x - %I:%M%p", local_time);
	strcpy_s(current.name, MAX_PATH, name);
	current.play_date = info.st_mtime;
	current.state = SUCCESS;
	return current;
}

//	TODO
char **upload_all_new(time_t old_dt, char dir_rt[MAX_PATH])
{
	char *result[MAX_UP];
	short rep_count = 0;
	struct stat info;
	struct dirent *entry;
	DIR *rep_dir = opendir(dir_rt);
	while((entry = readdir(rep_dir)) 	&& 
	strcmp(entry->d_name,".") != 0 		&& strcmp(entry->d_name, "..") != 0)
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
			
			// to be replaced with json
			
			// char out[200];
			// sprintf_s(out, sizeof(rep.play_date), "%lli\n", rep.play_date);
			// strcat_s(out, sizeof(out), rep.upload_date);
			// strcat_s(out, sizeof(out), "\n");
			// strcat_s(out, sizeof(out), rep.name);
			// sprintf_s(out, sizeof(rep.play_date), "\n%d", rep.state);
			// result[rep_count] = out;
			// Check wether size is of the first or the second one.
		}
		fclose(replay);
		rep_count+=1;
	}
	closedir(rep_dir);
	
	return result;
}


void clean(Replay *old_list)
{
	free(old_list);
}


//						Debug mode								//
check debug_mode()
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
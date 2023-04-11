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
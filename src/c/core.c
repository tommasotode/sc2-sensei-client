#include "include/core.h"

// TODO: Make a function to create them if they're not found
__declspec(dllexport) check check_files(char replays_path[MAX_PATH])
{
	check state = SUCCESS;
	DIR *dircheck;
	if(!(dircheck = opendir(replays_path)))
	{
		perror("[!] Unable to open replays directory");
		state = FAILURE;
	}
	closedir(dircheck);
	
	return state;
}

__declspec(dllexport) time_t get_dir_date(char dir_path[MAX_PATH])
{
	DIR *repl_dir = opendir(dir_path);
	readdir(repl_dir);
	time_t output = repl_dir->dd_dta.time_write;
	closedir(repl_dir);
	
	return output;
}

__declspec(dllexport) char *upload_all_new(time_t old_date, char dir_path[MAX_PATH], char username[MAX_USERNAME])
{
	char *log = upload_group(MAX_UP, old_date, dir_path, username);
	
	return log;
}

__declspec(dllexport) char *upload_last_n(unsigned short n, char dir_path[MAX_PATH], char username[MAX_USERNAME])
{
	if(n > MAX_UP || n < 1)
	{
		printf("Invalid replay number (VALID 1 - %d)\n", MAX_UP);
		return NULL;
	}
	char *log = upload_group(n, 0, dir_path, username);
	
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
		char name[MAX_PATH] = "debug_replay.SC2Replay";
		FILE *rep;
		printf("Insert the replay path\n");
		scanf("%s", path);
		if(!(rep = fopen(path, "rb")))
		{
			perror("Failed to open the replay");
			fclose(rep);
			return FAILURE;
		}
		char username[MAX_USERNAME];
		strcpy_s(username, sizeof(username), "gengiskhan");
		Replay result = upload_replay(rep, name, username);
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


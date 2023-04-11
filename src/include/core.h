#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <curl/curl.h>
#include <cJSON.h>

#define SUCCESS 1
#define FAILURE 0
#define MAX_UP 20
#define ID_LEN 25
#define MAX_RESPONSE 80
// TODO: Check max name length in SC2
#define MAX_NAME 40

#define UPLOAD_ENDPOINT "localhost:5000/auto_upload"
#define USERNAME_ENDPOINT "localhost:5000/check_username"


typedef unsigned char check;
typedef struct Replay
{
	char name[MAX_PATH];
	char id[ID_LEN];
	time_t play_date;
	time_t upload_date;
	check connection;
	char parse_rslt[100];
} Replay;

struct MemoryStruct
{
	char *memory;
	size_t size;
};

Replay upload_replay(FILE *replay, char name[MAX_PATH]);
short check_username(char username[MAX_NAME]);

// __declspec(dllexport) check check_files(char dat_rt[MAX_PATH], char dir_rt[MAX_PATH]);
// __declspec(dllexport) void wrt_file_date(char dat_rt[MAX_PATH], time_t date);
// __declspec(dllexport) time_t get_file_date(char dat_rt[MAX_PATH]);
// __declspec(dllexport) time_t get_dir_date(char dir_rt[MAX_PATH]);
// __declspec(dllexport) char *upload_last_n(unsigned short number, char dir_path[MAX_PATH]);
// __declspec(dllexport) char *upload_all_new(time_t old_date, char dir_path[MAX_PATH]);
// __declspec(dllexport) check debug_mode();

#endif
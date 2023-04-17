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
// TODO: Check max name length in SC2
#define MAX_USERNAME 40
// TODO: Check max parse result length
#define MAX_PARSE 100

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
	char parse_rslt[MAX_PARSE];
} Replay;

struct MemoryStruct
{
	char *memory;
	size_t size;
};

Replay upload_replay(FILE *replay, char replay_name[MAX_PATH], char username[MAX_USERNAME]);
char *upload_group(unsigned short max, time_t old_date, char dir_path[MAX_PATH], char username[MAX_USERNAME]);
size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);
cJSON *get_replay_json(Replay rep);
short check_user(char username[MAX_USERNAME]);

#endif
#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <cJSON.h>

#define MAX_UP 20
#define REPLAY_ID_LEN 32

// TODO: Check max name length in SC2
#define MAX_USERNAME 40
#define MAX_PASSWORD 64
#define USER_ID_LEN 32

// TODO: Check max parse result length
#define MAX_PARSE 100

// TODO: Find a realistic number for this
#define MAX_REPLAY_NAME 60

#define UPLOAD_ENDPOINT "localhost:5000/auto_upload"
#define USERNAME_ENDPOINT "localhost:5000/check_user"

typedef struct Replay
{
	char path[MAX_PATH];
	char username[MAX_USERNAME];
} Replay;

typedef struct ReplayLog
{
	char path[MAX_PATH];
	char id[REPLAY_ID_LEN];
	time_t play_date;
	time_t upload_date;
	bool connection;
	char parse_result[MAX_PARSE];
} ReplayLog;

typedef struct Response
{
	char *text;
	size_t size;
} Response;

size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

ReplayLog upload_replay(char path[MAX_PATH], char username[MAX_USERNAME]);
ReplayLog parse_response(Response data, char rep_path[MAX_PATH]);
char *upload(short max, time_t old_date, char dir_path[MAX_PATH], char username[MAX_USERNAME]);
char *check_valid_user(char username[MAX_USERNAME], char password[MAX_PASSWORD]);

cJSON *get_log_json(ReplayLog log);
char *get_raw_log(cJSON *log);
bool is_utf8(char *string);

#endif
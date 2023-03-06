#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <curl/curl.h>

#define SUCCESS 1
#define FAILURE 0

#define MAX_UP 10

typedef unsigned char check;
typedef struct replay
{
	time_t play_date;
	char *upload_date;
	char name[MAX_PATH];
	char replay_path[MAX_PATH];
	check state;
} Replay;

check check_files(char dat_rt[MAX_PATH], char dir_rt[MAX_PATH]);
void wrt_file_date(char dat_rt[MAX_PATH], time_t date);
time_t get_file_date(char dat_rt[MAX_PATH]);
time_t get_dir_date(char dir_rt[MAX_PATH]);
Replay *upload_all_new(time_t old_dt, char dir_rt[MAX_PATH]);
check debug_mode();

#endif
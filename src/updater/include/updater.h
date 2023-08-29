#ifndef AUTO_UPDATER_H
#define AUTO_UPDATER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <zip.h>

#define RELEASE_URL "https://github.com/TommasoTodescato/Sc2SenseiClient/releases/latest/download/release"
#define UPDATE_URL "https://github.com/TommasoTodescato/Sc2SenseiClient/releases/latest/download/test.zip"

// completely arbitrary length
#define URL_LEN 256

bool download_update(char save_path[MAX_PATH], char url[URL_LEN]);
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream);
bool extract(char archive_path[MAX_PATH], char destination_path[MAX_PATH]);
#endif
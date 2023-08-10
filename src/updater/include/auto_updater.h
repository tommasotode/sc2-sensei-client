#ifndef AUTO_UPDATER_H
#define AUTO_UPDATER_H

#include <stdio.h>
#include <curl/curl.h>
#include <stdbool.h>

#define UPDATE_ENDPOINT "https://github.com/TommasoTodescato/Sc2SenseiClient/releases/latest/download/test.zip"

bool download();
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream);

#endif
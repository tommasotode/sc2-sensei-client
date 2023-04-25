#ifndef AUTO_UPDATER_H
#define AUTO_UPDATER_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <curl/curl.h>

#define UPDATE_ENDPOINT "localhost:5000/client_update"

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream);


#endif
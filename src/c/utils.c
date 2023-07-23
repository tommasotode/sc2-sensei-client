#include "include/core.h"

size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	// read from a file (size specified outside)

	FILE *readhere = (FILE *)userdata;
	size_t retcode = fread(ptr, size, nmemb, readhere);
	curl_off_t nread = (curl_off_t)retcode;
	printf("[CURL] %lli bytes read\n", nread);

	return retcode;
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
	// create memory block and put contents in it

	size_t realsize = size * nmemb;
	Response *mem = (Response *)userp;
	char *ptr = realloc(mem->text, mem->size + realsize + 1);
	if (!ptr)
	{
		perror("Not enough memory");
		return 0;
	}
	mem->text = ptr;
	memcpy(&(mem->text[mem->size]), contents, realsize);
	mem->size = mem->size + realsize;
	mem->text[mem->size] = 0;

	return realsize;
}

ReplayLog parse_response(Response data, char rep_path[MAX_PATH])
{
	struct stat info;
	stat(rep_path, &info);
	ReplayLog log = {.upload_date = time(NULL), .play_date = info.st_mtime, .connection = true};
	strcpy_s(log.path, sizeof(log.path), rep_path);

	if (data.size == 0)
	{
		log.connection = false;
		free(data.text);
		return log;
	}

	cJSON *log_json = cJSON_ParseWithLength(data.text, data.size);
	const cJSON *parse = cJSON_GetObjectItem(log_json, "parse");
	const cJSON *rep_id = cJSON_GetObjectItem(log_json, "replay_id");

	strcpy_s(log.parse_result, sizeof(log.parse_result), cJSON_GetStringValue(parse));
	strcpy_s(log.id, sizeof(log.id), cJSON_GetStringValue(rep_id));

	// TODO: Maybe in the future I will need to change this (I don't think so)
	if (!is_utf8(log.parse_result) || !is_utf8(log.id))
	{
		strcpy_s(log.parse_result, sizeof(log.parse_result), "Decoding failure");
		strcpy_s(log.id, sizeof(log.id), "");
	}

	cJSON_Delete(log_json);
	free(data.text);

	return log;
}

cJSON *get_log_json(ReplayLog log)
{
	cJSON *log_json = cJSON_CreateObject();
	cJSON_AddStringToObject(log_json, "path", log.path);
	cJSON_AddStringToObject(log_json, "id", log.id);
	cJSON_AddNumberToObject(log_json, "play_date", log.play_date);
	cJSON_AddNumberToObject(log_json, "upload_date", log.upload_date);
	cJSON_AddBoolToObject(log_json, "connection", (cJSON_bool)log.connection);
	cJSON_AddStringToObject(log_json, "parse", log.parse_result);

	return log_json;
}

bool is_utf8(char *string)
{
	if(!string)
		return false;

	const unsigned char * bytes = (const unsigned char *)string;
	while(*bytes)
	{
		// ASCII
		if((
			bytes[0] == 0x09 ||
			bytes[0] == 0x0A ||
			bytes[0] == 0x0D ||
			(0x20 <= bytes[0] && bytes[0] <= 0x7E)
		))
		{
			bytes += 1;
			continue;
		}

		if((
			(0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
			(0x80 <= bytes[1] && bytes[1] <= 0xBF)
		))
		{
			bytes += 2;
			continue;
		}

		if((
				bytes[0] == 0xE0 &&
				(0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			) ||
			(
				((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
					bytes[0] == 0xEE ||
					bytes[0] == 0xEF) &&
				(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF)
			) ||
			(
				bytes[0] == 0xED &&
				(0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF)
		))
		{
			bytes += 3;
			continue;
		}

		if((
				bytes[0] == 0xF0 &&
				(0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
				(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			) ||
			(
				(0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
				(0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
				(0x80 <= bytes[3] && bytes[3] <= 0xBF)
			) ||
			(
				bytes[0] == 0xF4 &&
				(0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
				(0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
				(0x80 <= bytes[3] && bytes[3] <= 0xBF)
		))
		{
			bytes += 4;
			continue;
		}
		return false;
	}
	
	return true;
}
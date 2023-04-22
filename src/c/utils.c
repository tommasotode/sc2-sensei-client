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
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(!ptr)
	{
		perror("Not enough memory");
		return 0;
	}
	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size = mem->size + realsize;
	mem->memory[mem->size] = 0;

	return realsize;
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

check is_utf8(char *string)
{
	if(!string)
		return FAILURE;

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
		return FAILURE;
	}
	
	return SUCCESS;
}
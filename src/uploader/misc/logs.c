#include <core.h>

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

char *get_raw_log(cJSON *log)
{
	char *raw_log = NULL;
	raw_log = cJSON_Print(log);
	if (raw_log == NULL)
		perror("\n[JSON] Failure in printing object\n");

	return raw_log;
}
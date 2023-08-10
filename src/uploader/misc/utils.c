#include <core.h>

size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *readhere = (FILE *)userdata;
	size_t retcode = fread(ptr, size, nmemb, readhere);
	curl_off_t nread = (curl_off_t)retcode;
	printf("[CURL] %lli bytes read\n", nread);

	return retcode;
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
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
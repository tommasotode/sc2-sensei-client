#include <updater.h>

int on_entry(const char *filename, void *arg)
{
	static int i = 0;
	int n = *(int *)arg;
	printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

	return 0;
}

// TODO: Check for errors
// TODO: Add verification for files
bool extract(char archive_path[MAX_PATH], char destination_path[MAX_PATH])
{
	int arg = 2;
	zip_extract(archive_path, destination_path, on_entry, &arg);
	return true;
}
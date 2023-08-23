#include <updater.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

// LAUNCHER PROTOTYPE

#include <stdio.h>

// the build script will create a directory with the zip and the release in

int main()
{
	bool update = false;
	
	struct stat st;
	if(stat("tmp", &st) == -1)
	{
    	mkdir("tmp");
	}

	download_release();

	char old_path[MAX_PATH] = "release";
	char new_path[MAX_PATH] = "tmp/release";
	char old_content[100], new_content[100];
	FILE *old_release = fopen(old_path, "rb");
	FILE *new_release = fopen(new_path, "rb");

	fread(old_content, 1, sizeof(old_content), old_release);
	fread(new_content, 1, sizeof(new_content), new_release);

	if (strcmp(old_content, new_content) != 0)
		update = true;

	if(update)
	{
		char archive[MAX_PATH] = "tmp/temp.zip";
		char dir[MAX_PATH] = "tmp";
		
		download_update(archive);

		extract(archive, dir);
		remove(archive);

		//int files_count = 10;
		//for(int i=0; i < files_count; i++)
		//{
		//	remove("filename");

		//	move("from ./tmp/filename to ./filename");
		//}

		//remove("tmp");
	}

	char *args[]={"./client.exe",NULL};
	execvp(args[0],args);

	printf("\nFailure running client");

	return 0;
}
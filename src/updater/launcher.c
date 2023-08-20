#include <updater.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

// LAUNCHER PROTOTYPE

#define update true

int main()
{
	if(update)
	{
		struct stat st;
		if(stat("tmp", &st) == -1)
		{
    		mkdir("tmp");
		}

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


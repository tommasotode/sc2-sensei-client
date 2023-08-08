#include <auto_updater.h>


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

		char link[] = "https://github.com/TommasoTodescato/SC2SenseiClient/releases/latest/download/main.zip";
		char dir[] = "tmp";

		download(link, dir);

		char archive[] = "tmp/main.zip";

		extract(archive, dir);
		remove(archive);

		int files_count = 10;
		for(int i=0; i < files_count; i++)
		{
			remove("filename");

			move("from ./tmp/filename to ./filename");
		}

		remove("tmp");

	}
	char *args[]={"./client.exe",NULL};
	execvp(args[0],args);

	printf("Failure");

	return 0;
}


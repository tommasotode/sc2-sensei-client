#include "include/zip.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int on_entry(const char *filename, void *arg)
{
	static int i = 0;
	int n = *(int *)arg;
	printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

	return 0;
}




int main()
{
	int arg = 2;
	printf("\n%d", zip_extract("C:\\repos\\Sc2SenseiClient\\src\\updater\\prova.zip", "C:\\repos\\Sc2SenseiClient\\src\\updater\\temp", on_entry, &arg));

	unsigned char update = 1;
	pid_t pid;
	char *args[] = {NULL};
	// execv
	if(update)
	{
		pid = fork();
		if(pid == 0)
		{

		}
	}


}
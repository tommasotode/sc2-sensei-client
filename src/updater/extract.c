#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <zip.h>

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
	
	char dir[] = "C:\\repos\\Sc2SenseiClient\\src\\updater\\temp";
	char archive[] = "C:\\repos\\Sc2SenseiClient\\src\\updater\\temp\\prova.zip";

	printf("\n%d", zip_extract(archive, dir, on_entry, &arg));
	remove(archive);
}
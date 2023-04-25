#include "include/zip.h"
#include <stdio.h>

int on_extract_entry(const char *filename, void *arg)
{
	static int i = 0;
	int n = *(int *)arg;
	printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

	return 0;
}


int main()
{
	int arg = 215;
	printf("\n%d", zip_extract("C:\\repos\\Sc2SenseiClient\\src\\updater\\prova.zip", "C:\\repos\\Sc2SenseiClient\\src\\updater\\temp", on_extract_entry, &arg));
}

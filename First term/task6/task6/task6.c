#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys/stat.h>
#include "mman.h"
#include <string.h>

int cmpInc(const void* p1, const void* p2)
{
	return strcmp(*(char**)p1, *(char**)p2);
}

int cmpDec(const void* p1, const void* p2)
{
	return (-1) * strcmp(*(char**)p1, *(char**)p2);
}

int main(int argc, char* argv[])
{
	int fin, fout;
	struct stat info;
	char* map;
	char** lines;
	int countOfLines = 1;
	int key = 0;

	if (argc != 4)
		exit(-1);

	if ((fin = open(argv[1], O_RDWR)) == -1)
		exit(-1);

	if ((fout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IWRITE)) == -1)
		exit(-1);

	if (!strcmp(argv[3], "increase"))
		key = 1;
	else if (!strcmp(argv[3], "decrease"))
		key = 2;
	else
		exit(-1);

	fstat(fin, &info);
	if ((map = mmap(0, info.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fin, 0)) == MAP_FAILED)
		exit(-1);

	for (int i = 0; i < info.st_size; i++)
	{
		if (map[i] == '\n')
			countOfLines++;
	}
	lines = (char**)malloc(sizeof(char*) * countOfLines);
	lines[0] = &map[0];
	int j = 1;
	for (int i = 0; i < info.st_size; i++)
	{
		if (map[i] == '\n')
		{
			lines[j] = &map[i + 1];
			j++;
		}

	}
	if (key == 1)
		qsort(lines, countOfLines, sizeof(char*), cmpInc);
	if (key == 2)
		qsort(lines, countOfLines, sizeof(char*), cmpDec);

	char eol = '\n';
	for (int i = 0; i < countOfLines; i++)
	{
		int len = 0;
		while (*(lines[i] + len) != '\r' && *(lines[i] + len) != '\n' && *(lines[i] + len) != '\0')
		{
			if (&(*(lines[i] + len)) == &map[info.st_size - 1])
			{
				len++;
				break;
			}
			len++;
		}
		write(fout, lines[i], len);
		write(fout, &eol, 1);
	}
	free(lines);
	munmap(map, info.st_size);
	close(fin, fout);
	return 0;
}
/*
	MIT License

	Copyright (c) 2014-2022 Błażej Szczygieł

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "Wrapper.h"

#include <string.h>

REGPARM void sub_41B250(MAYBE_THIS uint32_t arg1, void *arg2);

#ifdef NFS_CPP
	#define sub_41B250(a, b) \
		sub_41B250(this, a, b)
#endif

typedef struct
{
	char name[10];
	int16_t car;
	int32_t time;
	int32_t mode;
} StfEntry;
typedef struct
{
	StfEntry bestLap;
	StfEntry laps[3][10];
} Stf;

REALIGN REGPARM FILE *fopen_wrap(const char *fileName, const char *p)
{
	char *tmpFileName = convertFilePath(fileName, true);
	FILE *f = fopen(tmpFileName, p);
	free(tmpFileName);
	return f;
}

static void readEntry(FILE *f, StfEntry *stfEntry)
{
	uint32_t i;
	fgets(stfEntry->name, sizeof stfEntry->name, f);
	for (i = 0; i < sizeof stfEntry->name; ++i)
		if (stfEntry->name[i] == '\t' || stfEntry->name[i] == '\n' || stfEntry->name[i] == '\r')
			stfEntry->name[i] = '\0';
	fscanf(f, "%hi\n%d\n%d\n", &stfEntry->car, &stfEntry->time, &stfEntry->mode);
}

REALIGN REGPARM void fetchTrackRecords(MAYBE_THIS uint32_t trackNo, BOOL clear)
{
	char buffer[MAX_PATH];
	Stf stf;
	FILE *f;

	memset(&stf, 0, sizeof stf);

	if (!clear)
	{
		//Get the track records relative file path, +20 means that we want a text file (ssf), not a binary file (stf)
		sub_41B250(trackNo + 20, buffer);
		if ((f = fopen_wrap(buffer, "r")))
		{
			//Skip unneeded data
			fgets(buffer, 80, f);
			fscanf(f, "%d\n", (int32_t *)buffer);
			fgets(buffer, 80, f);

			uint32_t i, j;
			readEntry(f, &stf.bestLap); //Best lap
			for (j = 0; j < 3; ++j)
			{
				fgets(buffer, 80, f); //Skip unneeded line
				for (i = 0; i < 10; ++i)
					readEntry(f, &stf.laps[j][i]);
			}

			fclose(f);
		}
	}

	//Get the track records relative file path, get a binary file (stf)
	sub_41B250(trackNo, buffer);
	if ((f = fopen_wrap(buffer, "wb")))
	{
		fwrite(&stf, 1, sizeof stf, f);
		fclose(f);
	}
}

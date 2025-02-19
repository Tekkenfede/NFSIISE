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

#ifndef KERNEL32_H
#define KERNEL32_H

#include "Wrapper.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <SDL2/SDL_thread.h>
	#include <SDL2/SDL_mutex.h>

	#include <dirent.h>

	typedef uint32_t (STDCALL *THREAD_START_ROUTINE)(void *);

	enum {GENERIC_WRITE = 0x40000000, GENERIC_READ = 0x80000000};

	typedef struct
	{
		uint32_t length;
		uint32_t memoryLoad;
		uint32_t totalPhys;
		uint32_t availPhys;
		uint32_t totalPageFile;
		uint32_t availPageFile;
		uint32_t totalVirtual;
		uint32_t availVirtual;
	} MEMORYSTATUS;

	typedef struct
	{
		union
		{
			uint32_t oemId;
			struct
			{
				uint16_t processorArchitecture;
				uint16_t reserved;
			};
		};
		uint32_t pageSize;
		void *minimumApplicationAddress;
		void *maximumApplicationAddress;
		uint32_t activeProcessorMask;
		uint32_t numberOfProcessors;
		uint32_t processorType;
		uint32_t allocationGranularity;
		uint16_t processorLevel;
		uint16_t processorRevision;
	} SYSTEM_INFO;

	typedef struct
	{
		void *reserved1, *reserved2, *reserved3;
		SDL_mutex *mutex;
		void *reserved4, *reserved5;
	} CRITICAL_SECTION;

	typedef struct
	{
		uint32_t Internal;
		uint32_t InternalHigh;
		union
		{
			struct
			{
				uint32_t Offset;
				uint32_t OffsetHigh;
			};
			void *Pointer;
		};
		void *hEvent;
	} OVERLAPPED;

	typedef struct
	{
		uint32_t ReadIntervalTimeout;
		uint32_t ReadTotalTimeoutMultiplier;
		uint32_t ReadTotalTimeoutConstant;
		uint32_t WriteTotalTimeoutMultiplier;
		uint32_t WriteTotalTimeoutConstant;
	} COMMTIMEOUTS;

	typedef struct
	{
		uint32_t lowDateTime;
		uint32_t highDateTime;
	} FILETIME;
	typedef struct
	{
		uint32_t fileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		uint32_t fileSizeHigh;
		uint32_t fileSizeLow;
		uint32_t reserved[2];
		char fileName[MAX_PATH];
		char alternateFileName[14];
	} WIN32_FIND_DATA;

	typedef void SECURITY_ATTRIBUTES;
	typedef void COMMPROP;
	typedef void DCB;

	typedef enum {HandleThread = 1, HandleFile, HandleFileMapping, HandleEvent} HandleType;
	typedef struct
	{
		HandleType handleType;
#ifdef NFS_CPP
		int (*function)(void *);
		void *arg;
#else
		int (*threadParameter)();
#endif
		SDL_sem *sem;
	} Thread;
	typedef struct
	{
		HandleType handleType;
		int fd;
		/* ASync */
		BOOL async, pending;
		uint32_t toRead, readSoFar;
		uint8_t *asyncReadBuffer;
		OVERLAPPED *readOverlapped;
		SDL_mutex *mutex;
		uint32_t us_timeout;
	} File;
	typedef struct
	{
		HandleType handleType;
		int fd;
	} FileMapping;
	typedef struct
	{
		HandleType handleType;
		BOOL manualReset, is_set;
	} Event;

	typedef struct
	{
		DIR *dir;
		char *filter;
	} FindFile;
#endif

#endif // KERNEL32_H

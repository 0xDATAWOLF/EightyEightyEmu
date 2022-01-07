
/**
 * Welcome, traveler, to main.c, where you will find all sorts of Italian cuisine.
 * Our current special today is spaghetti!
 * 
 * Anyway, this is broken up into two parts. Part one, application runtime, contains
 * the bulk of the runtime code. Part two, platform abstraction and entry points, contains
 * the "main" entry points ifdef'd based on the platform the code is compiled on. Additionally,
 * any OS-specific code is abstracted for the application, such as file IO.
 * 
 * THE MIGHTY TODO:
 * 
 * 1. Complete CPU emulation
 * 2. Bitmap drawing (basic)
 * 3. Bitmap drawing (GPU accelerated)
 * 4. Sound??
 * 5. IMGUI Debugging & Visualization
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "main.h"
#include "primitives.h"
#include "helpers.h"

#define ROM_INVADERS_E "./rom/invaders.e"
#define ROM_INVADERS_F "./rom/invaders.f"
#define ROM_INVADERS_G "./rom/invaders.g"
#define ROM_INVADERS_H "./rom/invaders.h"

typedef i32 fn_ptr_read_file(const char*, void*, i32);
fn_ptr_read_file* PlatformReadfile = NULL;

typedef i32 fn_ptr_file_size(const char*);
fn_ptr_file_size* PlatformFilesize = NULL;

typedef struct processor_state
{
	struct registers
	{
		u8 a;
		u8 b;
		u8 c;
		u8 d;
		u8 e;
		u8 h;
		u8 l;
		u16 sp;
		u16 pc;
	} registers;
	struct flags
	{
		u8 s: 1;
		u8 z: 1;
		u8 always_zero_a: 1;
		u8 ac: 1;
		u8 always_zero_b: 1;
		u8 p: 1;
		u8 always_one_a: 1;
		u8 c: 1;
	} flags;

	void* BaseAddress;
	u8* MemoryAddress;
	u8 int_enable;

} processor_state;

void
UnexpectedInstruction(processor_state* CPU)
{
	u8 currentOperation = CPU->MemoryAddress[CPU->registers.pc];
	printf("The emulator has encountered an unexpected instruction 0x%02X.\n", currentOperation);
	assert(!"Quick halt!");
}

i32
EmulationRuntime(processor_state* CPU)
{
	
	u8* opcode = &CPU->MemoryAddress[CPU->registers.pc];
	DisassembleBinary(opcode);
	
	switch (*opcode)
	{
		case 0x00: break;
		
		default: UnexpectedInstruction(CPU); break;
	};

	++CPU->registers.pc;
	return 0;

}

/**
 * Application Main is called right after the platform performs all it's required
 * duties, such as filling out function pointers, allocating memory, and window allocations.
 */
i32
ApplicationMain(void* AppMemory)
{

	assert(AppMemory != NULL);
	assert(PlatformReadfile != NULL);
	assert(PlatformFilesize != NULL);

	i32 InvadersH_Filesize = PlatformFilesize(ROM_INVADERS_H);
	i32 InvadersG_Filesize = PlatformFilesize(ROM_INVADERS_G);
	i32 InvadersF_Filesize = PlatformFilesize(ROM_INVADERS_F);
	i32 InvadersE_Filesize = PlatformFilesize(ROM_INVADERS_E);
	
	/**
	 * We are using the starting pointer of the application memory as the base address $0000.
	 */
	void* MemoryOffsetPtr = AppMemory;
	PlatformReadfile(ROM_INVADERS_H, MemoryOffsetPtr, InvadersH_Filesize);
	PlatformReadfile(ROM_INVADERS_G, (void*)((u8*)MemoryOffsetPtr += InvadersH_Filesize), InvadersG_Filesize);
	PlatformReadfile(ROM_INVADERS_F, (void*)((u8*)MemoryOffsetPtr += InvadersG_Filesize), InvadersF_Filesize);
	PlatformReadfile(ROM_INVADERS_E, (void*)((u8*)MemoryOffsetPtr += InvadersF_Filesize), InvadersE_Filesize);

	/**
	 * This function will output to the console the entire disassembled binary in a human readable
	 * format. The downside is that it will output to the console the entire disassembled binary.
	 * 
	 * It's not entirely useful except for debug purposes. And it won't work on Windows for various
	 * reason that I won't go into in a small comment.
	 * 
	 * TODO: We should pull the loop out of the disassembler and enforce its usage at the top
	 * level so we can do "stepping" within the console.
	 */
	// DisassembleBinary(AppMemory, ROMFilesize);
	processor_state CPU = {0};
	CPU.BaseAddress = AppMemory;

	u8 ApplicationRunning = 1;
	while (ApplicationRunning)
	{
		i32 ProcessorRFlags = EmulationRuntime(&CPU);
		/**
		 * Processor R-Flags, runtime flags, are a product of the CPU emulation.
		 * We can use the output of the EmulationRuntime function to determine
		 * if the CPU is a good state, or in a bad state (but recoverable), or
		 * an unrecoverable state and requires the application runtime to shut down.
		 * 
		 * Currently unused and unimplemented for now.
		 */
	}

	return 0;
}










/**
 * Application entry points are defined here. Since this is designed to work on all platforms,
 * we'll be abstracting out calls to platform calls to function pointers in order to make it
 * more portable. Please take note!
 */
#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <io.h>

i32
WIN32Readfile(const char* Filepath, void* MemoryLoc, i32 BytesReading)
{

	HANDLE fileHandle = CreateFileA(Filepath, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	/**
	 * NOTE: We are enforcing ReadFile to always read the bytes requested,
	 * but it may be necessary to check for EOF and if we have read the
	 * amount of bytes requested. However, for now, we'll just EXPLODE.
	 */
	DWORD BytesRead = 0;
	ReadFile(fileHandle, MemoryLoc, BytesReading, &BytesRead, NULL);
	CloseHandle(fileHandle);

	assert(BytesRead == BytesReading);

	return 0;

}

i32
WIN32Filesize(const char* Filepath)
{

	HANDLE fileHandle = CreateFileA(Filepath, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	assert(fileHandle != INVALID_HANDLE_VALUE);

	LARGE_INTEGER fileSize = {0};
	GetFileSizeEx(fileHandle, &fileSize);
	CloseHandle(fileHandle);

	/**
	 * We aren't expecting a file over 4GB, but in case it is, EXPLODE.
	 * TODO: We should probably take this into consideration at some point.
	 */
	assert(fileSize.QuadPart < 0xFFFFFFFF);
	return fileSize.LowPart;

}

i32 WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR Commandline, i32 CommandShow)
{

	void* ApplicationMemory = malloc(Megabytes(512));
	PlatformFilesize = WIN32Filesize;
	PlatformReadfile = WIN32Readfile;

	return ApplicationMain(ApplicationMemory);
}


#else

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

i32
POSIXReadfile(const char* Filepath, void* MemoryLoc, i32 BytesReading)
{
	i32 rom_fd = open(Filepath, O_RDONLY); 
	if (!rom_fd)
	{
		printf("Rom file was not found, file descriptor invalid.\n");
	}

	i32 read_status = read(rom_fd, MemoryLoc, BytesReading); 

	return 0;
}

i32
POSIXFilesize(const char* Filepath)
{
	off_t fileSize = -1;
	struct stat FileStat;
	if (stat(Filepath, &FileStat) == 0)
	{
		return FileStat.st_size;
	}
	return fileSize;
}

i32
main()
{
	
	void* ApplicationMemory = malloc(Megabytes(512));
	PlatformReadfile = POSIXReadfile;
	PlatformFilesize = POSIXFilesize;

	return ApplicationMain(ApplicationMemory);
}

#endif





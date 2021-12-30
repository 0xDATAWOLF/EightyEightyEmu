#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float r32;
typedef double r64;

/**
 * Byte sizing macros used for memory allocation and memory offsets.
 */
#define Kilobyte(size) (i32)(size*1024)
#define Megabytes(size) (i32)(Kilobyte(size)*1024)
#define Gigabytes(size) (i64)(Megabytes(size)*1024)
#define Terabytes(size) (i64)(Gigabytes(size)*1024)

#define ROM_FILE_PATH "./rom/invaders.h"

/**
 * Dissasmbles ROM binary into their corresponding opcodes and outputs to a file.
 */
i32
DisassembleBinary()
{

	

	return 0;
}


/**
 * Application entry points are defined here. Since this is designed to work on all platforms,
 * we'll be abstracting out calls to platform calls to function pointers in order to make it
 * more portable. Please take note!
 */
#ifdef _WIN32
#include <windows.h>

/**
 * Because, only Microsoft would ever make it this difficult to run a GUI app with a console.
 * https://stackoverflow.com/questions/191842/how-do-i-get-console-output-in-c-with-a-windows-program
 * 
 * These functions are designed specifically for debug output in the console and will be summarily replaced
 * with IMGUI output.
 */
void
AdjustConsoleBuffer(int16_t minLength)
{
    // Set the screen buffer to be big enough to scroll some text
    CONSOLE_SCREEN_BUFFER_INFO conInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
    if (conInfo.dwSize.Y < minLength)
        conInfo.dwSize.Y = minLength;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
}

bool
RedirectConsoleIO()
{
	bool result = true;
	FILE *fp;

	// Redirect STDIN if the console has an input handle
	if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
			result = false;
		else
			setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect STDOUT if the console has an output handle
	if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
			result = false;
		else
			setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect STDERR if the console has an error handle
	if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
		if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
			result = false;
		else
			setvbuf(stderr, NULL, _IONBF, 0);

	return result;
}

bool
ReleaseConsole()
{
	bool result = true;
	FILE *fp;

	// Just to be safe, redirect standard IO to NUL before releasing.

	// Redirect STDIN to NUL
	if (freopen_s(&fp, "NUL:", "r", stdin) != 0)
		result = false;
	else
		setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect STDOUT to NUL
	if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
		result = false;
	else
		setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect STDERR to NUL
	if (freopen_s(&fp, "NUL:", "w", stderr) != 0)
		result = false;
	else
		setvbuf(stderr, NULL, _IONBF, 0);

	// Detach from console
	if (!FreeConsole())
		result = false;

	return result;
}

i32 WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR Commandline, i32 CommandShow)
{

    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
		AdjustConsoleBuffer(1024);
        RedirectConsoleIO();
        printf("\n");

    }

	printf("Hello world.\n");


	return 0;
}


#else

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


i32
POSIXReadfile()
{
	i32 rom_fd = open(ROM_FILE_PATH, O_RDONLY); 
	if (!rom_fd)
	{
		printf("Rom file was not found, file descriptor invalid.\n");
	}

	return 0;
}

off_t
POSIXFileSize(const char* Filepath)
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
	printf("EightyEightyEmu Linux \n");
	
	i32 Filesize = POSIXFileSize(ROM_FILE_PATH);
	if (Filesize == -1 || Filesize == 0)
	{
		printf("ROM file, %s, has not been found.\n", ROM_FILE_PATH);
	}
	else
	{
		printf("ROM file, %s, has been found.\n", ROM_FILE_PATH);
		printf("ROM file size is: %d.\n", Filesize);
	}

	void* ApplicationMemory = malloc(Megabytes(512));
	if (ApplicationMemory == NULL)
	{
		printf("Unable to allocate application memory for runtime.\n");
	}
	else
	{
		printf("Application memory successfully allocated.\n");
	}

	return 0;
}

#endif





#include <stdio.h>
#include <stdint.h>
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

i32 WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR Commandline, i32 CommandShow)
{
	
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON","w",stdout);

	printf("Hello world.\n");


	while(1);
	return 0;
}


#else

int
main()
{
	


	return 0;
}

#endif

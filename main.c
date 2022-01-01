#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



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
#define Kilobytes(size) (i32)(size*1024)
#define Megabytes(size) (i32)(Kilobytes(size)*1024)
#define Gigabytes(size) (i64)(Megabytes(size)*1024)
#define Terabytes(size) (i64)(Gigabytes(size)*1024)

#define ROM_FILE_PATH "./rom/invaders.h"

typedef i32 fn_ptr_read_file(const char*, void*, i32);
fn_ptr_read_file* PlatformReadfile = NULL;

typedef i32 fn_ptr_file_size(const char*);
fn_ptr_file_size* PlatformFilesize = NULL;

/**
 * Dissasmbles ROM binary into human readable instructions and prints them out.
 * Eventually, I will set up a file writing procedure to output this information
 * as a much more "digestable" and "tabular" form. For now, it just dumps to
 * disassembly to console.
 */
i32
DisassembleBinary(void* BinaryData, i32 BinaryByteCount)
{

	i32 opIndex = 0;
	while(opIndex < BinaryByteCount)
	{

		u8 iSize = 1;
		u8* opData = (u8*)BinaryData;
		u8 instr = opData[opIndex];
		switch(instr)
		{
			case 0x00: printf("0x%02X NOP : 1\n", instr); iSize = 1; break;
			case 0x01: printf("0x%02X LXI 0x%02X 0x%02X B,D16 : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0x02: printf("0x%02X STAX B : 1\n", instr); iSize = 1; break;
			case 0x03: printf("0x%02X INX B : 1\n", instr); iSize = 1; break;
			case 0x04: printf("0x%02X INR B : 1\n", instr); iSize = 1; break;
			case 0x05: printf("0x%02X DCR B : 1\n", instr); iSize = 1; break;
			case 0x06: printf("0x%02X MVI B,D8 0x%02X : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0x07: printf("0x%02X RLC : 1\n", instr); iSize = 1; break;
			case 0x08: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0x09: printf("0x%02X DAD B : 1\n", instr); iSize = 1; break;
			case 0x0A: printf("0x%02X LDAX B : 1\n", instr); iSize = 1; break;
			case 0x0B: printf("0x%02X DCX B : 1\n", instr); iSize = 1; break;
			case 0x0C: printf("0x%02X INR C : 1\n", instr); iSize = 1; break;
			case 0x0D: printf("0x%02X DCR C : 1\n", instr); iSize = 1; break;
			case 0x0E: printf("0x%02X MVI %02X C,D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0x0F: printf("0x%02X RRC: 1\n", instr); iSize = 1; break;

			case 0x10: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0x11: printf("0x%02X LXI 0x%02X 0x%02X D,D16 : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0x12: printf("0x%02X STAX D : 1\n", instr); iSize = 1; break;
			case 0x13: printf("0x%02X INX D : 1\n", instr); iSize = 1; break;
			case 0x14: printf("0x%02X INR D : 1\n", instr); iSize = 1; break;
			case 0x15: printf("0x%02X DCR D : 1\n", instr); iSize = 1; break;
			case 0x16: printf("0x%02X MVI 0x%02X D,D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0x17: printf("0x%02X RAL : 1\n", instr); iSize = 1; break;
			case 0x18: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0x19: printf("0x%02X DAD D : 1\n", instr); iSize = 1; break;
			case 0x1A: printf("0x%02X LDAX D : 1\n", instr); iSize = 1; break;
			case 0x1B: printf("0x%02X DCX D : 1\n", instr); iSize = 1; break;
			case 0x1C: printf("0x%02X INR E : 1\n", instr); iSize = 1; break;
			case 0x1D: printf("0x%02X DCR E : 1\n", instr); iSize = 1; break;
			case 0x1E: printf("0x%02X MVI 0x%02X E,D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0x1F: printf("0x%02X RAR : 1\n", instr); iSize = 1; break;


			case 0x20: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0x21: printf("0x%02X LXI 0x%02X 0x%02X H,D16 : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0x22: printf("0x%02X SHLD adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0x23: printf("0x%02X INX H : 1\n", instr); iSize = 1; break;
			case 0x24: printf("0x%02X INR H : 1\n", instr); iSize = 1; break;
			case 0x25: printf("0x%02X DCR H : 1\n", instr); iSize = 1; break;
			case 0x26: printf("0x%02X MVI 0x%02X H,D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0x27: printf("0x%02X DAA : 1\n", instr); iSize = 1; break;
			case 0x28: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0x29: printf("0x%02X DAD H : 1\n", instr); iSize = 1; break;
			case 0x2A: printf("0x%02X LHLD adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0x2B: printf("0x%02X DCX H : 1\n", instr); iSize = 1; break;
			case 0x2C: printf("0x%02X INR L : 1\n", instr); iSize = 1; break;
			case 0x2D: printf("0x%02X DCR L : 1\n", instr); iSize = 1; break;
			case 0x2E: printf("0x%02X MVI 0x%02X L,D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0x2F: printf("0x%02X CMA : 1\n", instr); iSize = 1; break;
			
			case 0x30: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0x31: printf("0x%02X LXI 0x%02X 0x%02X SP,D16 : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0x32: printf("0x%02X STA adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0x33: printf("0x%02X INX SP : 1\n", instr); iSize = 1; break;
			case 0x34: printf("0x%02X INR M : 1\n", instr); iSize = 1; break;
			case 0x35: printf("0x%02X DCR M : 1\n", instr); iSize = 1; break;
			case 0x36: printf("0x%02X MVI 0x%02X M,D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0x37: printf("0x%02X STC : 1\n", instr); iSize = 1; break;
			case 0x38: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0x39: printf("0x%02X DAD SP : 1\n", instr); iSize = 1; break;
			case 0x3A: printf("0x%02X LDA adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0x3B: printf("0x%02X DCX SP : 1\n", instr); iSize = 1; break;
			case 0x3C: printf("0x%02X INR A : 1\n", instr); iSize = 1; break;
			case 0x3D: printf("0x%02X DCR A : 1\n", instr); iSize = 1; break;
			case 0x3E: printf("0x%02X MVI 0x%02X A,D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0x3F: printf("0x%02X CMC : 1\n", instr); iSize = 1; break;

			case 0x40: printf("0x%02X MOV B,B : 1\n", instr); iSize = 1; break;
			case 0x41: printf("0x%02X MOV B,C : 1\n", instr); iSize = 1; break;
			case 0x42: printf("0x%02X MOV B,D : 1\n", instr); iSize = 1; break;
			case 0x43: printf("0x%02X MOV B,E : 1\n", instr); iSize = 1; break;
			case 0x44: printf("0x%02X MOV B,H : 1\n", instr); iSize = 1; break;
			case 0x45: printf("0x%02X MOV B,L : 1\n", instr); iSize = 1; break;
			case 0x46: printf("0x%02X MOV B,M : 1\n", instr); iSize = 1; break;
			case 0x47: printf("0x%02X MOV B,A : 1\n", instr); iSize = 1; break;
			case 0x48: printf("0x%02X MOV C,B : 1\n", instr); iSize = 1; break;
			case 0x49: printf("0x%02X MOV C,C : 1\n", instr); iSize = 1; break;
			case 0x4A: printf("0x%02X MOV C,D : 1\n", instr); iSize = 1; break;
			case 0x4B: printf("0x%02X MOV C,E : 1\n", instr); iSize = 1; break;
			case 0x4C: printf("0x%02X MOV C,H : 1\n", instr); iSize = 1; break;
			case 0x4D: printf("0x%02X MOV C,L : 1\n", instr); iSize = 1; break;
			case 0x4E: printf("0x%02X MOV C,M : 1\n", instr); iSize = 1; break;
			case 0x4F: printf("0x%02X MOV C,A : 1\n", instr); iSize = 1; break;

			case 0x50: printf("0x%02X MOV D,B : 1\n", instr); iSize = 1; break;
			case 0x51: printf("0x%02X MOV D,C : 1\n", instr); iSize = 1; break;
			case 0x52: printf("0x%02X MOV D,D : 1\n", instr); iSize = 1; break;
			case 0x53: printf("0x%02X MOV D,E : 1\n", instr); iSize = 1; break;
			case 0x54: printf("0x%02X MOV D,H : 1\n", instr); iSize = 1; break;
			case 0x55: printf("0x%02X MOV D,L : 1\n", instr); iSize = 1; break;
			case 0x56: printf("0x%02X MOV D,M : 1\n", instr); iSize = 1; break;
			case 0x57: printf("0x%02X MOV D,A : 1\n", instr); iSize = 1; break;
			case 0x58: printf("0x%02X MOV E,B : 1\n", instr); iSize = 1; break;
			case 0x59: printf("0x%02X MOV E,C : 1\n", instr); iSize = 1; break;
			case 0x5A: printf("0x%02X MOV E,D : 1\n", instr); iSize = 1; break;
			case 0x5B: printf("0x%02X MOV E,E : 1\n", instr); iSize = 1; break;
			case 0x5C: printf("0x%02X MOV E,H : 1\n", instr); iSize = 1; break;
			case 0x5D: printf("0x%02X MOV E,L : 1\n", instr); iSize = 1; break;
			case 0x5E: printf("0x%02X MOV E,M : 1\n", instr); iSize = 1; break;
			case 0x5F: printf("0x%02X MOV E,A : 1\n", instr); iSize = 1; break;

			case 0x60: printf("0x%02X MOV H,B : 1\n", instr); iSize = 1; break;
			case 0x61: printf("0x%02X MOV H,C : 1\n", instr); iSize = 1; break;
			case 0x62: printf("0x%02X MOV H,D : 1\n", instr); iSize = 1; break;
			case 0x63: printf("0x%02X MOV H,E : 1\n", instr); iSize = 1; break;
			case 0x64: printf("0x%02X MOV H,H : 1\n", instr); iSize = 1; break;
			case 0x65: printf("0x%02X MOV H,L : 1\n", instr); iSize = 1; break;
			case 0x66: printf("0x%02X MOV H,M : 1\n", instr); iSize = 1; break;
			case 0x67: printf("0x%02X MOV H,A : 1\n", instr); iSize = 1; break;
			case 0x68: printf("0x%02X MOV L,B : 1\n", instr); iSize = 1; break;
			case 0x69: printf("0x%02X MOV L,C : 1\n", instr); iSize = 1; break;
			case 0x6A: printf("0x%02X MOV L,D : 1\n", instr); iSize = 1; break;
			case 0x6B: printf("0x%02X MOV L,E : 1\n", instr); iSize = 1; break;
			case 0x6C: printf("0x%02X MOV L,H : 1\n", instr); iSize = 1; break;
			case 0x6D: printf("0x%02X MOV L,L : 1\n", instr); iSize = 1; break;
			case 0x6E: printf("0x%02X MOV L,M : 1\n", instr); iSize = 1; break;
			case 0x6F: printf("0x%02X MOV L,A : 1\n", instr); iSize = 1; break;

			case 0x70: printf("0x%02X MOV M,B : 1\n", instr); iSize = 1; break;
			case 0x71: printf("0x%02X MOV M,C : 1\n", instr); iSize = 1; break;
			case 0x72: printf("0x%02X MOV M,D : 1\n", instr); iSize = 1; break;
			case 0x73: printf("0x%02X MOV M,E : 1\n", instr); iSize = 1; break;
			case 0x74: printf("0x%02X MOV M,H : 1\n", instr); iSize = 1; break;
			case 0x75: printf("0x%02X MOV M,L : 1\n", instr); iSize = 1; break;
			case 0x76: printf("0x%02X HLT : 1\n", instr); iSize = 1; break;
			case 0x77: printf("0x%02X MOV M,A : 1\n", instr); iSize = 1; break;
			case 0x78: printf("0x%02X MOV A,B : 1\n", instr); iSize = 1; break;
			case 0x79: printf("0x%02X MOV A,C : 1\n", instr); iSize = 1; break;
			case 0x7A: printf("0x%02X MOV A,D : 1\n", instr); iSize = 1; break;
			case 0x7B: printf("0x%02X MOV A,E : 1\n", instr); iSize = 1; break;
			case 0x7C: printf("0x%02X MOV A,H : 1\n", instr); iSize = 1; break;
			case 0x7D: printf("0x%02X MOV A,L : 1\n", instr); iSize = 1; break;
			case 0x7E: printf("0x%02X MOV A,M : 1\n", instr); iSize = 1; break;
			case 0x7F: printf("0x%02X MOV A,A : 1\n", instr); iSize = 1; break;
			
			case 0x80: printf("0x%02X ADD B : 1\n", instr); iSize = 1; break;
			case 0x81: printf("0x%02X ADD C : 1\n", instr); iSize = 1; break;
			case 0x82: printf("0x%02X ADD D : 1\n", instr); iSize = 1; break;
			case 0x83: printf("0x%02X ADD E : 1\n", instr); iSize = 1; break;
			case 0x84: printf("0x%02X ADD H : 1\n", instr); iSize = 1; break;
			case 0x85: printf("0x%02X ADD L : 1\n", instr); iSize = 1; break;
			case 0x86: printf("0x%02X ADD M : 1\n", instr); iSize = 1; break;
			case 0x87: printf("0x%02X ADD A : 1\n", instr); iSize = 1; break;
			case 0x88: printf("0x%02X ADC B : 1\n", instr); iSize = 1; break;
			case 0x89: printf("0x%02X ADC C : 1\n", instr); iSize = 1; break;
			case 0x8A: printf("0x%02X ADC D : 1\n", instr); iSize = 1; break;
			case 0x8B: printf("0x%02X ADC E : 1\n", instr); iSize = 1; break;
			case 0x8C: printf("0x%02X ADC H : 1\n", instr); iSize = 1; break;
			case 0x8D: printf("0x%02X ADC L : 1\n", instr); iSize = 1; break;
			case 0x8E: printf("0x%02X ADC M : 1\n", instr); iSize = 1; break;
			case 0x8F: printf("0x%02X ADC A : 1\n", instr); iSize = 1; break;
			
			case 0x90: printf("0x%02X SUB B : 1\n", instr); iSize = 1; break;
			case 0x91: printf("0x%02X SUB C : 1\n", instr); iSize = 1; break;
			case 0x92: printf("0x%02X SUB D : 1\n", instr); iSize = 1; break;
			case 0x93: printf("0x%02X SUB E : 1\n", instr); iSize = 1; break;
			case 0x94: printf("0x%02X SUB H : 1\n", instr); iSize = 1; break;
			case 0x95: printf("0x%02X SUB L : 1\n", instr); iSize = 1; break;
			case 0x96: printf("0x%02X SUB M : 1\n", instr); iSize = 1; break;
			case 0x97: printf("0x%02X SUB A : 1\n", instr); iSize = 1; break;
			case 0x98: printf("0x%02X SBB B : 1\n", instr); iSize = 1; break;
			case 0x99: printf("0x%02X SBB C : 1\n", instr); iSize = 1; break;
			case 0x9A: printf("0x%02X SBB D : 1\n", instr); iSize = 1; break;
			case 0x9B: printf("0x%02X SBB E : 1\n", instr); iSize = 1; break;
			case 0x9C: printf("0x%02X SBB H : 1\n", instr); iSize = 1; break;
			case 0x9D: printf("0x%02X SBB L : 1\n", instr); iSize = 1; break;
			case 0x9E: printf("0x%02X SBB M : 1\n", instr); iSize = 1; break;
			case 0x9F: printf("0x%02X SBB A : 1\n", instr); iSize = 1; break;
			
			case 0xA0: printf("0x%02X ANA B : 1\n", instr); iSize = 1; break;
			case 0xA1: printf("0x%02X ANA C : 1\n", instr); iSize = 1; break;
			case 0xA2: printf("0x%02X ANA D : 1\n", instr); iSize = 1; break;
			case 0xA3: printf("0x%02X ANA E : 1\n", instr); iSize = 1; break;
			case 0xA4: printf("0x%02X ANA H : 1\n", instr); iSize = 1; break;
			case 0xA5: printf("0x%02X ANA L : 1\n", instr); iSize = 1; break;
			case 0xA6: printf("0x%02X ANA M : 1\n", instr); iSize = 1; break;
			case 0xA7: printf("0x%02X ANA A : 1\n", instr); iSize = 1; break;
			case 0xA8: printf("0x%02X XRA B : 1\n", instr); iSize = 1; break;
			case 0xA9: printf("0x%02X XRA C : 1\n", instr); iSize = 1; break;
			case 0xAA: printf("0x%02X XRA D : 1\n", instr); iSize = 1; break;
			case 0xAB: printf("0x%02X XRA E : 1\n", instr); iSize = 1; break;
			case 0xAC: printf("0x%02X XRA H : 1\n", instr); iSize = 1; break;
			case 0xAD: printf("0x%02X XRA L : 1\n", instr); iSize = 1; break;
			case 0xAE: printf("0x%02X XRA M : 1\n", instr); iSize = 1; break;
			case 0xAF: printf("0x%02X XRA A : 1\n", instr); iSize = 1; break;

			case 0xB0: printf("0x%02X ORA B : 1\n", instr); iSize = 1; break;
			case 0xB1: printf("0x%02X ORA C : 1\n", instr); iSize = 1; break;
			case 0xB2: printf("0x%02X ORA D : 1\n", instr); iSize = 1; break;
			case 0xB3: printf("0x%02X ORA E : 1\n", instr); iSize = 1; break;
			case 0xB4: printf("0x%02X ORA H : 1\n", instr); iSize = 1; break;
			case 0xB5: printf("0x%02X ORA L : 1\n", instr); iSize = 1; break;
			case 0xB6: printf("0x%02X ORA M : 1\n", instr); iSize = 1; break;
			case 0xB7: printf("0x%02X ORA A : 1\n", instr); iSize = 1; break;
			case 0xB8: printf("0x%02X CMP B : 1\n", instr); iSize = 1; break;
			case 0xB9: printf("0x%02X CMP C : 1\n", instr); iSize = 1; break;
			case 0xBA: printf("0x%02X CMP D : 1\n", instr); iSize = 1; break;
			case 0xBB: printf("0x%02X CMP E : 1\n", instr); iSize = 1; break;
			case 0xBC: printf("0x%02X CMP H : 1\n", instr); iSize = 1; break;
			case 0xBD: printf("0x%02X CMP L : 1\n", instr); iSize = 1; break;
			case 0xBE: printf("0x%02X CMP M : 1\n", instr); iSize = 1; break;
			case 0xBF: printf("0x%02X CMP A : 1\n", instr); iSize = 1; break;

			case 0xC0: printf("0x%02X RNZ : 1\n", instr); iSize = 1; break;
			case 0xC1: printf("0x%02X POP B : 1\n", instr); iSize = 1; break;
			case 0xC2: printf("0x%02X JNZ adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xC3: printf("0x%02X JMP adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xC4: printf("0x%02X CNZ adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xC5: printf("0x%02X PUSH B : 1\n", instr); iSize = 1; break;
			case 0xC6: printf("0x%02X ADI 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xC7: printf("0x%02X RST 0 : 1\n", instr); iSize = 1; break;
			case 0xC8: printf("0x%02X RZ : 1\n", instr); iSize = 1; break;
			case 0xC9: printf("0x%02X RET : 1\n", instr); iSize = 1; break;
			case 0xCA: printf("0x%02X JZ adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xCB: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0xCC: printf("0x%02X CZ adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xCD: printf("0x%02X CALL adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xCE: printf("0x%02X ACI 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xCF: printf("0x%02X RST 1 : 1\n", instr); iSize = 1; break;
			
			case 0xD0: printf("0x%02X RNC : 1\n", instr); iSize = 1; break;
			case 0xD1: printf("0x%02X POP D : 1\n", instr); iSize = 1; break;
			case 0xD2: printf("0x%02X JNC adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xD3: printf("0x%02X OUT 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xD4: printf("0x%02X CNC adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xD5: printf("0x%02X PUSH D : 1\n", instr); iSize = 1; break;
			case 0xD6: printf("0x%02X SUI 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xD7: printf("0x%02X RST 2 : 1\n", instr); iSize = 1; break;
			case 0xD8: printf("0x%02X RC : 1\n", instr); iSize = 1; break;
			case 0xD9: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0xDA: printf("0x%02X JC adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xDB: printf("0x%02X IN 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xDC: printf("0x%02X CC adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xDD: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0xDE: printf("0x%02X SBI 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xDF: printf("0x%02X RST 3 : 1\n", instr); iSize = 1; break;

			case 0xE0: printf("0x%02X RPO : 1\n", instr); iSize = 1; break;
			case 0xE1: printf("0x%02X POP H : 1\n", instr); iSize = 1; break;
			case 0xE2: printf("0x%02X JPO adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xE3: printf("0x%02X XTHL : 1\n", instr); iSize = 1; break;
			case 0xE4: printf("0x%02X CPO adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xE5: printf("0x%02X PUSH H : 1\n", instr); iSize = 1; break;
			case 0xE6: printf("0x%02X ANI 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xE7: printf("0x%02X RST 4 : 1\n", instr); iSize = 1; break;
			case 0xE8: printf("0x%02X RPE : 1\n", instr); iSize = 1; break;
			case 0xE9: printf("0x%02X PCHL : 1\n", instr); iSize = 1; break;
			case 0xEA: printf("0x%02X JPE adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xEB: printf("0x%02X XCHG : 1\n", instr); iSize = 1; break;
			case 0xEC: printf("0x%02X CPE adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xED: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0xEE: printf("0x%02X XRI 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xEF: printf("0x%02X RST 5 : 1\n", instr); iSize = 1; break;
			
			case 0xF0: printf("0x%02X RP : 1\n", instr); iSize = 1; break;
			case 0xF1: printf("0x%02X POP PSW : 1\n", instr); iSize = 1; break;
			case 0xF2: printf("0x%02X JP adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xF3: printf("0x%02X DI : 1\n", instr); iSize = 1; break;
			case 0xF4: printf("0x%02X CP adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xF5: printf("0x%02X PUSH PSW : 1\n", instr); iSize = 1; break;
			case 0xF6: printf("0x%02X ORI 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xF7: printf("0x%02X RST 6 : 1\n", instr); iSize = 1; break;
			case 0xF8: printf("0x%02X RM : 1\n", instr); iSize = 1; break;
			case 0xF9: printf("0x%02X SPHL : 1\n", instr); iSize = 1; break;
			case 0xFA: printf("0x%02X JM adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xFB: printf("0x%02X EI : 1\n", instr); iSize = 1; break;
			case 0xFC: printf("0x%02X CM adr 0x%02X 0x%02X : 3\n", instr,  opData[opIndex+2], opData[opIndex+1]); iSize = 3; break;
			case 0xFD: printf("0x%02X - : 1\n", instr); iSize = 1; break;
			case 0xFE: printf("0x%02X CPI 0x%02X D8 : 2\n", instr, opData[opIndex+1]); iSize = 2; break;
			case 0xFF: printf("0x%02X RST 7 : 1\n", instr); iSize = 1; break;


			default: printf("0x%02X : ?\n", instr); break;
		}

		opIndex += iSize;
	}

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

	i32 ROMFilesize = PlatformFilesize(ROM_FILE_PATH);
	i32 ReadStatus = PlatformReadfile(ROM_FILE_PATH, AppMemory, ROMFilesize);

	DisassembleBinary(AppMemory, ROMFilesize);

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
#include <unistd.h>

i32
POSIXReadfile(const char* Filepath, void* MemoryLoc, i32 BytesReading)
{
	i32 rom_fd = open(ROM_FILE_PATH, O_RDONLY); 
	if (!rom_fd)
	{
		printf("Rom file was not found, file descriptor invalid.\n");
	}

	i32 read_status = read(rom_fd, MemoryLoc, BytesReading); 

	return 0;
}

i32
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
	
	void* ApplicationMemory = malloc(Megabytes(512));
	PlatformReadfile = POSIXReadfile;
	PlatformFilesize = POSIXFileSize;

	return ApplicationMain(ApplicationMemory);
}

#endif





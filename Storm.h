#ifndef __STORM_H__
#define __STORM_H__

#include <windows.h>

#define FASTCALL _fastcall

#ifdef __cplusplus
extern "C" {
#endif

BOOL     WINAPI SFile280(char* buff);
BOOL     WINAPI SFile281(void* hArchive, const char* filename, char** buff, int* len, int unknow, DWORD searchScope, void* lpOverlapped);

int      WINAPI SStrCopy(char *dest, const char *source, size_t size);
size_t   WINAPI SStrLen(const char* string);
int      WINAPI SStrUpper(char *string);
char*  FASTCALL SStrChrR(const char *string, char c);

int      WINAPI SBigDel(HANDLE hBig);
int      WINAPI SBigNew(HANDLE* hBig);
int      WINAPI SBigFromBinary(HANDLE hBig, const char* buff, unsigned int num);
int      WINAPI SBigToBinaryBuffer(HANDLE hBig, char* buff, unsigned int num, unsigned int* wnum);
int      WINAPI SBigPowMod(HANDLE hBigDes, HANDLE hBigSrc, HANDLE hBigPow, HANDLE hBigMod);

#pragma comment(lib, "Storm.lib")

#ifdef __cplusplus
}
#endif

#endif // __STORM_H__

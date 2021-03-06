#include "sha_1.h"
#include "Storm.h"
#include "checkmap.h"

BYTE RSA_e[0x04] = {0x01,0x00,0x01,0x00};
BYTE RSA_n[0x100] = {
0xE9, 0xF8, 0xFE, 0x07, 0xE8, 0xC5, 0xA6, 0xC2, 0x7A, 0x17, 
0x5D, 0xA7, 0x41, 0x64, 0x8E, 0x56, 0x0B, 0x04, 0x4A, 0x11, 
0xA7, 0xDA, 0x9F, 0x02, 0x40, 0x8B, 0x06, 0xFC, 0x23, 0x9E, 
0x73, 0x53, 0x90, 0x4F, 0x83, 0x0D, 0xBB, 0x86, 0x8E, 0x01, 
0x4F, 0xFF, 0x65, 0x08, 0x24, 0x89, 0xE8, 0xE1, 0x85, 0x9B, 
0xAE, 0x47, 0xC4, 0x97, 0xA1, 0xBC, 0x99, 0x47, 0x44, 0xAC, 
0x99, 0x4B, 0x57, 0xED, 0xA2, 0x70, 0x64, 0xD8, 0x69, 0xE9, 
0x06, 0xF7, 0x06, 0x8B, 0xBF, 0x91, 0x88, 0x12, 0xC8, 0x3B, 
0x29, 0x6B, 0xA6, 0x2B, 0x51, 0x55, 0x2D, 0xD2, 0xCE, 0x0A, 
0xD6, 0x25, 0xCD, 0x49, 0x84, 0x14, 0x80, 0xD2, 0xA0, 0x3D, 
0x89, 0x0A, 0xE1, 0x95, 0x55, 0xED, 0x8D, 0x89, 0xEE, 0x7E, 
0x7D, 0x4E, 0x16, 0x30, 0xD7, 0x11, 0x9B, 0xE3, 0x29, 0xE1, 
0x70, 0x39, 0xEB, 0xDE, 0xC5, 0xCC, 0x69, 0x40, 0x1B, 0xFB, 
0xA5, 0x86, 0xCA, 0x9B, 0xBA, 0x6A, 0xF5, 0xA8, 0x0A, 0xA3, 
0xDB, 0x56, 0x3A, 0xFA, 0xD6, 0x34, 0xAC, 0x1F, 0x44, 0x7C, 
0x39, 0xC9, 0x5B, 0x25, 0xEE, 0xC0, 0x25, 0xE6, 0xEE, 0x6F, 
0x52, 0x71, 0x82, 0x07, 0xF7, 0xE9, 0xFB, 0x1E, 0x8A, 0x09, 
0x59, 0x68, 0x5B, 0x40, 0xA7, 0x93, 0x72, 0xA0, 0xD8, 0xC8, 
0x4E, 0x86, 0x83, 0x60, 0x63, 0x4C, 0x6E, 0x5F, 0xDD, 0xC7, 
0x03, 0xDF, 0x88, 0x77, 0xB7, 0xD9, 0xF1, 0xFA, 0x08, 0x57, 
0x9B, 0xD4, 0x1E, 0x1B, 0x85, 0x5A, 0x5B, 0xC1, 0xB2, 0xF8, 
0x9A, 0xA1, 0xFA, 0xAD, 0x0D, 0xE7, 0x36, 0x24, 0xBD, 0x0B, 
0x82, 0x5B, 0xD2, 0xEC, 0x27, 0xD7, 0x24, 0xE5, 0xE9, 0x62, 
0x7E, 0x40, 0x8E, 0x81, 0xFA, 0x83, 0xB2, 0x73, 0x95, 0x23, 
0xCB, 0x79, 0x81, 0x46, 0x81, 0x33, 0x8B, 0xE3, 0x4B, 0xDD, 
0x10, 0x45, 0x95, 0x24, 0x1C, 0xD4
};

int GetMapName(const char* MapPath, char* dest, size_t len)
{
  char* n = SStrChrR(MapPath, '/');
  char* m = SStrChrR(MapPath, '\\');

  if (n < m)
    n = m;

  if (n != NULL)
    return SStrCopy(dest, n+1, len);
  else
    return SStrCopy(dest, MapPath, len);
}

BOOL GetSIGNOffset(const char* Map, int size, int len, int* offset)
{
  *offset = size - len - 4;
  if (size < len + 4)
    return FALSE;
  return (*(DWORD*)&Map[*offset] == 'SIGN');
}

int GetMapSha1(const char* MapName, const char* Map, int offset, char* buff, int unknow)
{
  SHA1Context context;
  char Name[0x400];
  
  SHA1Reset(&context);
  
  if (unknow != 0)
  {
    SHA1Input(&context, Map+unknow, offset - unknow);
    offset = unknow;
  }
  SHA1Input(&context, Map, offset);

  SStrCopy(Name, MapName, 0x400);
  SStrUpper(Name);
  SHA1Input(&context, Name, SStrLen(Name));

  return SHA1Result(&context, buff);
}

void RSACrypto(char* data, int len, char* n, int nlen, char* e, int elen)
{
  HANDLE hBigC, hBigM, hBigN, hBigE;

  SBigNew(&hBigC);
  SBigFromBinary(hBigC, data, len);
  SBigNew(&hBigN);
  SBigFromBinary(hBigN, n, nlen);
  SBigNew(&hBigE);
  SBigFromBinary(hBigE, e, elen);

  SBigNew(&hBigM);
  SBigPowMod(hBigM, hBigC, hBigE, hBigN);

  memset(data, 0, len);
  SBigToBinaryBuffer(hBigM, data, len, &len);

  SBigDel(hBigM);
  SBigDel(hBigE);
  SBigDel(hBigN);
  SBigDel(hBigC);
}

BOOL CheckMapInRSA(const char* MapName, const char* Map, int size, char* n, int nlen, char* e, int elen, int unknow)
{
  char sha1out[SHA1HashSize];
  int offset = 0;
  char* rsaout = _alloca(nlen);
  
  if (!GetSIGNOffset(Map, size, nlen, &offset))
    return FALSE;

  if (shaSuccess != GetMapSha1(MapName, Map, offset, sha1out, 0))
    return FALSE;
  
  memcpy(rsaout, Map+offset+4, nlen);

  RSACrypto(rsaout, nlen, n, nlen, e, elen);

  return (memcmp(rsaout, sha1out, SHA1HashSize) == 0);
}

BOOL CheckMap(const char* MapPath)
{
  char  MapName[MAX_PATH];
  char* Map = NULL;
  int   len = 0;
  BOOL  bSuccess = FALSE;

  if (SFile281(NULL, MapPath, &Map, &len, 0, 1, NULL))
  {
    GetMapName(MapPath, MapName, 0x104);
    bSuccess = CheckMapInRSA(MapName, Map, len, RSA_n, 0x100, RSA_e, 0x4, 0);
    SFile280(Map);
  }

  return bSuccess;
}

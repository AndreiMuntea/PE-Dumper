#ifndef FILE_MAP_H
#define FILE_MAP_H
#include "definitions.h"

typedef struct _FILE_MAP
{
   HANDLE hFile;
   HANDLE hMapping;
   PBYTE  pData;
   DWORD  bcSize;
} FILE_MAP, *PFILE_MAP;

STATUS
FileMapPreInit(
   _Inout_	PFILE_MAP	pFileMap
);

STATUS
FileMapCreate(
   _Out_	PFILE_MAP	pFileMap,
   _In_	PCHAR		   szFileName
);

VOID
FileMapDestroy(
   _Inout_  PFILE_MAP pFileMap
);


#endif //FILE_MAP_H
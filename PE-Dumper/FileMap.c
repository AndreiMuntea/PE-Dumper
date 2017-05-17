#include "FileMap.h"

STATUS
FileMapPreInit(
   _Inout_	PFILE_MAP pFileMap
)
{
   STATUS status;

   status = EXIT_SUCCESS_STATUS;
   if (NULL == pFileMap)
   {
      status = NULL_POINTER;
      goto CleanUp;
   }

   pFileMap->bcSize = 0;
   pFileMap->hFile = INVALID_HANDLE_VALUE;
   pFileMap->hMapping = NULL;
   pFileMap->pData = NULL;

CleanUp:
   return status;
}

STATUS
FileMapCreate(
   _Out_	PFILE_MAP pFileMap,
        	_In_	PCHAR szFileName
)
{
   STATUS status;
   DWORD fileAccess;
   DWORD flProtect;
   DWORD mappingAccess;

   fileAccess = GENERIC_READ;
   flProtect = PAGE_READONLY;
   mappingAccess = FILE_MAP_READ;
   status = EXIT_SUCCESS_STATUS;

   status = FileMapPreInit(pFileMap);
   if (!SUCCESS(status))
   {
      goto CleanUp;
   }

   pFileMap->hFile = CreateFileA(
      szFileName,
      fileAccess,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      NULL
   );
   if (INVALID_HANDLE_VALUE == pFileMap->hFile)
   {
      status = GetLastError();
      goto CleanUp;
   }

   pFileMap->bcSize = GetFileSize(pFileMap->hFile, NULL);

   pFileMap->hMapping = CreateFileMappingA(
      pFileMap->hFile,
      NULL,
      flProtect,
      0,
      0,
      NULL
   );
   if (NULL == pFileMap->hMapping)
   {
      status = GetLastError();
      goto CleanUp;
   }

   pFileMap->pData = MapViewOfFile(
      pFileMap->hMapping,
      mappingAccess,
      0,
      0,
      0
   );

   if (NULL == pFileMap->pData)
   {
      status = GetLastError();
      goto CleanUp;
   }

CleanUp:
   if(!SUCCESS(status))
   {
      FileMapDestroy(pFileMap);
   }
   return status;
}


VOID
FileMapDestroy(
   _Inout_  PFILE_MAP pFileMap
)
{
   if(NULL == pFileMap)
   {
      goto Out;
   }

   if (NULL != pFileMap->pData)
   {
      UnmapViewOfFile(pFileMap->pData);
      pFileMap->pData = NULL;
   }
   if (NULL != pFileMap->hMapping)
   {
      CloseHandle(pFileMap->hMapping);
      pFileMap->hMapping = NULL;
   }
   if (INVALID_HANDLE_VALUE != pFileMap->hFile)
   {
      CloseHandle(pFileMap->hFile);
      pFileMap->hFile = INVALID_HANDLE_VALUE;
   }
   pFileMap->bcSize = 0;

Out:;
}
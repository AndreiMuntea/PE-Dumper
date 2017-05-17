#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include <stdio.h>

#include "definitions.h"
#include "FileMap.h"
#include "ImageHeaders.h"

int main(int argc, char* argv[])
{
   STATUS status;
   LPSTR  pFileName;
   LPSTR outputFileName;
   PFILE_MAP pFileMap;
   FILE* f;
   errno_t err;

   err = 0;
   status = EXIT_SUCCESS_STATUS;
   pFileMap = NULL;
   f = NULL;

   if(argc != 3)
   {
      status = INVALID_ARGUMENTS;
      goto CleanUp;
   }
   pFileName = argv[1];
   outputFileName = argv[2];

   err = fopen_s(&f, outputFileName, "w");
   if(err != 0)
   {
      status = FILE_IO_ERROR;
      goto CleanUp;
   }

   pFileMap = (PFILE_MAP)malloc(sizeof(FILE_MAP));
   if(NULL == pFileMap)
   {
      status = BAD_ALLOCATION;
      goto CleanUp;
   }

   status = FileMapCreate(pFileMap, pFileName);
   if(!SUCCESS(status))
   {
      fprintf(f, "INVALID FILE! GLE=%d\n", status);
      goto CleanUp;
   }

   status = AnalyzeImageHeaders(pFileMap,f);

CleanUp:
   FileMapDestroy(pFileMap);
   free(pFileMap);
   pFileMap = NULL;

   _CrtDumpMemoryLeaks();
   
   if (NULL != f)
   {
      fclose(f);
   }
   
   printf("Exitcode status: 0x%08X\n", status);
   return status;
}

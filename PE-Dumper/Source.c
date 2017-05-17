#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include <stdio.h>

#include "definitions.h"
#include "FileMap.h"
#include "ImageHeaders.h"

int main()
{
   STATUS status;
   LPSTR  pFileName;
   PFILE_MAP pFileMap;
   FILE* f;

   pFileName = "C:\\Users\\munte\\Desktop\\kernel32.dll";
   
   fopen_s(&f, "log.txt", "w");
   //pFileName = "C:\\Users\\munte\\Desktop\\f2150_int86_32.dll";
   //pFileName = "C:\\Users\\munte\\Desktop\\procexp.exe";
   //pFileName = "D:\\Projects\\C\\PE-Dumper\\Debug\\PE-Dumper.exe";

   status = EXIT_SUCCESS_STATUS;
   pFileMap = NULL;

   pFileMap = (PFILE_MAP)malloc(sizeof(FILE_MAP));
   if(NULL == pFileMap)
   {
      status = BAD_ALLOCATION;
      goto CleanUp;
   }

   status = FileMapCreate(pFileMap, pFileName);
   if(!SUCCESS(status))
   {
      goto CleanUp;
   }

   status = AnalyzeImageHeaders(pFileMap,f);

CleanUp:
   FileMapDestroy(pFileMap);
   free(pFileMap);
   pFileMap = NULL;

   _CrtDumpMemoryLeaks();
   fclose(f);
   printf("Exitcode status: 0x%08X\n", status);
   return status;
}

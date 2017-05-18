#include "ImageHeaders.h"
#include <stdio.h>
#include <assert.h>


VOID
PrintMachine(
   _In_     DWORD          subsystem,
   _In_     FILE*          file
)
{
   fprintf(file,"\t>Machine: 0x%04X ", subsystem);
   switch (subsystem)
   {
   case 0x014c:
      {
         fprintf(file, "(x86)\n");
         break;
      }
   case 0x0200:
      {
         fprintf(file, "(Intel Itanium)\n");
         break;
      }
   case 0x8664:
      {
         fprintf(file, "(x64)\n");
         break;
      }
   default:
      {
      fprintf(file, "INVALID MACHINE)\n");
      RaiseException(INVALID_MACHINE, 0, 0, NULL);
      }
   }
}

VOID
PrintCharacteristics(
   _In_     DWORD          characteristics,
   _In_     FILE*          file
)
{
   fprintf(file,"\t>Characteristics: 0x%04X:\n", characteristics);
   if(characteristics & 0x0001)
   {
      fprintf(file, "\t  * IMAGE_FILE_RELOCS_STRIPPED\n");
   }
   if(characteristics & 0x0002)
   {
      fprintf(file, "\t  * IMAGE_FILE_EXECUTABLE_IMAGE\n");
   }
   if (characteristics & 0x0004)
   {
      fprintf(file, "\t  * IMAGE_FILE_LINE_NUMS_STRIPPED\n");
   }
   if (characteristics & 0x0008)
   {
      fprintf(file, "\t  * IMAGE_FILE_LOCAL_SYMS_STRIPPED\n");
   }
   if (characteristics & 0x0010)
   {
      fprintf(file, "\t  * IMAGE_FILE_AGGRESIVE_WS_TRIM\n");
   }
   if (characteristics & 0x0020)
   {
      fprintf(file, "\t  * IMAGE_FILE_LARGE_ADDRESS_AWARE\n");
   }
   if (characteristics & 0x0080)
   {
      fprintf(file, "\t  * IMAGE_FILE_BYTES_REVERSED_LO\n");
   }
   if (characteristics & 0x0100)
   {
      fprintf(file, "\t  * IMAGE_FILE_32BIT_MACHINE\n");
   }   
   if (characteristics & 0x0200)
   {
      fprintf(file, "\t  * IMAGE_FILE_DEBUG_STRIPPED\n");
   }
   if (characteristics & 0x0400)
   {
      fprintf(file, "\t  * IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP\n");
   }
   if (characteristics & 0x0800)
   {
      fprintf(file, "\t  * IMAGE_FILE_NET_RUN_FROM_SWAP\n");
   }
   if (characteristics & 0x1000)
   {
      fprintf(file, "\t  * IMAGE_FILE_SYSTEM\n");
   }
   if (characteristics & 0x2000)
   {
      fprintf(file, "\t  * IMAGE_FILE_DLL\n");
   }
   if (characteristics & 0x4000)
   {
      fprintf(file, "\t  * IMAGE_FILE_UP_SYSTEM_ONLY\n");
   }
   if (characteristics & 0x8000)
   {
      fprintf(file, "\t  * IMAGE_FILE_BYTES_REVERSED_HI\n");
   }
}

VOID
PrintSubsystem(
   _In_     DWORD          subsystem,
   _In_     FILE*          file
)
{
   fprintf(file,"\t>Subsystem: %d ", subsystem);
   switch (subsystem)
   {
   case 0:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_UNKNOWN)\n");
         break;
      }
   case 1:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_NATIVE)\n");
         break;
      }
   case 2:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_WINDOWS_GUI)\n");
         break;
      }
   case 3:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_WINDOWS_CUI)\n");
         break;
      }
   case 5:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_OS2_CUI)\n");
         break;
      }
   case 7:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_POSIX_CUI)\n");
         break;
      }
   case 9:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_WINDOWS_CE_GUI)\n");
         break;
      }
   case 10:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_EFI_APPLICATION)\n");
         break;
      }
   case 11:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER)\n");
         break;
      }
   case 12:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER)\n");
         break;
      }
   case 13:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_EFI_ROM)\n");
         break;
      }
   case 14:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_XBOX)\n");
         break;
      }
   case 16:
      {
         fprintf(file,"(IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION)\n");
         break;
      }
   default:
      {
         fprintf(file, "INVALID SUBSYSTEM\n");
         RaiseException(INVALID_SUBSYSTEM, 0, 0, NULL);
      }
   }
}

DWORD
FaFromRva(
   _In_ PIMAGE_HEADERS	pImageHeaders,
   _In_ DWORD		      rvaAddress
)
{
   DWORD i;
   DWORD offset;
   DWORD res;

   for (i = 0; i < pImageHeaders->pImageNTHeaders->FileHeader.NumberOfSections; ++i)
   {
      if (rvaAddress >= pImageHeaders->pImageSectionHeader[i].VirtualAddress
         && rvaAddress < pImageHeaders->pImageSectionHeader[i].VirtualAddress + pImageHeaders->pImageSectionHeader[i].Misc.VirtualSize
         )
      {
         offset = rvaAddress - pImageHeaders->pImageSectionHeader[i].VirtualAddress;
         res = (pImageHeaders->pImageSectionHeader[i].PointerToRawData) + offset;
         return res;
      }
   }
   return 0;
}

PBYTE
OffsetFromRva(
   _In_ PFILE_MAP	      pFileMap,
   _In_ PIMAGE_HEADERS  pImageHeaders,
   _In_ DWORD		      rvaAddress
)
{
   DWORD faAddress;
   faAddress = FaFromRva(pImageHeaders, rvaAddress);
   if (0 == faAddress)
   {
      return NULL;
   }
   return pFileMap->pData + faAddress;
}

STATUS
ImageHeadersPreInit(
   _Inout_  PIMAGE_HEADERS pImageHeaders
)
{
   STATUS status;

   status = EXIT_SUCCESS_STATUS;
   if (NULL == pImageHeaders)
   {
      status = NULL_POINTER;
      goto CleanUp;
   }

   pImageHeaders->pImageDOSHeader = NULL;
   pImageHeaders->pImageFileHeader = NULL;
   pImageHeaders->pImageNTHeaders = NULL;
   pImageHeaders->pImageOptionalHeader = NULL;
   pImageHeaders->pImageSectionHeader = NULL;

CleanUp:
   return status;
}


STATUS
AnalyzeImageHeaders(
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
)
{
   STATUS status;
   IMAGE_HEADERS imageHeaders;

   status = EXIT_SUCCESS_STATUS;

   status = ImageHeadersPreInit(&imageHeaders);
   if (!SUCCESS(status))
   {
      goto CleanUp;
   }

   if (NULL == pFileMap)
   {
      status = NULL_POINTER;
      goto CleanUp;
   }

   __try
   {
      fprintf(file, "DOS Header:\n");
      status = CheckImageDOSHeader(&imageHeaders, pFileMap, file);
      if (!SUCCESS(status))
      {
         status = INVALID_DOS_HEADER;
         fprintf(file, "INVALID DOS HEADER!\n");
         goto CleanUp;
      }
   }
   __except(EXCEPTION_EXECUTE_FAULT)
   {
      status = INVALID_DOS_HEADER;
      fprintf(file, "INVALID DOS HEADER!\n");
      goto CleanUp;
   }
   fprintf(file, "--------------------------------------------------------------------------------\n\n");

   __try
   {
      fprintf(file, "NT Header:\n");
      status = CheckImageNTHeader(&imageHeaders, pFileMap, file);
      if (!SUCCESS(status))
      {
         status = INVALID_NT_HEADER;
         fprintf(file, "INVALID NT HEADER!\n");
         goto CleanUp;
      }
   }
   __except (EXCEPTION_EXECUTE_FAULT)
   {
      status = INVALID_NT_HEADER;
      fprintf(file, "INVALID NT HEADER!\n");
      goto CleanUp;
   }
   fprintf(file, "--------------------------------------------------------------------------------\n\n");

   __try {
      fprintf(file, "File Header:\n");
      status = CheckImageFileHeader(&imageHeaders, pFileMap, file);
      if (!SUCCESS(status))
      {
         status = INVALID_FILE_HEADER;
         fprintf(file, "INVALID FILE HEADER!\n");
         goto CleanUp;
      }
   }
   __except (EXCEPTION_EXECUTE_FAULT)
   {
      status = INVALID_FILE_HEADER;
      fprintf(file, "INVALID FILE HEADER!\n");
      goto CleanUp;
   }
   fprintf(file, "--------------------------------------------------------------------------------\n\n");

   __try {
      fprintf(file, "Optional Header:\n");
      status = CheckImageOptionalHeader(&imageHeaders, pFileMap, file);
      if (!SUCCESS(status))
      {
         status = INVALID_OPTIONAL_HEADER;
         fprintf(file, "INVALID FILE HEADER!\n");
         goto CleanUp;
      }
   }
   __except (EXCEPTION_EXECUTE_FAULT)
   {
      status = INVALID_OPTIONAL_HEADER;
      fprintf(file, "INVALID FILE HEADER!\n");
      goto CleanUp;
   }
   fprintf(file, "--------------------------------------------------------------------------------\n\n");

   __try {
      fprintf(file, "Section Headers:\n");
      status = CheckImageSectionsHeader(&imageHeaders, pFileMap, file);
      if (!SUCCESS(status))
      {
         status = INVALID_SECTIONS_HEADER;
         fprintf(file, "INVALID SECTION HEADER!\n");
         goto CleanUp;
      }
   }
   __except (EXCEPTION_EXECUTE_FAULT)
   {
      status = INVALID_SECTIONS_HEADER;
      fprintf(file, "INVALID SECTION HEADER!\n");
      goto CleanUp;
   }
   fprintf(file, "--------------------------------------------------------------------------------\n\n");

   __try {
      fprintf(file, "Export directory:\n");
      status = CheckImageDirectoryEntryExport(&imageHeaders, pFileMap, file);
      if (!SUCCESS(status))
      {
         status = INVALID_EXPORT_DIRECTORY;
         fprintf(file, "INVALID EXPORT DIRECTORY!\n");
         goto CleanUp;
      }
   }
   __except (EXCEPTION_EXECUTE_FAULT)
   {
      status = INVALID_EXPORT_DIRECTORY;
      fprintf(file, "INVALID EXPORT DIRECTORY!\n");
      goto CleanUp;
   }
   fprintf(file, "--------------------------------------------------------------------------------\n\n");

   __try {
      fprintf(file, "Import directory:\n");
      status = CheckImageDirectoryEntryImport(&imageHeaders, pFileMap, file);
      if (!SUCCESS(status))
      {
         status = INVALID_IMPORT_DIRECTORY;
         fprintf(file, "INVALID IMPORT DIRECTORY!\n");
         goto CleanUp;
      }
   }
   __except (EXCEPTION_EXECUTE_FAULT)
   {
      status = INVALID_IMPORT_DIRECTORY;
      fprintf(file, "INVALID IMPORT DIRECTORY!\n");
      goto CleanUp;
   }
   fprintf(file, "--------------------------------------------------------------------------------\n\n");

CleanUp:
   return status;
}

STATUS
CheckImageDOSHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
)
{
   STATUS status;

   status = EXIT_SUCCESS_STATUS;
   _CRT_UNUSED(file);

   pImageHeaders->pImageDOSHeader = (PIMAGE_DOS_HEADER)pFileMap->pData;


   if ((PBYTE)pImageHeaders->pImageDOSHeader + sizeof(IMAGE_DOS_HEADER) > pFileMap->pData + pFileMap->bcSize)
   {
      fprintf(file, "ERROR INVALID DOS HEADER - OUT OF BOUNDS!!!\n");
      status = OUT_OF_BOUNDS;
      goto CleanUp;
   }

   if (MZ != pImageHeaders->pImageDOSHeader->e_magic)
   {
      fprintf(file, "ERROR INVALID MZ FORMAT!!!\n");
      status = INVALID_MZ_FORMAT;
      goto CleanUp;
   }


CleanUp:
   return status;
}

STATUS
CheckImageNTHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
)
{
   STATUS status;

   status = EXIT_SUCCESS_STATUS;
   _CRT_UNUSED(file);

   pImageHeaders->pImageNTHeaders = (PIMAGE_NT_HEADERS)((PBYTE)pImageHeaders->pImageDOSHeader + pImageHeaders->pImageDOSHeader->e_lfanew);
   if ((PBYTE)pImageHeaders->pImageNTHeaders + sizeof(IMAGE_NT_HEADERS) > pFileMap->pData + pFileMap->bcSize)
   {
      fprintf(file, "ERROR INVALID NT HEADER - OUT OF BOUNDS!!!\n");
      status = OUT_OF_BOUNDS;
      goto CleanUp;
   }

   if (PE != pImageHeaders->pImageNTHeaders->Signature)
   {
      fprintf(file, "ERROR INVALID PE FORMAT!!!\n");
      status = INVALID_PE_FORMAT;
      goto CleanUp;
   }


CleanUp:
   return status;
}

STATUS
CheckImageFileHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
)
{
   STATUS status;

   status = EXIT_SUCCESS_STATUS;

   pImageHeaders->pImageFileHeader = (PIMAGE_FILE_HEADER)(&pImageHeaders->pImageNTHeaders->FileHeader);
   if ((PBYTE)pImageHeaders->pImageFileHeader + sizeof(IMAGE_FILE_HEADER) > pFileMap->pData + pFileMap->bcSize)
   {
      fprintf(file, "ERROR INVALID IMAGE FILE HEADER - OUT OF BOUNDS!!!\n");
      status = OUT_OF_BOUNDS;
      goto CleanUp;
   }

   PrintMachine(pImageHeaders->pImageFileHeader->Machine, file);
   fprintf(file,"\t>Number of sections: %d\n", pImageHeaders->pImageFileHeader->NumberOfSections);
   PrintCharacteristics(pImageHeaders->pImageFileHeader->Characteristics, file);


CleanUp:
   return status;
}

STATUS
CheckImageOptionalHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
)
{
   STATUS status;

   status = EXIT_SUCCESS_STATUS;

   pImageHeaders->pImageOptionalHeader = (PIMAGE_OPTIONAL_HEADER)(&pImageHeaders->pImageNTHeaders->OptionalHeader);
   if ((PBYTE)pImageHeaders->pImageOptionalHeader + sizeof(IMAGE_OPTIONAL_HEADER) > pFileMap->pData + pFileMap->bcSize)
   {
      fprintf(file, "ERROR INVALID OPTIONAL HEADER - OUT OF BOUNDS!!!\n");
      status = OUT_OF_BOUNDS;
      goto CleanUp;
   }

   fprintf(file,"\t>Address of Entry Point: 0x%08X\n", pImageHeaders->pImageOptionalHeader->AddressOfEntryPoint);
   fprintf(file,"\t>Image Base: 0x%08X\n", pImageHeaders->pImageOptionalHeader->ImageBase);
   fprintf(file,"\t>Section Alignment: 0x%08X\n", pImageHeaders->pImageOptionalHeader->SectionAlignment);
   fprintf(file,"\t>File Alignment: 0x%08X\n", pImageHeaders->pImageOptionalHeader->FileAlignment);
   PrintSubsystem(pImageHeaders->pImageOptionalHeader->Subsystem, file);
   fprintf(file,"\t>NumberOfRvaAndSizes: %d\n", pImageHeaders->pImageOptionalHeader->NumberOfRvaAndSizes);

CleanUp:
   return status;
}


STATUS
CheckImageSectionsHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
)
{
   STATUS status;
   DWORD noSections;
   DWORD i;
   DWORD j;
   DWORD offset;
   IMAGE_SECTION_HEADER section;

   j = 0;
   i = 0;
   status = EXIT_SUCCESS_STATUS;
   noSections = pImageHeaders->pImageFileHeader->NumberOfSections;
   offset = sizeof(IMAGE_NT_HEADERS);

   pImageHeaders->pImageSectionHeader = (PIMAGE_SECTION_HEADER)((PBYTE)pImageHeaders->pImageNTHeaders + offset);
   if ((PBYTE)pImageHeaders->pImageSectionHeader + sizeof(IMAGE_SECTION_HEADER) * noSections > pFileMap->pData + pFileMap->bcSize)
   {
      fprintf(file, "ERROR INVALID SECTION HEADER - OUT OF BOUNDS!!!\n");
      status = OUT_OF_BOUNDS;
      goto CleanUp;
   }

   for(i = 0; i < noSections; ++i)
   {
      fprintf(file,"\t>Section %d :\n", i);

      section = pImageHeaders->pImageSectionHeader[i];

      fprintf(file,"\t  * Name: ");
      for(j = 0; j < IMAGE_SIZEOF_SHORT_NAME;++j)
      {
         if('\0' == section.Name[j])
         {
            break;
         }
         fprintf(file,"%c", section.Name[j]);
      }

      fprintf(file,"\n\t  * Virtual Size: %d\n",section.Misc.VirtualSize);
      fprintf(file,"\t  * Virtual Address: 0x%08X\n", section.VirtualAddress);
      fprintf(file,"\t  * Raw Size: %d\n", section.SizeOfRawData);
      fprintf(file,"\t  * Raw Address: 0x%08X\n", section.PointerToRawData);
   }


CleanUp:
   return status;
}

STATUS
CheckImageDirectoryEntryExport(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
)
{
   STATUS status;
   IMAGE_DATA_DIRECTORY imageDirectoryEntryExport;
   PIMAGE_EXPORT_DIRECTORY imageExportDirectory;
   DWORD* addressOfNames;
   WORD* addressOfOrdinals;
   DWORD* addressOfFunctions;
   DWORD* matches;
   LPSTR name;
   DWORD counter;
   DWORD i;

   counter = 0;
   name = NULL;
   matches = NULL;
   addressOfFunctions = NULL;
   addressOfNames = NULL;
   addressOfOrdinals = NULL;
   i = 0;
   status = EXIT_SUCCESS_STATUS;

   imageDirectoryEntryExport = pImageHeaders->pImageOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
   imageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(OffsetFromRva(pFileMap, pImageHeaders, imageDirectoryEntryExport.VirtualAddress));
   
   if(NULL == imageExportDirectory)
   {
      fprintf(file, "\t  * No exports\n");
      goto CleanUp;
   }

   if((PBYTE)imageExportDirectory > pFileMap->pData + pFileMap->bcSize)
   {
      fprintf(file, "ERROR INVALID SECTION - OUT OF BOUNDS!!!\n");
      status = OUT_OF_BOUNDS;
      goto CleanUp;
   }

   addressOfNames = (DWORD*)OffsetFromRva(pFileMap, pImageHeaders, imageExportDirectory->AddressOfNames);
   if(NULL != addressOfNames)
   {
      // it must be valid because we have functions exported by name
      addressOfOrdinals = (WORD*)OffsetFromRva(pFileMap, pImageHeaders, imageExportDirectory->AddressOfNameOrdinals);
      if (NULL == addressOfOrdinals)
      {
         fprintf(file, "ERROR INVALID SECTION FOR ADDRESS OF NAME ORDINALS!!!\n");
         status = INVALID_SECTION;
         goto CleanUp;
      }

   }
   addressOfFunctions = (DWORD*)OffsetFromRva(pFileMap, pImageHeaders, imageExportDirectory->AddressOfFunctions);
   if (NULL == addressOfFunctions)
   {
      fprintf(file, "ERROR INVALID SECTION FOR ADDRESS OF FUNCTIONS!!!\n");
      status = INVALID_SECTION;
      goto CleanUp;
   }

   //an array to flag the visited functions.
   //we will iterate the ones exported by name and after that, the ones exported by ordinals
   matches = (DWORD*)malloc(imageExportDirectory->NumberOfFunctions * sizeof(DWORD));
   if(NULL == matches)
   {
      status = BAD_ALLOCATION;
      goto CleanUp;
   }
   ZeroMemory(matches, imageExportDirectory->NumberOfFunctions * sizeof(DWORD));

   if(imageExportDirectory->NumberOfNames > 0 && addressOfNames == NULL)
   {
      status = INVALID_SECTION;
      goto CleanUp;
   }

   // iterate functions exported by name
   fprintf(file, "\t  * Functions exported by name: \n");
   for(i = 0; i < imageExportDirectory->NumberOfNames; ++i)
   {
      fprintf(file,"\t# %d:\n", counter);
      name = NULL;

      name = (LPSTR)OffsetFromRva(pFileMap, pImageHeaders, addressOfNames[i]);
      if (NULL == name)
      {
         status = INVALID_SECTION;
         goto CleanUp;
      }

      fprintf(file,"\t\t- Name: %s\n", name);
      fprintf(file,"\t\t- Name RVA: 0X%08X\n", addressOfNames[i]);
      fprintf(file,"\t\t- Name Ordinal: %d\n", addressOfOrdinals[i]);
      fprintf(file,"\t\t- Address: 0x%08X\n", addressOfFunctions[addressOfOrdinals[i]]);
      matches[addressOfOrdinals[i]] = 1; //flag
      ++counter;
   }

   // iterate functions exported by ordinal
   fprintf(file, "\t  * Functions exported by ordinal: \n");
   for(i = 0; i < imageExportDirectory->NumberOfFunctions; ++i)
   {
      //function was exported by name
      if(matches[i] != 0 || addressOfFunctions[i] == 0)
      {
         continue;
      }
      fprintf(file, "\t# %d:\n", counter);
      fprintf(file, "\t\t- Ordinal: %d\n", i + imageExportDirectory->Base);
      fprintf(file, "\t\t- Address: 0x%08X\n", addressOfFunctions[i]);
      ++counter;
   }


   
CleanUp:
   free(matches);
   return status;
}


STATUS
CheckImageDirectoryEntryImport(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
)
{
   STATUS status;
   IMAGE_DATA_DIRECTORY imageImportDataDirectory;
   PIMAGE_IMPORT_DESCRIPTOR pImageImportDescriptor;
   PIMAGE_THUNK_DATA pImageThunkData;
   PIMAGE_IMPORT_BY_NAME pImageImportByName;
   DWORD i;
   DWORD j;
   LPSTR moduleName;

   i = 0;
   j = 0;
   moduleName = NULL;
   status = EXIT_SUCCESS_STATUS;
   pImageImportDescriptor = NULL;
   pImageThunkData = NULL;
   pImageImportByName = NULL;
   imageImportDataDirectory = pImageHeaders->pImageOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

   pImageImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)OffsetFromRva(pFileMap, pImageHeaders, imageImportDataDirectory.VirtualAddress);
   if(NULL == pImageImportDescriptor)
   {
      fprintf(file, "\t  * No imports\n");
      goto CleanUp;
   }

   if ((PBYTE)pImageImportDescriptor > pFileMap->pData + pFileMap->bcSize)
   {
      fprintf(file, "ERROR INVALID SECTION - OUT OF BOUNDS!!!\n");
      status = OUT_OF_BOUNDS;
      goto CleanUp;
   }

   for(i = 0; pImageImportDescriptor[i].FirstThunk != 0 ||
              pImageImportDescriptor[i].OriginalFirstThunk != 0; ++i)
   {
      moduleName = (LPSTR)OffsetFromRva(pFileMap, pImageHeaders, pImageImportDescriptor[i].Name);
      if(NULL == moduleName)
      {

         fprintf(file, "ERROR INVALID SECTION FOR MODULE NAME!!!\n");
         status = INVALID_SECTION;
         goto CleanUp;
      }
      fprintf(file, "\t>Module %s:\n", moduleName);

      if (pImageImportDescriptor[i].OriginalFirstThunk != 0)
      {
         pImageThunkData = (PIMAGE_THUNK_DATA)OffsetFromRva(pFileMap, pImageHeaders, pImageImportDescriptor[i].OriginalFirstThunk);
      }
      else
      {
         pImageThunkData = (PIMAGE_THUNK_DATA)OffsetFromRva(pFileMap, pImageHeaders, pImageImportDescriptor[i].FirstThunk);
      }
      if(NULL == pImageThunkData)
      {
         fprintf(file, "ERROR INVALID SECTION FOR IMAGE THUNK DATA!!!\n");
         status = INVALID_SECTION;
         goto CleanUp;
      }

      for(j = 0; pImageThunkData[j].u1.AddressOfData != 0; ++j)
      {
         pImageImportByName = (PIMAGE_IMPORT_BY_NAME)OffsetFromRva(pFileMap, pImageHeaders, pImageThunkData[j].u1.AddressOfData);
         if (NULL == pImageImportByName)
         {
            fprintf(file, "\t  # %d : (NULL) Function imported by ordinal\n", j);
         }
         else
         {
            fprintf(file, "\t  # %d : %s\n", j, pImageImportByName->Name);
         }
      }

   }

CleanUp:
   return status;
}
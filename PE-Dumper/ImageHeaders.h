#ifndef IMAGE_HEADERS_H
#define IMAGE_HEADERS_H

#include "FileMap.h"
#include <stdio.h>

typedef struct _IMAGE_HEADERS
{
   PIMAGE_DOS_HEADER pImageDOSHeader;
   PIMAGE_NT_HEADERS pImageNTHeaders;
   PIMAGE_FILE_HEADER pImageFileHeader;
   PIMAGE_OPTIONAL_HEADER pImageOptionalHeader;
   PIMAGE_SECTION_HEADER pImageSectionHeader;
} IMAGE_HEADERS, *PIMAGE_HEADERS;


STATUS
ImageHeadersPreInit(
   _Inout_  PIMAGE_HEADERS pImageHeaders
);

STATUS
AnalyzeImageHeaders(
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
);

STATUS
CheckImageDOSHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
);

STATUS
CheckImageNTHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
);

STATUS
CheckImageFileHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
);

STATUS
CheckImageOptionalHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
);

STATUS
CheckImageSectionsHeader(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
);

STATUS
CheckImageDirectoryEntryExport(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
);

STATUS
CheckImageDirectoryEntryImport(
   _Inout_  PIMAGE_HEADERS pImageHeaders,
   _In_     PFILE_MAP      pFileMap,
   _In_     FILE*          file
);

#endif //IMAGE_HEADERS_H

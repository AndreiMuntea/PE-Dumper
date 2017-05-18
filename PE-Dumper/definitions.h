#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <Windows.h>

typedef DWORD STATUS;

#define MZ                                0x00005A4D
#define PE                                0x00004550

#define ERROR_BITS                        0xFFFF0000
#define WARNING_BITS                      0x0000FFFF
#define INTERNAL_ERROR_BITS               0xF0000000

#define EXIT_SUCCESS_STATUS               0x00000000
#define SUCCESS(status)                   (!((status) & (ERROR_BITS | WARNING_BITS)))
#define INTERNAL_ERROR(status)            ((status) & INTERNAL_ERROR_BITS)


#define BAD_ALLOCATION                    0x10000000
#define NULL_POINTER                      0x20000000


#define INVALID_MZ_FORMAT                 0x00100000
#define OUT_OF_BOUNDS                     0x00200000
#define INVALID_PE_FORMAT                 0x00300000
#define INVALID_SECTION                   0x00400000
#define INVALID_DOS_HEADER                0x00500000
#define INVALID_NT_HEADER                 0x00600000
#define INVALID_FILE_HEADER               0x00700000
#define INVALID_OPTIONAL_HEADER           0x00800000
#define INVALID_SECTIONS_HEADER           0x00900000
#define INVALID_EXPORT_DIRECTORY          0x00A00000
#define INVALID_IMPORT_DIRECTORY          0x00B00000

#define INVALID_CHARACTERISTICS           0x00100001
#define INVALID_MACHINE                   0x00100002
#define INVALID_SUBSYSTEM                 0x00100003


#define FILE_IO_ERROR                     0x01000000
#define INVALID_ARGUMENTS                 0x02000000

#endif //DEFINITIONS_H
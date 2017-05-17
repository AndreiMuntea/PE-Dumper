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

#endif //DEFINITIONS_H
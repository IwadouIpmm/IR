#ifndef __MACRO__ERRORS__HF__
#define __MACRO__ERRORS__HF__

//Macros for char and unsigned char error indicators.
#define UCAR__ERROR 0xFF
#define PCAR__ERROR 0x7F
#define NCAR__ERROR 0x80

//Macros for char and unsigned char maxium value digit. Used in functions like is_scar, is_ucar, to_scar and to_ucar
#define MAXD__SCAR 19
#define MAXD__UCAR 20

//Macros for short int and unsigned short int error indicators.
#define USHI__ERROR 0xFFFF
#define PSHI__ERROR 0x7FFF
#define NSHI__ERROR 0x8000

//Macros for short and unsigned short maximum value digit. Used in functions like: is_sshi, is_usshi, to_sshi and to_ushi
#define MAXD__SSHI 39
#define MAXD__USHI 42

//Macros for int and unsigned int error indicators.
#define UINT__ERROR 0xFFFFFFFF
#define PINT__ERROR 0x7FFFFFFF
#define NINT__ERROR 0x80000000

//Macros for int and unsigned int maximum value digit. Used in functions like is_sint, is_uint, to_sint and to_uint
#define MAXD__SINT 83
#define MAXD__UINT 85

//Macros for long long int and unsigned long long int error indicators.
#define ULLI__ERROR 0xFFFFFFFFFFFFFFFF
#define PLLI__ERROR 0x7FFFFFFFFFFFFFFF
#define NLLI__ERROR 0x8000000000000000

//Macros for long long and unsigned long long maximum value digit. Used in functions like is_slli, is_ulli, to_slli and to_ulli
#define MAXD__SLLI 171
#define MAXD__ULLI 172

//Macros for floating point error indicators.
#define PFLT__ERROR 0x7FFFFFFF.0fp0
#define NFLT__ERROR ((0x80000000.0fp0) * -1)

//Macros for double error indicators.
#define PDBL__ERROR 0x7FFFFFFFFFFFFFFF.0fp0
#define NDBL__ERROR ((0x8000000000000000.0fp0) * -1)

#endif

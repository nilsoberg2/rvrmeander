#ifndef COMMON_H__20090106__
#define COMMON_H__20090106__


#if defined(__GNUC__) || defined(__CYGWIN__) || defined(__MINGW32__)
#define DLL_EXPORT
#define DLL_IMPORT
#elif defined(__INTEL_COMPILER)
#define DLL_EXPORT
#define DLL_IMPORT
#elif defined(_MSC_VER)
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#endif


#define OK                                   0
#define ERR_BAD_ARG                         -1
#define ERR_NOT_IMPL                        -10
#define ERR_FILE_OPEN                       -50
#define ERR_FILE_READ                       -51
#define ERR_FILE_FORMAT                     -52
#define ERR_FILE_ELEM_COUNT                 -53


#endif//COMMON_H__20090106__

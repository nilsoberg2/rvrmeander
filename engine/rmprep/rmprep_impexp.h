#ifndef __RMPREP_IMPEXP_H___________________20090413132121__
#define __RMPREP_IMPEXP_H___________________20090413132121__


#if defined(LIBPREP_EXPORTS)
#define LIBPREP_DLL_IMP_EXP DLL_EXPORT
#elif defined(LIBPREP_STATIC)
#define LIBPREP_DLL_IMP_EXP
#else
#define LIBPREP_DLL_IMP_EXP DLL_IMPORT
#endif


#endif//__RMPREP_IMPEXP_H___________________20090413132121__

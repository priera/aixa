#ifndef AIXA_AIXA_EXPORT_H
#define AIXA_AIXA_EXPORT_H

#ifdef WIN32
#ifdef LIB_EXPORTING
    #ifndef LIB_EXPORT
    #define LIB_EXPORT __declspec(dllexport)
    #endif
#else
    #ifndef LIB_EXPORT
    #define LIB_EXPORT __declspec(dllimport)
    #endif
#endif
#else
#define LIB_EXPORT
#endif

#endif //AIXA_AIXA_EXPORT_H

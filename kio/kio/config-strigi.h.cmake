#cmakedefine STRIGI_NEEDS_SIGNED_CHAR 1
#ifdef STRIGI_NEEDS_SIGNED_CHAR
#define STRIGI_ENDANALYZER_RETVAL signed char
#else
#define STRIGI_ENDANALYZER_RETVAL char
#endif
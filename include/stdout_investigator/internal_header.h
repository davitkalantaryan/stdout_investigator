//
// file:			internal_header.h
// path:			include/stdout_investigator/internal_header.h
// created on:		2022 Jun 17
// created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#ifndef STDOUT_INVEST_INCLUDE_STDOUT_INVEST_INTERNAL_HEADER_H
#define STDOUT_INVEST_INCLUDE_STDOUT_INVEST_INTERNAL_HEADER_H

#include <cpputils/internal_header.h>


#if defined(STDOUT_INVEST_COMPILING_SHARED_LIB)
    #define STDOUT_INVEST_EXPORT CPPUTILS_DLL_PUBLIC
#elif defined(STDOUT_INVEST_USING_STATIC_LIB_OR_OBJECTS)
    #ifdef STDOUT_INVEST_DEFAULT_VISIBILITY
        #define STDOUT_INVEST_EXPORT
    #else
        #define STDOUT_INVEST_EXPORT CPPUTILS_DLL_PRIVATE
    #endif
#else
    #define STDOUT_INVEST_EXPORT CPPUTILS_IMPORT_FROM_DLL
#endif


#if defined(STDOUT_INVEST_COMPILING_SHARED_LIB_CPP)
    #define STDOUT_INVEST_CPP_EXPORT CPPUTILS_DLL_PUBLIC
#elif defined(STDOUT_INVEST_USING_STATIC_LIB_OR_OBJECTS_CPP)
    #ifdef STDOUT_INVEST_DEFAULT_VISIBILITY_CPP
        #define STDOUT_INVEST_CPP_EXPORT
    #else
        #define STDOUT_INVEST_CPP_EXPORT CPPUTILS_DLL_PRIVATE
    #endif
#else
    #define STDOUT_INVEST_CPP_EXPORT CPPUTILS_IMPORT_FROM_DLL
#endif



#endif  // #ifndef STDOUT_INVEST_INCLUDE_STDOUT_INVEST_INTERNAL_HEADER_H

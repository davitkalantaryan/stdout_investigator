//
// file:		stdout_analyzer_hook02_unix.c
// path:		src/core/stdout_analyzer_hook02_unix.c
// created by:	Davit Kalantaryan (davit.kalataryan@desy.de)
// created on:	2022 Jun 14
//


#ifdef STDOUT_INVEST_USE_HOOK02
#if (!defined(_WIN32)) || defined(__INTELICENCE__)

#include <stdout_investigator/investigator.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <dlfcn.h>

CPPUTILS_BEGIN_C

typedef int (*Type_puts)(const char*);
typedef size_t (*Type_fwrite)(const void *ptr, size_t size, size_t nmemb,FILE *stream);


static void __attribute__((constructor)) ConstructStdoutInvestigateor(void);

static void HandleUserStdout(const char* a_buffer, size_t a_unBufferSize );
static void HandleUserStderr(const char* a_buffer, size_t a_unBufferSize );


static Type_puts      s_original_puts   =  CPPUTILS_NULL;
static Type_fwrite      s_original_fwrite   =  CPPUTILS_NULL;
bool        s_stdoutInvestCanRunWriteCallback = true;


STDOUT_INVEST_EXPORT StdoutInvestTypeWriter  g_stdoutInvestStdoutHandler = &HandleUserStdout;
STDOUT_INVEST_EXPORT StdoutInvestTypeWriter  g_stdoutInvestStderrHandler = &HandleUserStderr;


STDOUT_INVEST_EXPORT void StdoutInvestWriteToStdOutNoClbk(const char* a_buffer, size_t a_unBufferSize)
{
    write(STDOUT_FILENO,a_buffer,a_unBufferSize);
}


STDOUT_INVEST_EXPORT void StdoutInvestWriteToStdErrNoClbk(const char* a_buffer, size_t a_unBufferSize)
{
    write(STDERR_FILENO,a_buffer,a_unBufferSize);
}


STDOUT_INVEST_EXPORT void StdoutInvestStopCallbacks(void)
{
    s_stdoutInvestCanRunWriteCallback = false;
}


STDOUT_INVEST_EXPORT void StdoutInvestStartCallbacks(void)
{
    s_stdoutInvestCanRunWriteCallback = true;
}


size_t fwrite(const void* a_ptr, size_t a_size, size_t a_nmemb,FILE* a_stream)
{
    if(!s_original_fwrite){
        ConstructStdoutInvestigateor();
    }
	
	if(s_stdoutInvestCanRunWriteCallback){
		const size_t cunReturn = a_size*a_nmemb;
		if(a_stream==stdout){
			(*g_stdoutInvestStdoutHandler)(CPPUTILS_STATIC_CAST(const char*,a_ptr),cunReturn);
			return cunReturn;
		}
		else if(a_stream==stderr){
			(*g_stdoutInvestStderrHandler)(CPPUTILS_STATIC_CAST(const char*,a_ptr),cunReturn);
			return cunReturn;
		}
	} // if(s_stdoutInvestCanRunWriteCallback){
	
    return (*s_original_fwrite)(a_ptr, a_size, a_nmemb,a_stream);
}


int puts(const char* a_cpcString)
{
    if(!s_original_puts){
        ConstructStdoutInvestigateor();
    }
	
	if(s_stdoutInvestCanRunWriteCallback){
		const size_t cunReturn = strlen(a_cpcString);
		(*g_stdoutInvestStdoutHandler)(a_cpcString,cunReturn);
		return cunReturn;
	}
	
    return (*s_original_puts)(a_cpcString);
}



static void HandleUserStdout(const char* a_buffer, size_t a_unBufferSize)
{
    write(STDOUT_FILENO,a_buffer,a_unBufferSize);
}


static void HandleUserStderr(const char* a_buffer, size_t a_unBufferSize)
{
    write(STDERR_FILENO,a_buffer,a_unBufferSize);
}



static void __attribute__((constructor)) ConstructStdoutInvestigateor(void)
{
    s_original_puts = CPPUTILS_REINTERPRET_CAST(Type_puts,dlsym(RTLD_NEXT, "puts"));
    s_original_fwrite = CPPUTILS_REINTERPRET_CAST(Type_fwrite,dlsym(RTLD_NEXT, "fwrite"));
    s_stdoutInvestCanRunWriteCallback = true;
}


CPPUTILS_END_C


#endif  // #if (!defined(_WIN32)) || defined(__INTELICENCE__)
#endif  // #ifdef STDOUT_INVEST_USE_HOOK02

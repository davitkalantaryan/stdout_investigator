//
// file:			investigator.h
// path:			include/stdout_investigator/investigator.h
// created on:		2022 Jun 17
// created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#ifndef STDOUT_INVEST_INCLUDE_STDOUT_INVESTIGATOR_INVESTIGATOR_H
#define STDOUT_INVEST_INCLUDE_STDOUT_INVESTIGATOR_INVESTIGATOR_H

#include <stdout_investigator/internal_header.h>
#include <stddef.h>

CPPUTILS_BEGIN_C

typedef void (*StdoutInvestTypeWriter)(const char*,size_t);

extern STDOUT_INVEST_EXPORT StdoutInvestTypeWriter  g_stdoutInvestStdoutHandler;
extern STDOUT_INVEST_EXPORT StdoutInvestTypeWriter  g_stdoutInvestStderrHandler;

STDOUT_INVEST_EXPORT void StdoutInvestWriteToStdOutNoClbk(const char* a_buffer, size_t a_unBufferSize);
STDOUT_INVEST_EXPORT void StdoutInvestWriteToStdErrNoClbk(const char* a_buffer, size_t a_unBufferSize);
STDOUT_INVEST_EXPORT void StdoutInvestStopCallbacks(void);
STDOUT_INVEST_EXPORT void StdoutInvestStartCallbacks(void);

CPPUTILS_END_C

#endif  // #ifndef STDOUT_INVEST_INCLUDE_STDOUT_INVESTIGATOR_INVESTIGATOR_H

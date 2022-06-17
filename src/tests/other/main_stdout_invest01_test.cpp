//
// file:		main_stdout_invest01_test.c
// path:		src/tests/other/main_stdout_invest01_test.c
// created by:	Davit Kalantaryan (davit.kalataryan@desy.de)
// created on:	2022 Jun 17
//

#include <stdio.h>
#include <iostream>
#include <stdout_investigator/investigator.h>


static void StdoutInvestigatorStatic(const char* a_pBuffer, size_t a_size)
{
	StdoutInvestStopCallbacks();
	printf("buffLen:%d => ",static_cast<int>(a_size)); fflush(stdout);
	StdoutInvestStartCallbacks();
	StdoutInvestWriteToStdOutNoClbk(a_pBuffer,a_size);
}

int main(void)
{
	printf("press any key and then enter to continue "); fflush(stdout);
	getchar();
	g_stdoutInvestStdoutHandler = &StdoutInvestigatorStatic;
	::std::cout << "Hello world!\n";
	
	return 0;
}

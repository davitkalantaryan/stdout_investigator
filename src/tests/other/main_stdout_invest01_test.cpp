//
// file:		main_stdout_invest01_test.c
// path:		src/tests/other/main_stdout_invest01_test.c
// created by:	Davit Kalantaryan (davit.kalataryan@desy.de)
// created on:	2022 Jun 17
//

#include <stdio.h>
#include <iostream>
#include <stdout_investigator/investigator.h>


void StdoutInvestigatorStatic(const char* a_pBuffer, size_t a_size)
{
	StdoutInvestStopCallbacks();
	printf("buffLen:%d => ",static_cast<int>(a_size));
	StdoutInvestWriteToStdOutNoClbk(a_pBuffer,a_size);
}

int main(void)
{
	::std::cout << "Hello world!\n";
	
	return 0;
}

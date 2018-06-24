#include "Main.h"

//application inpoint
int main(int in_argc, char ** in_argv)
{
	int result = 0;

	//result |= UnitGVector3Float::Run();

	//assert(0 == result);

#if DSC_DEBUG
	printf( "\n<press enter key to exit>" ); 
	char c; 
	scanf_s("%c", &c); 
#endif

	return result;
}

#include <stdlib.h> // EXIT_* macros
#include <stdio.h> // TODO: replace printf with OS fns
#include <proto/exec.h> // OpenLibrary
#include <proto/dos.h> // Dos library fns

struct DosLibrary *DOSBase;
struct ExecBase *SysBase;


static unsigned int myStrLen(const char* szStr) {
	unsigned int lLen = 0;
	while(*szStr++) {
		lLen++;
	}
	return lLen;
}

static void myPuts(const char *szMsg) {
	Write(Output(), (char*)szMsg, myStrLen(szMsg));
	Write(Output(), (char*)"\n", 1);
}

int main(int lArgCount, const char *pArgs[]) {
	SysBase = *((struct ExecBase**)4UL);
	DOSBase = (struct DosLibrary*)OpenLibrary((CONST_STRPTR)"dos.library", 0);

	if (!DOSBase) {
		return(EXIT_FAILURE);
	}

	if(lArgCount == 1) {
		// No args - print current dir
		const char *szCurrDir = "???"; // TODO: get current directory properly
		myPuts(szCurrDir);
	}
	else if(lArgCount != 2) {
		puts("wrong number of arguments");
		CloseLibrary((struct Library *) DOSBase);
		return(EXIT_FAILURE);
	}
	else {
		BPTR bpDirLock = Lock((STRPTR)pArgs[1], ACCESS_READ);
		if(bpDirLock == 0) {
			char szMsg;
			printf("Can't find %s\n", pArgs[1]);
			CloseLibrary((struct Library *) DOSBase);
			return(EXIT_FAILURE);
		}
		CurrentDir(bpDirLock);
		UnLock(bpDirLock);
	}

	CloseLibrary((struct Library *) DOSBase);
	return(EXIT_SUCCESS);
}

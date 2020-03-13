#include <stdio.h>
#include <Windows.h>
#include <process.h>

unsigned WINAPI ThreadFunc(void *arg);

int main(int argc, char *argv[])
{
	HANDLE hThread;
	unsigned threadID;
	int param = 5;
	DWORD wr;
	hThread = (HANDLE)_beginthreadex(NULL,0,ThreadFunc, (void*)&param,0,&threadID);

	if (hThread == 0)
	{
		puts("_beginthreadex() error");
		return -1;
	}

	if ((wr = WaitForSingleObject(hThread, INFINITE)) == WAIT_FAILED)
	{
		puts("thread wait error");
		return -1;
	}
	printf("wait result : %s \n ",(wr == WAIT_OBJECT_0) ? "signaled":"non-signaled");


	puts("end of main");
	return 0;

}

unsigned WINAPI ThreadFunc(void *arg){
	
	int i;
	int cnt = *((int *)arg);
	for ( i = 0; i < cnt; i++)
	{
		Sleep(1000);
		puts("runing thread");
	}
	return 0;
}
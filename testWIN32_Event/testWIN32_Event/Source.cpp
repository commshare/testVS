#include <windows.h>
#include <process.h>
#include<iostream>
using namespace std;

DWORD WINAPI ThreadProc(LPVOID lpParam);
DWORD WINAPI ThreadProc2(LPVOID lpParam);

DWORD g_dwThreadID;
DWORD g_dwThreadID2;

UINT g_nTickets = 300;  //int g_nTickets = 300;  //备注1  
HANDLE g_hEvent = NULL;

HANDLE g_hEvent1 = NULL;
HANDLE g_hEvent2 = NULL;

CRITICAL_SECTION g_cs;

int ThreadCout = 0;

int main()
{
	cout << "Main thread is running." << endl;

	InitializeCriticalSection(&g_cs);//初始化临界区  

	HANDLE hHandle = CreateThread(NULL, 0, ThreadProc, NULL, 0, &g_dwThreadID);
	ThreadCout++;
	HANDLE hHandle2 = CreateThread(NULL, 0, ThreadProc2, NULL, 0, &g_dwThreadID2);
	ThreadCout++;

	//g_hEvent = CreateEvent(NULL, FALSE,  TRUE, NULL);    
	g_hEvent1 = CreateEvent(NULL, FALSE, TRUE, NULL);  //备注5:g_hEvent1 = CreateEvent(NULL, TRUE,  TRUE, NULL);  
	g_hEvent2 = CreateEvent(NULL, FALSE, TRUE, NULL);  //备注5:g_hEvent2 = CreateEvent(NULL, TRUE,  TRUE, NULL);  

	ResetEvent(g_hEvent1);
	ResetEvent(g_hEvent2);

	SetEvent(g_hEvent1);


	while (TRUE)
	{
		EnterCriticalSection(&g_cs);
		int nCount = ThreadCout;
		LeaveCriticalSection(&g_cs);

		if (nCount == 0)
		{
			cout << "Main thread is break." << endl;
			break;
		}

	}


	Sleep(1000);    //备注4     

	CloseHandle(hHandle);
	CloseHandle(hHandle2);

	DeleteCriticalSection(&g_cs);

	cout << "Main thread is end." << endl;

	system("pause");
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	// cout << "No." << g_dwThreadID << " thread is running." << endl;    
	while (TRUE)
	{
		WaitForSingleObject(g_hEvent1, INFINITE);
		cout << "No.1 " << g_dwThreadID << " thread is running." << endl;

		EnterCriticalSection(&g_cs);
		int temp = g_nTickets;
		LeaveCriticalSection(&g_cs);

		cout << "No.1 " << g_dwThreadID << " thread is temp." << endl;

		if (temp > 0)
		{
			Sleep(10);  //Sleep(1000)   //备注2  
			cout << "No.1-" << g_dwThreadID << " sell ticket : " << temp << endl;


			EnterCriticalSection(&g_cs);
			g_nTickets--;
			LeaveCriticalSection(&g_cs);

			SetEvent(g_hEvent2);
			//ResetEvent(g_hEvent1);//备注6  


		}
		else
		{
			cout << "No.1- break" << endl;
			//ResetEvent(g_hEvent1);//备注6  
			SetEvent(g_hEvent2);//没有这个ThreadProc2不能终止   //备注3  
			break;
		}
	}

	EnterCriticalSection(&g_cs);
	ThreadCout--;
	LeaveCriticalSection(&g_cs);
	cout << "No.1- end" << endl;

	return 0;
}

DWORD WINAPI ThreadProc2(LPVOID lpParam)
{
	//   
	while (TRUE)
	{
		WaitForSingleObject(g_hEvent2, INFINITE);
		cout << "No.2 " << g_dwThreadID2 << " thread is running." << endl;

		EnterCriticalSection(&g_cs);
		int temp = g_nTickets;
		LeaveCriticalSection(&g_cs);

		if (temp > 0)
		{
			Sleep(10);  //Sleep(1000)   //备注2  
			cout << "No.2-" << g_dwThreadID2 << " sell ticket : " << temp << endl;

			EnterCriticalSection(&g_cs);
			g_nTickets--;
			LeaveCriticalSection(&g_cs);

			SetEvent(g_hEvent1);
			//ResetEvent(g_hEvent2);//备注6  
		}
		else
		{
			cout << "No.2- break" << endl;
			//ResetEvent(g_hEvent2);//备注6  
			SetEvent(g_hEvent1);//同样的问题，没有这个ThreadProc不能终止      //备注3  
			break;
		}
	}

	EnterCriticalSection(&g_cs);
	ThreadCout--;
	LeaveCriticalSection(&g_cs);

	cout << "No.2- end" << endl;
	return 0;
}
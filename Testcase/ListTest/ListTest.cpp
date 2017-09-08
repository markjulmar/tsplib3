// ListTest.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#pragma warning(disable:4786)
#include <string>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

CRITICAL_SECTION gcsDebugLog;
HANDLE ghevtDebugLog;
HANDLE ghevtShutdown;
std::list<std::string> garrLogOutput;

inline void MyOutputFunc(std::string& str)
{
	OutputDebugString(str.c_str());
}

///////////////////////////////////////////////////////////////////////////
// LoggerThread
//
// Runs the internal logger thread for the library.
//
unsigned __stdcall LoggerThread(void* pParam)
{
	HANDLE arrHandles[2];
	std::list<std::string> stringList;

	arrHandles[0] = ghevtDebugLog;
	arrHandles[1] = ghevtShutdown;

	// Spin waiting for the event to close
    for (;;)
    {
        // Wait for either our objects to signal (meaning we have trace data)
        // or for the object to die (meaning we are shutting down).
        LONG lResult = WaitForMultipleObjects(2, arrHandles, false, INFINITE);
        if (lResult == (WAIT_OBJECT_0 + 1) || lResult == WAIT_ABANDONED_0 ||
			lResult == (WAIT_ABANDONED_0+1))
            break;

		// Get the critical section
		EnterCriticalSection(&gcsDebugLog);

		// See if we are getting too slow ...
		if (garrLogOutput.size() > 50000)
		{
			puts("TraceOut is too slow for debug output rate, stopping thread.\r\n");
			break;
		}

		// Copy the current array into a new one so we can release 
		// real worker threads inserting into the queue immediately.
		try
		{
			std::cout << "-- Moving " << garrLogOutput.size() << " elements to private list" << std::endl;
			stringList.splice(stringList.begin(), garrLogOutput);
		}
		catch (...)
		{
		}

		// Allow workers to add new things to the queue.
		ResetEvent(ghevtDebugLog);
		LeaveCriticalSection(&gcsDebugLog);

		try 
		{
			// Now dump it all out the debug log
			for_each(stringList.begin(), stringList.end(), MyOutputFunc);
		}
		catch (...)
		{
		}

		stringList.clear();
	}

	// Get the critical section
	EnterCriticalSection(&gcsDebugLog);

	// Thread is exiting due to a shutdown of the provider.
	try
	{
		std::cout << "-- Emptying " << garrLogOutput.size() << " elements from LogOutput" << std::endl;
		garrLogOutput.clear();
	}
	catch (...)
	{
	}

	// Allow workers to continue to run
	LeaveCriticalSection(&gcsDebugLog);
	
	_endthreadex(0);
	return 0;

}// LoggerThread

void AddBuffer(std::string& strBuff)
{
	EnterCriticalSection(&gcsDebugLog);
	try
	{
		garrLogOutput.push_back(strBuff);
		SetEvent(ghevtDebugLog);
	}
	catch (...)
	{
	}
	LeaveCriticalSection(&gcsDebugLog);
}

unsigned __stdcall RunnerThread(void* pParam)
{
	int i = 0;
	char chBuff[50];

	// Spin waiting for the event to close
    for (;;)
    {
        LONG lResult = WaitForSingleObject(ghevtShutdown, 0);
        if (lResult == WAIT_OBJECT_0 || lResult == WAIT_ABANDONED_0)
            break;

		wsprintf(chBuff, "0x%lx line #%d\n", GetCurrentThreadId(), i++);
		AddBuffer(std::string(chBuff));
	}

	_endthreadex(0);
	return 0;
}

int main(int argc, char* argv[])
{
	const UINT nThreads = 20;

	printf("Multi-threaded list test beginning!\n");

	InitializeCriticalSection(&gcsDebugLog); 
	ghevtDebugLog = CreateEvent(NULL, TRUE, 0, NULL);
	ghevtShutdown = CreateEvent(NULL, TRUE, 0, NULL);

	HANDLE* hHandles = new HANDLE[nThreads+1];

	UINT uiThread;
	hHandles[0] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, LoggerThread, 0, 0, &uiThread));
	if (hHandles[0])
		SetThreadPriority(hHandles[0], THREAD_PRIORITY_NORMAL);

	for (int i = 0; i < nThreads; i++)
	{
		hHandles[i+1] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, RunnerThread, 0, 0, &uiThread));
		if (hHandles[i+1])
			SetThreadPriority(hHandles[i+1], THREAD_PRIORITY_NORMAL);
	}

	while (!kbhit())
	{
		if (WaitForSingleObject(hHandles[0], 0) == WAIT_OBJECT_0)
			break;
		Sleep(500);
	}
		
	printf("Shutting down!\n");
	SetEvent(ghevtShutdown);

	if (kbhit()) _getch();

	// Close the thread
	SetThreadPriority(hHandles[0], THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForMultipleObjects(nThreads+1, hHandles, TRUE, 5000);

	for (i = 0; i < nThreads+1; i++)
		CloseHandle(hHandles[i]);

	DeleteCriticalSection(&gcsDebugLog);

	return 0;
}


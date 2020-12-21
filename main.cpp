#include <iostream>
#include <windows.h>
#include <thread>
#include <ILogger.h>
#include <sstream>

#define THREADCOUNT 100
CRITICAL_SECTION CriticalSection;

struct MyData {
	std::string threadId;
	std::string functionName;
	std::string message;
	Logger& log;
};

DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
	MyData* data = static_cast<MyData*>(lpParam);
	EnterCriticalSection(&CriticalSection);
	for (int i = 0; i < 10; ++i) {
		std::ostringstream ss;
		ss << std::this_thread::get_id();
		std::string idstr = ss.str();
		data->threadId = idstr;
		data->functionName = __FUNCTION__;
		data->message = "Some message";
		std::string messageToLog = data->threadId + " " + data->functionName + " " + data->message;
		data->log.Write(messageToLog);
	}
	LeaveCriticalSection(&CriticalSection);
	return 0;
}

int main()
{
	Logger log;
	MyData data = {"", "", "", log};
	HANDLE hThr[THREADCOUNT] = {};
	DWORD i, dwRet = 0;
	WCHAR* Buf[THREADCOUNT] = {};

	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,
		0x00000400))
		return 0;

	for (int i = 0; i < THREADCOUNT; ++i) {
		DWORD threadId = 0;
		hThr[i] = CreateThread(
			NULL,
			0,
			MyThreadFunction,
			&data,
			0,
			&threadId);				
	}

	WaitForMultipleObjects(
		THREADCOUNT, 
		hThr, 
		TRUE, 
		INFINITE);

	for (i = 0; i < THREADCOUNT; ++i)
	{
		if (CloseHandle(hThr[i]) != 0) {
			std::cout << data.threadId << " handle was closed successfully!" 
				<< std::endl;
		}
		else {
			std::cout << "Failed to close " << data.threadId << " handle, error " 
				<< GetLastError() << std::endl;
		}
	}	
	DeleteCriticalSection(&CriticalSection);
	ExitProcess(0);
	return 0;
}
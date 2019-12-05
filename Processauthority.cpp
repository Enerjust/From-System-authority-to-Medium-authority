/*
	Processauthority.cpp
	Author：3gstudent@3gstudent
	Look through all the process and detect whether the process runs as admin
*/

#include <windows.h>  
#include <TlHelp32.h>  
#pragma comment(lib,"advapi32.lib")

BOOL IsRunasAdmin(HANDLE hProcess)
{
	BOOL bElevated = FALSE;
	HANDLE hToken = NULL;
	if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
		return FALSE;
	TOKEN_ELEVATION tokenEle;
	DWORD dwRetLen = 0;
	if (GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen))
	{
		if (dwRetLen == sizeof(tokenEle))
		{
			bElevated = tokenEle.TokenIsElevated;
		}
	}
	CloseHandle(hToken);
	return bElevated;
}

BOOL CheckProcessauthority()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	BOOL bRunAsAdmin;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("[!]CreateToolhelp32Snapshot Failed.<%d>\n", GetLastError());
		return(FALSE);
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		printf("[!]Process32First Failed.<%d>\n", GetLastError());
		CloseHandle(hProcessSnap);
		return(FALSE);
	}
	printf("\n%-40s	 PID	Run as Admin\n", "Process");
	printf("========================================	====	============\n");
	do
	{
		printf("%-40s	%4d	", pe32.szExeFile, pe32.th32ProcessID);
		hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
		bRunAsAdmin = IsRunasAdmin(hProcess);
		if (bRunAsAdmin)
			printf("%-12s\n", "Yes");
		else
			printf("\n");


	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(TRUE);
}

int main()
{
	CheckProcessauthority();
	return 0;
}



#define BUF_SIZE 1024
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

TCHAR szName[] = TEXT("�������");
TCHAR vaName[] = TEXT("VMAlloc");
LPCWSTR Semaphore = (LPCWSTR) "�������";

int main()
{
	SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    char buff[255];
    int len;
    HANDLE hMapFile;
    LPCTSTR pBuf, NewMem;
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, 0, Semaphore);
    HANDLE Console = GetStdHandle(STD_ERROR_HANDLE);
    len = sprintf(buff, "app.exe \n");
    WriteFile(Console, buff, len, NULL, NULL);

    if (hSemaphore != NULL)
    {
        len = sprintf(buff, "������� ������ �������\n");
        WriteFile(Console, buff, len, NULL, NULL);
    }
    else
    {
        len = sprintf(buff, "������: %d\n", GetLastError());
        WriteFile(Console, buff, len, NULL, NULL);
        _getch();
        return 1;
    }

    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        szName);

    pBuf = (LPTSTR)MapViewOfFile(hMapFile,
                                 FILE_MAP_ALL_ACCESS,
                                 0,
                                 0,
                                 BUF_SIZE);

    MessageBox(NULL, pBuf, TEXT("������� 2 - ������ ���������"), MB_OK);
    WaitForSingleObject(hSemaphore, INFINITE);
    MessageBox(NULL, pBuf, TEXT("������� 2 - ������ ���������"), MB_OK);
    len = sprintf(buff, "\n\t ������� ESC ����� ��������� ���������");
    WriteFile(Console, buff, len, NULL, NULL);

    while (1)
    {
        if (GetAsyncKeyState(27))
        {
            UnmapViewOfFile(pBuf);
            CloseHandle(hMapFile);
            CloseHandle(hSemaphore);
            break;
        }
    }
    return 0;
}
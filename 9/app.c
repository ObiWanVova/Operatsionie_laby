#define BUF_SIZE 1024
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

TCHAR szName[] = TEXT("Главная");
TCHAR vaName[] = TEXT("VMAlloc");
LPCWSTR Semaphore = (LPCWSTR) "Семафор";

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
        len = sprintf(buff, "Семафор открыт успешно\n");
        WriteFile(Console, buff, len, NULL, NULL);
    }
    else
    {
        len = sprintf(buff, "Ошибка: %d\n", GetLastError());
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

    MessageBox(NULL, pBuf, TEXT("Процесс 2 - первое сообщение"), MB_OK);
    WaitForSingleObject(hSemaphore, INFINITE);
    MessageBox(NULL, pBuf, TEXT("Процесс 2 - второе сообщение"), MB_OK);
    len = sprintf(buff, "\n\t Нажмите ESC чтобы завершить программу");
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
#define BUF_SIZE 15000
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

TCHAR szName[] = TEXT("Главная");
TCHAR fMsg[] = TEXT("Привет из Главной");
TCHAR szMsg[] = TEXT("Сообщение с первого процесса");
LPCWSTR Semaphore = (LPCWSTR) "Семафор";

char AllocText[] = "abcdefghijklmno";

int main()
{
	system("cls"); 	// очистка консоли
	SetConsoleOutputCP(1251); 	// установка формата 
    SetConsoleCP(1251);
    char buff[255];
    int len;
    HANDLE Console = GetStdHandle(STD_ERROR_HANDLE); // получение хэндла консоли
    len = sprintf(buff, "Главная программа\n");
    WriteFile(Console, buff, len, NULL, NULL);
    HANDLE hSemaphore = CreateSemaphore(NULL, 0, 1, Semaphore); // инициализация семафора 
	// Функция CreateFileMapping создает или 
	// открывает именованный или безымянный объект отображенного в памяти (проецируемого) фай
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,	// дескриптор файла
        NULL,					// защита
        PAGE_READWRITE,			// атрибуты защиты
        0,						// Старшее двойное слово (DWORD) максимального размера объекта "проецируемый файл".
        BUF_SIZE,				// Младшее двойное слово (DWORD) максимального размера объекта "проецируемый файл"
        szName);				// имя объекта
	// Функция MapViewOfFile отображает представление проецируемого файла в адресное пространство вызывающего процесса.
    LPTSTR pBuf = (LPTSTR)MapViewOfFile(hMapFile, // дескриптор
                                        FILE_MAP_ALL_ACCESS, // режим доступа
                                        0,	// старшее и младшее DWORD смещения
                                        0,
                                        0); // число отображаемых байтов

    len = sprintf(buff, "Написание первого сообщения в память\n");
    WriteFile(Console, buff, len, NULL, NULL);
    CopyMemory((PVOID)pBuf, fMsg, (_tcslen(fMsg) * sizeof(TCHAR))); 	// Копирование блока памяти
    Sleep(2000); // задержка 2 секунды
    STARTUPINFO sInfo; // начальная информация процесса 
    PROCESS_INFORMATION pInfo; // информация процесса 
    ZeroMemory(&sInfo, sizeof(STARTUPINFO)); // заполнение нулями
    sInfo.cb = sizeof(sInfo); // выделение размера
    TCHAR path[MAX_PATH] = _T("app.exe");	// имя программы app.exe
    if (CreateProcess(NULL, path, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sInfo, &pInfo) == 0) // запуск программы и проверка 
    {
        len = sprintf(buff, "Файл app.exe не найден\n");
        WriteFile(Console, buff, len, NULL, NULL);
    }
    len = sprintf(buff, "Ждите 10 секунд\n");
    WriteFile(Console, buff, len, NULL, NULL);
    Sleep(10000); // задержка 10 секунд
    len = sprintf(buff, "Запись:");
    WriteFile(Console, buff, len, NULL, NULL);
    len = sprintf(buff, "(сообщения из первого процесса в программу)\n");
    WriteFile(Console, buff, len, NULL, NULL);
    CopyMemory((PVOID)pBuf, szMsg, (_tcslen(szMsg) * sizeof(TCHAR))); 	// передача второго сообщения
    ReleaseSemaphore(hSemaphore, 1, NULL); 	// освобождение семафора
 	// создание виртуальной памяти
    LPTSTR NewMem = (LPTSTR)VirtualAlloc(
        NULL,
        BUF_SIZE,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);

    if (NewMem != NULL)
    {
        len = sprintf(buff, "Виртуальная память создана\n");
        WriteFile(Console, buff, len, NULL, NULL);
    }
    else
    {
        len = sprintf(buff, "Не вышло\n");
        WriteFile(Console, buff, len, NULL, NULL);
        _getch();
        return 1;
    }
	
    LPTSTR Address = NewMem;

    for (int i = 0; i < 15; i++)
    {
		// копирование блока в память
        CopyMemory((LPVOID)Address, (PVOID)AllocText, sizeof(char));
        len = sprintf(buff, "Адрес:");
        WriteFile(Console, buff, len, NULL, NULL);
        len = sprintf(buff, "0x%X", Address);
        WriteFile(Console, buff, len, NULL, NULL);
        printf("(0x%X) ", Address);
        len = sprintf(buff, "Значение:");
        WriteFile(Console, buff, len, NULL, NULL);
        printf("Value: ");
        len = sprintf(buff, "(%c)\n", AllocText[i]);
        WriteFile(Console, buff, len, NULL, NULL);
        Address += 400; // сдвиг
        Sleep(200);
    }
    len = sprintf(buff, "\n\t Нажмите что нибудь, чтобы завершить программу");
    WriteFile(Console, buff, len, NULL, NULL);
    getchar();
    return 0;
}
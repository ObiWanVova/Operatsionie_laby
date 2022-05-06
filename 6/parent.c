#include "windows.h"

#include "stdio.h"

int main()

{

STARTUPINFO info = { sizeof(info) };

PROCESS_INFORMATION processInfo1, processInfo2; // Создание переменных для хранения информации о потоке

HANDLE hJob = CreateJobObject(NULL, NULL); // Создание задачи

if (!CreateProcess("Child_1.exe", "AA", NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo1)) // Создание процесса

{

return 0;

}

if (!CreateProcess("Child_2.exe", "BB", NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo2)) // Создание следующего процесса

{

return 0;

}

AssignProcessToJobObject(hJob, processInfo2.hProcess); // Процесс связывается с задачей

for (int i = 1; i <= 15; ++i) {

printf("\nI'm a PARENT, step - %d\n", i);

if (i == 7) {

TerminateProcess(processInfo1.hProcess, 0);

printf("Process A completed, step - %d\n\n", i);

}

if (i == 11) {

TerminateJobObject(hJob, 0); // Уничтожение процесса

printf("Process B and doughter completed, step - %d\n\n", i);

}

Sleep(2500);

}

CloseHandle(processInfo1.hProcess);

CloseHandle(processInfo1.hThread);

CloseHandle(processInfo2.hProcess);

CloseHandle(processInfo2.hThread);

return 0;

}
#include "windows.h"

#include "stdio.h"

int main(int argc, char** argv)

{

for (int i = 1; i < 15; ++i) {

printf("---I'm a grson %s, my father %s, step - %d\n", argv[1], argv[0], i);

Sleep(2500);

}

return 0;

}
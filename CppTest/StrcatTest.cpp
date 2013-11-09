#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>

int main()
{
    char path[_MAX_PATH];
    getcwd( path, _MAX_PATH );

    strcat( path, "\\test\\" );

    printf( path );
    return 0;
}

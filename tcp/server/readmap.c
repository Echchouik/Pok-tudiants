#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define RC '\n'
#define NB_ROWS 10
#define NB_COLS 10




char** read_map(char* fileToRead, char map[NB_ROWS][NB_COLS])
{
    
    /* Try to open the file */
    int c;
	int i=0;
	int j=0;
FILE *file;
file = fopen(fileToRead, "r");
if (file) {
    while ((c = getc(file)) != EOF)
{
	for(i;i<NB_ROWS;i++)
	{
		for(j=0;j<NB_COLS;j++) map[i][j]=c;	
		
	}

}
    fclose(file);
}
    return map;
}

void main()
{
char map[NB_ROWS][NB_COLS];
int i = 0;
int j = 0;

read_map("map.txt", map);
for(i;i<NB_ROWS;i++)
	{
		for(j=0;j<NB_COLS;j++){
		printf("%c", map[i][j]);
	}
	printf("\n");
	}
}

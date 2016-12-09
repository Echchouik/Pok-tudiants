#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char *readMap(FILE *fp)
{
    if (!fp)
        return NULL;
    char buffer[BUFSIZ] = { 0 };
    char *line = NULL;
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            line = malloc(len);
            if (!line)
            {
                fclose(fp);
                fprintf(stderr, "Could not allocate memory!\n");
                exit(-1);
            }
            strcpy(line, buffer);
            return line;
        }
        line = malloc(strlen(buffer) + 1);
        strcpy(line, buffer);
        int done = 0;
        while (!done && fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            len = strlen(buffer);
            if (buffer[len - 1] == '\n')
            {
                buffer[len - 1] = '\0';
                done = 1;
            }
            char *temp = realloc(line, strlen(line) + len + 1);
            if (!temp)
            {
                free(line);
                fclose(fp);
                fprintf(stderr, "Could not reallocate memory!\n");
                exit(-1);
            }
            line = temp;
            strcat(line, buffer);
        }
    }
    return line;
}

int main(void)
{
    /* Try to open the file */
    FILE *fp = fopen("map.txt", "r");
    if (!fp)
    {
        fprintf(stderr, "Unable to open %s: %s\n",
                fileToRead, strerror(errno));
        exit(-1);
    }

    /* Our array of lines is actually a pointer to an array of character        
       pointers, one for each line, so keep track of the number of lines. */
    char **lines = NULL;
    size_t lineCount = 0;
    char *line = NULL;

    while ((line = readMap(fp)) != NULL)
    {
        /* As noted above in the function, realloc failure means                
           we need to cleanup the previously created memory, so we              
           use a temporary to hold the returned value, keeping the              
           pointer to the previously created memory valid for cleanup           
        */
        char **temp = realloc(lines, sizeof(*lines) * lineCount + 1);
        if (!temp)
        {
            fprintf(stderr, "Unable to realloc for lines!\n");
            /* Cleanup the existing memory */
            size_t i = 0;
            for (; i < lineCount; ++i)
            {
                free(lines[i]);
            }
            free(lines);
            fclose(fp);
            exit(-1);
        }

        /* Assign our lines variable to point to the reallocated memory */
        lines = temp;

        /* Set our newly allocated pointer to point to the line returned */
        lines[lineCount++] = line;
    }

    /* Print our lines and clean up */
    for (int j = 0; j < lineCount; j++)
    {
        printf("%s\n", lines[j]);

        /* Free the line's memory allocated in the readLine function */
        free(lines[j]);
    }

    /* Free the array that held the lines */
    free(lines);

    fclose(fp);

    return 0;
}
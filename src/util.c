#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


char *util_read_file(const char *path)
{
    char *contents = malloc(sizeof(char));
    contents[0] = '\0';

    FILE *fp = fopen(path, "r");
    char *line = 0;
    size_t len = 0;
    ssize_t read;

    if (!fp)
    {
        fprintf(stderr, "[util_read_file]: Couldn't read file %s\n", path);
        return 0;
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        unsigned long prev_length = strlen(contents);
        contents = realloc(contents, (strlen(contents) + read) * sizeof(char));

        memcpy(&contents[prev_length], line, (read - 1) * sizeof(char));
        contents[prev_length + read - 1] = '\0';
    }

    free(line);
    fclose(fp);

    return contents;
}


float util_restrict_angle(float angle)
{
    if (angle < 0.f)
        angle += 2.f * M_PI;

    if (angle > 2.f * M_PI)
        angle -= 2.f * M_PI;

    return angle;
}


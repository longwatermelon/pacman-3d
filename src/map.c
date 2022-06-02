#include "map.h"
#include "util.h"


struct Map *map_alloc(const char *map_fp, const char *info_fp)
{
    struct Map *m = malloc(sizeof(struct Map));
    m->layout = util_read_file(map_fp);

    FILE *f = fopen(info_fp, "r");
    fscanf(f, "%d %d %d", &m->dim.x, &m->dim.y, &m->tile_size);
    fclose(f);

    return m;
}


void map_free(struct Map *m)
{
    free(m->layout);
    free(m);
}


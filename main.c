#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct map {
    char** grid;
    int height, width;
    int level;
} map;

typedef struct edges {
    int left, right;
} edges;

void free_map(map* m);
void print_map(map* m);
void init_map(map* m, int* heights);
edges* get_edges(map* m, int row);
int get_volume(map* m);

int main(int argc, char* argv[]) {

    for (int i=1; i<argc; i++) {
        for (int j=0; argv[i][j]; j++) {

            if (!isdigit(argv[i][j])) {
                fprintf(stderr, "\033[31mERROR: \033[0m"
                                "Arguments must be positive integers\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    int* heights = calloc(argc, sizeof(int));
    int max_h = 0, level = 0;

    for (int i=0; i<argc-1; i++) {
        int h = atoi(argv[i+1]);
        heights[i] = h;

        if (h >= max_h) {
            level = max_h;
            max_h = h;
        }
    }

    map m;
    m.height = max_h;
    m.width = argc - 1;
    m.level = level;

    m.grid = calloc(max_h, sizeof(char*));

    if (m.grid == NULL) {
        exit(EXIT_FAILURE);
    }
    for (int i=0; i<m.height; i++) {
        m.grid[i] = calloc(m.width, sizeof(char));

        if (m.grid[i] == NULL) {
            exit(EXIT_FAILURE);
        }
    }

    init_map(&m, heights);

    int volume = get_volume(&m);
    print_map(&m);
    printf("Volume = %i\n", volume);

    free_map(&m);
    free(heights);
    return 0;
}

int get_volume(map* m) {

    int volume = 0;

    for (int row = m->height-1; row >= m->height-m->level; row--) {
        edges* e = get_edges(m, row);

        for (int col = e->left+1; col < m->width-e->right; col++) {

            if (m->grid[row][col] != 'X') {
                m->grid[row][col] = '~';
                volume++;
            }
        }
        free(e);
    }
    return volume;
}

void free_map(map* m) {

    for (int row=0; row<m->height; row++) {
        free(m->grid[row]);
    }
    free(m->grid);
}

void print_map(map* m) {

    printf("\n");

    for (int row=0; row<m->height; row++) {
        for (int col=0; col<m->width; col++) {

            if (m->grid[row][col] == 'X') {
                printf("\033[107m  \033[0m");
            }
            else if (m->grid[row][col] == '~') {
                printf("\033[44m  \033[0m");
            }
            else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void init_map(map* m, int* heights) {

    for (int col=0; col<m->width; col++) {
        for (int row = m->height-1; row >= m->height-heights[col]; row--) {
            m->grid[row][col] = 'X';
        }
    }
}

edges* get_edges(map* m, int row) {

    edges* e = calloc(1, sizeof(edges));
    int fromL = 0, fromR = 0;

    for (int col=0; col<m->width && m->grid[row][col] == 0; col++) {
        fromL++;
    }
    for (int col=m->width-1; col>=0 && m->grid[row][col] == 0; col--) {
        fromR++;
    }

    e->left = fromL;
    e->right = fromR;
    return e;
}


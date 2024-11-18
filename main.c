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
void check_arguments(int argc, char* argv[]);
char** allocate_grid(map* m);
void get_heights(int* heights, int argc, char** argv, int* max_h, int* level);

int main(int argc, char* argv[]) {

    check_arguments(argc, argv);

    int* heights = calloc(argc, sizeof(int));

    if (heights == NULL) {
        fprintf(stderr, "\033[31mERROR:\033[0m Memory Allocation Failure");
        exit(EXIT_FAILURE);
    }
    int max_h = 0, level = 0;

    get_heights(heights, argc, argv, &max_h, &level);

    map m;
    m.height = max_h;
    m.width = argc - 1;
    m.level = level;
    m.grid = allocate_grid(&m);

    init_map(&m, heights);

    int volume = get_volume(&m);
    print_map(&m);
    printf("\033[1;3;36mVolume = %i\033[0m\n\n", volume);

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

char** allocate_grid(map* m) {

    char** grid = calloc(m->height, sizeof(char*));

    if (grid == NULL) {
        exit(EXIT_FAILURE);
    }
    for (int i=0; i<m->height; i++) {
        grid[i] = calloc(m->width, sizeof(char));

        if (grid[i] == NULL) {
            exit(EXIT_FAILURE);
        }
    }
    return grid;
}

void get_heights(int* heights, int argc, char** argv, int* max_h, int* level) {

    for (int i=0; i<argc-1; i++) {
        int h = atoi(argv[i+1]);
        heights[i] = h;

        if (h >= *max_h) {
            *level = *max_h;
            *max_h = h;
        }
        if (h > *level) {
            *level = h;
        }
    }
}

void check_arguments(int argc, char* argv[]) {

    for (int i=1; i<argc; i++) {
        for (int j=0; argv[i][j]; j++) {

            if (!isdigit(argv[i][j])) {
                fprintf(stderr, "\033[31mERROR: \033[0m"
                                "Arguments must be positive integers\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

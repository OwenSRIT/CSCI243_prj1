// #define _DEFAULT_SOURCE

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"

#define EMPTY ' '
#define TREE 'Y'
#define BURNING_1 '1'
#define BURNING_2 '2'
#define BURNING_3 '3'
#define BURNED '.'

static int start_burn_prop = 10;
static int burn_prob = 30;
static int tree_dense = 50;
static int neigbor_prop = 25;
static int print_cycles = 0;
static int grid_size = 10;
static int burned_trees = 0;

static void usage() {
  fprintf(stderr, "\nusage: start a wildfire simulation [-H -bNUM -cNUM -dNUM "
                  "-nNUM -sNUM -pNUM]\n");
  fprintf(stderr, "\t-H is for getting help\n");
  fprintf(stderr,
          "\t-bN sets the proportion of trees that are buning at the start\n");
  fprintf(stderr, "\t-cN sets the probability of a tree catching fire\n");
  fprintf(stderr, "\t-dN sets the density of trees at the start\n");
  fprintf(stderr, "\t-nN sets the percentage of neighbors that need to be on "
                  "fire for a tree to catch fire\n");
  fprintf(stderr, "\t-sN sets the size of the grid\n");
  fprintf(stderr, "\t-pN sets the program to print mode and will print N "
                  "cycles of the simulation\n");
}

static int **initialize_grid() {
  float grid_area = grid_size * grid_size;
  srand(41);
  int **grid = malloc(sizeof(int[grid_size][grid_size]));
  for (int i = 0; i < grid_size; i++) {
    grid[i] = malloc(sizeof(int[grid_size]));
  }
  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      grid[row][col] = ' ';
    }
  }
  float trees = ((tree_dense / 100.00) * grid_area);

  float tree_count = 0;
  float burn_count = 0;
  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      if ((tree_count / (grid_size * grid_size)) * 100 < tree_dense) {
        grid[row][col] = TREE;
        tree_count++;
      }
      if (burn_count < (start_burn_prop / 100.00) * trees) {
        grid[row][col] = BURNING_1;
        burn_count++;
      }
    }
  }
  int temp_symbol;
  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      temp_symbol = grid[row][col];

      int rand_row = rand() % grid_size;
      int rand_col = rand() % grid_size;

      grid[row][col] = grid[rand_row][rand_col];
      grid[rand_row][rand_col] = temp_symbol;
    }
  }
  return grid;
}

int update_grid(int **in_grid) {
  int state_updates = 0;
  int **temp_grid = malloc(sizeof(int[grid_size][grid_size]));
  for (int i = 0; i < grid_size; i++) {
    temp_grid[i] = malloc(sizeof(int[grid_size]));
  }
  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      temp_grid[row][col] = in_grid[row][col];
    }
  }

  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      float buring_neighbors = 0;
      if (temp_grid[row][col] == TREE) {
        // northwest
        if (row - 1 >= 0 && col - 1 >= 0) {
          if ((temp_grid[row - 1][col - 1] == BURNING_1 ||
               temp_grid[row - 1][col - 1] == BURNING_2 ||
               temp_grid[row - 1][col - 1] == BURNING_3)) {
            buring_neighbors++;
          }
        }
        // north
        if (row - 1 >= 0) {
          if ((temp_grid[row - 1][col] == BURNING_1 ||
               temp_grid[row - 1][col] == BURNING_2 ||
               temp_grid[row - 1][col] == BURNING_3)) {
            buring_neighbors++;
          }
        }
        // northeast
        if (row - 1 >= 0 && col + 1 < grid_size) {
          if ((temp_grid[row - 1][col + 1] == BURNING_1 ||
               temp_grid[row - 1][col + 1] == BURNING_2 ||
               temp_grid[row - 1][col + 1] == BURNING_3)) {
            buring_neighbors++;
          }
        }
        // west
        if (col - 1 >= 0) {
          if ((temp_grid[row][col - 1] == BURNING_1 ||
               temp_grid[row][col - 1] == BURNING_2 ||
               temp_grid[row][col - 1] == BURNING_3)) {
            buring_neighbors++;
          }
        }
        // east
        if (col + 1 < grid_size) {
          if ((temp_grid[row][col + 1] == BURNING_1 ||
               temp_grid[row][col + 1] == BURNING_2 ||
               temp_grid[row][col + 1] == BURNING_3)) {
            buring_neighbors++;
          }
        }
        // southwest
        if (row + 1 < grid_size && col - 1 >= 0) {
          if ((temp_grid[row + 1][col - 1] == BURNING_1 ||
               temp_grid[row + 1][col - 1] == BURNING_2 ||
               temp_grid[row + 1][col - 1] == BURNING_3)) {
            buring_neighbors++;
          }
        }
        // south
        if (row + 1 < grid_size) {
          if ((temp_grid[row + 1][col] == BURNING_1 ||
               temp_grid[row + 1][col] == BURNING_2 ||
               temp_grid[row + 1][col] == BURNING_3)) {
            buring_neighbors++;
          }
        }
        // southeast
        if (row + 1 < grid_size && col + 1 < grid_size) {
          if ((temp_grid[row + 1][col + 1] == BURNING_1 ||
               temp_grid[row + 1][col + 1] == BURNING_2 ||
               temp_grid[row + 1][col + 1] == BURNING_3)) {
            buring_neighbors++;
          }
        }
        if ((buring_neighbors / 8) * 100 >= neigbor_prop &&
            rand() % 100 <= burn_prob) {
          in_grid[row][col] = BURNING_1;
          state_updates++;
        }

      } else if (temp_grid[row][col] == BURNING_1) {
        in_grid[row][col] = BURNING_2;
      } else if (temp_grid[row][col] == BURNING_2) {
        in_grid[row][col] = BURNING_3;
      } else if (temp_grid[row][col] == BURNING_3) {
        in_grid[row][col] = BURNED;
        burned_trees++;
        state_updates++;
      }
    }
  }

  for (int i = 0; i < grid_size; i++) {
    free(temp_grid[i]);
  }

  free(temp_grid);

  return state_updates;
}

void grid_print(int **grid, int cycle, int tot_changes, int cur_changes) {
  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      if (grid[row][col] == BURNING_1 || grid[row][col] == BURNING_2 ||
          grid[row][col] == BURNING_3) {
        printf("*");
      } else {
        printf("%c", grid[row][col]);
      }
    }
    printf("\n");
  }
  printf("size: %d, pCatch: %f, Density: %f, pBurning: %f, pNeighbor: %f\n"
         "cycle %d, current changes: %d, cumulative changes %d\n",
         grid_size, (float)burn_prob / 100, (float)tree_dense / 100,
         (float)start_burn_prop / 100, (float)neigbor_prop / 100, cycle,
         cur_changes, tot_changes);
}

int main(int argc, char *argv[]) {

  int opt;
  int recieved = 0;

  while ((opt = getopt(argc, argv, "Hb:c:d:n:s:p:")) != -1) {
    switch (opt) {
    case 'H':
      usage();
      break;

    case 'b':
      recieved = (int)strtol(optarg, NULL, 10);
      if (recieved > 0 && recieved <= 100) {
        start_burn_prop = recieved;
      } else {
        fprintf(stderr, "Please enter a proportion between 1 and 100.\n");
      }
      break;

    case 'c':
      recieved = (int)strtol(optarg, NULL, 10);
      if (recieved > 0 && recieved <= 100) {
        burn_prob = recieved;
      } else {
        fprintf(stderr, "Please enter a probability between 1 and 100.\n");
      }
      break;

    case 'd':
      recieved = (int)strtol(optarg, NULL, 10);
      if (recieved > 0 && recieved <= 100) {
        tree_dense = recieved;
      } else {
        fprintf(stderr, "Please enter a percentage between 1 and 100.\n");
      }
      break;

    case 'n':
      recieved = (int)strtol(optarg, NULL, 10);
      if (recieved > 0 && recieved <= 100) {
        neigbor_prop = recieved;
      } else {
        fprintf(stderr, "Please enter a percentage between 1 and 100.\n");
      }
      break;

    case 'p':
      recieved = (int)strtol(optarg, NULL, 10);
      if (recieved > 0) {
        print_cycles = recieved;
      } else {
        fprintf(stderr, "Please enter a non negative number of cycles\n");
      }
      break;

    case 's':
      recieved = (int)strtol(optarg, NULL, 10);
      if (recieved > 4 && recieved <= 40) {
        grid_size = recieved;
      } else {
        fprintf(stderr, "Please enter a size between 5 and 40.\n");
      }
      break;

    default:
      break;
    }
  }

  int **grid = initialize_grid();
  int tot_changes = 0;
  int cur_changes = 0;
  if (print_cycles) {
    for (int i = 0; i < print_cycles; i++) {
      printf("===========================\n"
             "======== Wildfire =========\n"
             "===========================\n"
             "=== Print %02d Time Steps ===\n"
             "===========================\n",
             i + 1);
      grid_print(grid, i + 1, tot_changes, cur_changes);
      cur_changes = update_grid(grid);
      tot_changes += cur_changes;
    }
  }
  return EXIT_SUCCESS;
}

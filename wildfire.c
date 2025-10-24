// #define _DEFAULT_SOURCE

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY ' '
#define TREE 'Y'
#define BURNING '*'
#define BURNED '.'

static int start_burn_prop = 10;
static int burn_prob = 30;
static int tree_dense = 50;
static int neigbor_prop = 25;
static int print_cycles;
static int grid_size = 10;

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

static char ***initialize_grid() {

  fprintf(stderr, "1\n");
  srand(41);
  char **grid = malloc(sizeof(int[grid_size][grid_size]));
  for (int i = 0; i < grid_size; i++) {
    grid[i] = malloc(grid_size);
  }
  int tree_count = 0;
  int burn_count = 0;
  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      if ((tree_count / (grid_size * grid_size)) * 100 < tree_dense) {
        grid[row][col] = TREE;
        tree_count++;
      }
      if ((burn_count / (grid_size * grid_size)) * 100 < start_burn_prop) {
        grid[row][col] = BURNING;
        burn_count++;
      }
    }
  }
  char temp_symbol;
  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      temp_symbol = grid[row][col];

      int rand_row = rand() % grid_size;
      int rand_col = rand() % grid_size;

      grid[row][col] = grid[rand_row][rand_col];
      grid[rand_row][rand_col] = temp_symbol;
    }
  }

  char ***grid_ptr = &grid;

  return grid_ptr;
}

int main(int argc, char *argv[]) {

  int opt;
  int recieved = 0;

  fprintf(stderr, "command line:\t >>> ");
  for (int j = 0; j < argc; ++j) {
    fprintf(stderr, "%s ", argv[j]);
  }
  fprintf(stderr, "\n");

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
  char ***grid = initialize_grid();
  for (int row = 0; row < grid_size; row++) {
    for (int col = 0; col < grid_size; col++) {
      printf("%c", *grid[row][col]);
    }
    printf("\n");
  }
  return EXIT_SUCCESS;
}

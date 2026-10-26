#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define N_ROW 6
#define DEFAULT 0

void print_banner(const char *const *lines, size_t n, int color_cols);

void init();

void end();

#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "textCONST.h"

#define N_ROW 6
#define DEFAULT 0

void print_banner(const wchar_t *const *lines, size_t n, int color_cols);

void print_percent(int percent);

void init();

void end();

#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "textCONST.h"
extern const wchar_t *BANNER_TX[];
extern const wchar_t *BANNER_KB[];
extern const wchar_t *BANNER_MB[];

#define N_ROW 6
#define DEFAULT 0

void print_banner(const wchar_t *const *lines, size_t n, int color_cols);

void print_percent(int percent);

void init();
void print_bytes(int bytes);

void end();

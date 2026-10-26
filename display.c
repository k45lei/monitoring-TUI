#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

extern const char *BANNER_LOGO[];
extern const char *BANNER_CPU[];
extern const char *BANNER_MEMORY[];

/*
 * print_banner:
 *   lines       - NULL-terminated-free array of strings (banner rows)
 *   n           - number of rows in the array
 *   color_cols  - number of leading columns (per row) to render in the
 *                 alternate color. Values <= 0 mean "no colored columns".
 *
 * Each row is printed on its own line; a blank line is emitted afterward.
 * If a row is shorter than color_cols, only its existing characters are
 * colored (no out-of-bounds reads).
 */
void print_banner(const char *const *lines, size_t n, int color_cols) { 
//color_cols is the number of columns that are in a different color
    for (size_t i = 0; i < n; i++) {
        const char *line = lines[i];
        size_t len = strlen(line);

        size_t colored = 0;
        if (color_cols > 0) {
            colored = (size_t)color_cols;
            if (colored > len) colored = len;
        }

        if (colored > 0) {
            attron(COLOR_PAIR(1));
            for (size_t j = 0; j < colored; j++)
                printw("%c", line[j]);
            attroff(COLOR_PAIR(1));
        }

        printw("%s\n", line + colored);
    }
    printw("\n");
}

void init(){
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	curs_set(0);


	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
}

void end(){
	printw("Press any key to exit...");
	refresh();
	getch();
	endwin();
}

int main(void) {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    size_t logo_n = 6;
    size_t cpu_n = 6;
    size_t mem_n = 6;

    /* Pick how many leading columns you want in the alternate color. */
    print_banner(BANNER_LOGO,   logo_n, 10);
    print_banner(BANNER_CPU,    cpu_n,   5);
    print_banner(BANNER_MEMORY, mem_n,   8);

    printw("Press any key to exit...");
    refresh();
    getch();

    endwin();
    return 0;
}

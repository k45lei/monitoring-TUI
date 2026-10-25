#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>

extern const char *BANNER_LOGO[];
extern const char *BANNER_CPU[];
extern const char *BANNER_MEMORY[];

static void print_banner(const char *const *lines, size_t n) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    (void)rows; (void)cols;
    for (size_t i = 0; i < n; i++) {
        printw("%s\n", lines[i]);
    }
    printw("\n");
}

int main(void) {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    size_t logo_n = 6;
    size_t cpu_n = 6;
    size_t mem_n = 6;

    print_banner(BANNER_LOGO, logo_n);
    print_banner(BANNER_CPU, cpu_n);
    print_banner(BANNER_MEMORY, mem_n);

    printw("Press any key to exit...");
    refresh();
    getch();

    endwin();
    return 0;
}

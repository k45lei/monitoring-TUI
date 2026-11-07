#define NCURSES_WIDECHAR 1
#include "display.h"
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "textCONST.h"
#include <wchar.h>

/* External digit banners defined in textCONST.c */
extern const wchar_t *BANNER_ZERO[];
extern const wchar_t *BANNER_ONE[];
extern const wchar_t *BANNER_TWO[];
extern const wchar_t *BANNER_THREE[];
extern const wchar_t *BANNER_FOUR[];
extern const wchar_t *BANNER_FIVE[];
extern const wchar_t *BANNER_SIX[];
extern const wchar_t *BANNER_SEVEN[];
extern const wchar_t *BANNER_EIGHT[];
extern const wchar_t *BANNER_NINE[];

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
void print_banner(const wchar_t *const *lines, size_t n, int color_cols) {
    // Print each line character‑by‑character using wide‑character ncurses API.
    // The first `color_cols` visual columns are rendered with the alternate colour.
    for (size_t i = 0; i < n; i++) {
        const wchar_t *line = lines[i];
        if (!line) continue;
        size_t len = wcslen(line);
        for (size_t j = 0; j < len; j++) {
            cchar_t wc;
            setcchar(&wc, &line[j], A_NORMAL, 0, NULL);
            if ((int)j < color_cols) {
                attron(COLOR_PAIR(1));
                add_wch(&wc);
                attroff(COLOR_PAIR(1));
            } else {
                add_wch(&wc);
            }
        }
        // Print newline
        cchar_t nl;
        setcchar(&nl, L"\n", A_NORMAL, 0, NULL);
        add_wch(&nl);
    }
    // Print an extra newline after the banner
    cchar_t nl2;
    setcchar(&nl2, L"\n", A_NORMAL, 0, NULL);
    add_wch(&nl2);
}

void print_percent(int percent){
    // Display a two‑digit percentage using the BANNER_* digit arrays.
    if (percent < 0) percent = 0;
    if (percent > 99) percent = 99;

    int tens = percent / 10;
    int ones = percent % 10;

    const wchar_t *const *digit_tens;
    const wchar_t *const *digit_ones;

    switch (tens) {
        case 0: digit_tens = BANNER_ZERO;  break;
        case 1: digit_tens = BANNER_ONE;   break;
        case 2: digit_tens = BANNER_TWO;   break;
        case 3: digit_tens = BANNER_THREE; break;
        case 4: digit_tens = BANNER_FOUR;  break;
        case 5: digit_tens = BANNER_FIVE;  break;
        case 6: digit_tens = BANNER_SIX;   break;
        case 7: digit_tens = BANNER_SEVEN; break;
        case 8: digit_tens = BANNER_EIGHT; break;
        case 9: digit_tens = BANNER_NINE;  break;
        default: digit_tens = BANNER_ZERO; break;
    }
    switch (ones) {
        case 0: digit_ones = BANNER_ZERO;  break;
        case 1: digit_ones = BANNER_ONE;   break;
        case 2: digit_ones = BANNER_TWO;   break;
        case 3: digit_ones = BANNER_THREE; break;
        case 4: digit_ones = BANNER_FOUR;  break;
        case 5: digit_ones = BANNER_FIVE;  break;
        case 6: digit_ones = BANNER_SIX;   break;
        case 7: digit_ones = BANNER_SEVEN; break;
        case 8: digit_ones = BANNER_EIGHT; break;
        case 9: digit_ones = BANNER_NINE;  break;
        default: digit_ones = BANNER_ZERO; break;
    }

    // Capture the column where the caller positioned the cursor.
    int start_x = getcurx(stdscr);

    // Banners have N_ROW rows (defined in display.h)
    for (size_t row = 0; row < N_ROW; ++row) {
        // Ensure we start each row at the original column, not column 0.
        int cur_y = getcury(stdscr);
        move(cur_y, start_x);

        const wchar_t *left = digit_tens[row];
        const wchar_t *right = digit_ones[row];
        // print left digit
        for (size_t j = 0; left && left[j]; ++j) {
            cchar_t wc;
            setcchar(&wc, &left[j], A_NORMAL, 0, NULL);
            add_wch(&wc);
        }
        // print right digit
        for (size_t j = 0; right && right[j]; ++j) {
            cchar_t wc;
            setcchar(&wc, &right[j], A_NORMAL, 0, NULL);
            add_wch(&wc);
        }
        // newline after each row (moves cursor to column 0 of next line)
        cchar_t nl;
        setcchar(&nl, L"\n", A_NORMAL, 0, NULL);
        add_wch(&nl);
    }
    // extra blank line after the number
    cchar_t nl2;
    setcchar(&nl2, L"\n", A_NORMAL, 0, NULL);
    add_wch(&nl2);
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




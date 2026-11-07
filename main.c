#define _XOPEN_SOURCE 700
#include "cpuINFO.h"
#include "memINFO.h"
#include "textCONST.h"
#include "display.h"
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>


#include <sys/types.h>
extern int usleep(useconds_t);

static int visible_width(const wchar_t *s) {
    int w = 0;
    for (const wchar_t *p = s; *p; ++p) {
        int cw = wcwidth(*p);
        if (cw < 0) cw = 0;
        w += cw;
    }
    return w;
}

void run(unsigned int usleep_time){
    COREinfo *cpus = malloc((AFTER+1)*sizeof(COREinfo));
    MEMinfo *mem = malloc(sizeof(MEMinfo));
    /* Initial snapshot – this becomes the "BEFORE" reference */
    READ_PROC_LINE(cpus, true);
    double cpu_percent;
    while (true) {
        /* Wait for the sampling interval first */
        usleep(usleep_time);
        /* Capture the "AFTER" snapshot after the wait */
        READ_PROC_LINE(cpus, false);
        /* Compute utilisation for the interval we just waited */
        cpu_percent = CALC_P(cpus);
        /* Prepare for the next iteration: make AFTER the new BEFORE */
        BECOME_BEFORE(cpus);
        /* Memory handling and display */
        READ_MEM_PROC_LINE(mem);
        calcGiB(mem);
        erase();
        int cpu_cols = (int)(((double)cpu_percent / 100.0) * (double)visible_width(BANNER_CPU[0]));
        int mem_cols = (int)((mem->usedGiB / mem->totalGiB) * (double)visible_width(BANNER_MEMORY[0]));
        print_banner(BANNER_LOGO, N_ROW, DEFAULT);
        print_banner(BANNER_CPU, N_ROW, cpu_cols);
        print_banner(BANNER_MEMORY, N_ROW, mem_cols);
        refresh();
    }
    free(mem);
    free(cpus);
}


int main(int argc, char *argv[]) {
    unsigned int usleep_time = 100000; // default 10ms
    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--usleep") == 0) && i + 1 < argc) {
            usleep_time = (unsigned int)strtoul(argv[i + 1], NULL, 10);
            i++; // skip next
        }
    }
    init();

	run(usleep_time);
	
    end();
    return 0;
}

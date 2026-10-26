#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>

typedef struct{ uint64_t user, nice, system, idle, iowait, irq, softirq, steal; uint64_t user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1; double percent; } COREinfo;
typedef struct{ long totalKB, usedKB; double totalGiB, usedGiB; } MEMinfo;

int get_CORECOUNT(void);
void READ_PROC_LINE(COREinfo *cinfo, int core_count, bool before);
void CALC_P(COREinfo *cpu, int core_count);
void READ_MEM_PROC_LINE(MEMinfo *m);
void calcGiB(MEMinfo *m);

extern const char *BANNER_LOGO[];
extern const char *BANNER_CPU[];
extern const char *BANNER_MEMORY[];

void print_banner(const char *const *lines, size_t n, int color_cols);
void init(void);
void end(void);

int main(void) {
    init();

    int core_count = get_CORECOUNT();
    COREinfo *cpus = calloc(core_count, sizeof(COREinfo));
    READ_PROC_LINE(cpus, core_count, true);
    usleep(100000);
    READ_PROC_LINE(cpus, core_count, false);
    CALC_P(cpus, core_count);

    MEMinfo mem;
    READ_MEM_PROC_LINE(&mem);
    calcGiB(&mem);

    double avg = 0;
    for (int i = 0; i < core_count; i++) avg += cpus[i].percent;
    avg /= core_count;

    int cpu_cols = (int)((avg / 100.0) * (double)strlen(BANNER_CPU[0]));
    int mem_cols = (int)((mem.usedGiB / mem.totalGiB) * (double)strlen(BANNER_MEMORY[0]));

    print_banner(BANNER_LOGO, 6, 0);
    print_banner(BANNER_CPU, 6, cpu_cols);
    print_banner(BANNER_MEMORY, 6, mem_cols);

    end();
    free(cpus);
    return 0;
}
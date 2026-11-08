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
#include <pthread.h>

/* Shared state between UI thread and collector thread */
typedef struct {
    double cpu_percent;
    double mem_used_gib;
    double mem_total_gib;
    unsigned int usleep_time;
    int quit;
    pthread_mutex_t lock;
} shared_state_t;

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


/* CPU collector thread – reads /proc/stat at the requested interval */
static void *cpu_collector_thread(void *arg) {
    shared_state_t *st = (shared_state_t *)arg;
    COREinfo *cpus = malloc((AFTER+1)*sizeof(COREinfo));
    /* Initial snapshot – this becomes the "BEFORE" reference */
    READ_PROC_LINE(cpus, true);
    while (!st->quit) {
        usleep(st->usleep_time);
        /* Capture the "AFTER" snapshot after the wait */
        READ_PROC_LINE(cpus, false);
        double cpu_percent = CALC_P(cpus);
        BECOME_BEFORE(cpus);
        pthread_mutex_lock(&st->lock);
        st->cpu_percent = cpu_percent;
        pthread_mutex_unlock(&st->lock);
    }
    free(cpus);
    return NULL;
}

/* Memory collector thread – reads /proc/meminfo at the requested interval */
static void *mem_collector_thread(void *arg) {
    shared_state_t *st = (shared_state_t *)arg;
    MEMinfo *mem = malloc(sizeof(MEMinfo));
    while (!st->quit) {
        usleep(st->usleep_time);
        READ_MEM_PROC_LINE(mem);
        calcGiB(mem);
        pthread_mutex_lock(&st->lock);
        st->mem_used_gib = mem->usedGiB;
        st->mem_total_gib = mem->totalGiB;
        pthread_mutex_unlock(&st->lock);
    }
    free(mem);
    return NULL;
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
    /* Initialise shared state */
    shared_state_t state = {
        .cpu_percent = 0.0,
        .mem_used_gib = 0.0,
        .mem_total_gib = 0.0,
        .usleep_time = usleep_time,
        .quit = 0,
        .lock = PTHREAD_MUTEX_INITIALIZER
    };

    /* Start collector thread */
    pthread_t cpu_thread, mem_thread;
    pthread_create(&cpu_thread, NULL, cpu_collector_thread, &state);
    pthread_create(&mem_thread, NULL, mem_collector_thread, &state);

    init();
    /* Make getch non‑blocking so we can detect a quit key */
    nodelay(stdscr, TRUE);
    while (1) {
        int ch = getch();
        if (ch == 'q' || ch == 'Q' || ch == 27) { // q or ESC to quit
            pthread_mutex_lock(&state.lock);
            state.quit = 1;
            pthread_mutex_unlock(&state.lock);
            break;
        }
        /* Copy shared data under lock */
        pthread_mutex_lock(&state.lock);
        double cpu = state.cpu_percent;
        double used = state.mem_used_gib;
        double total = state.mem_total_gib;
        pthread_mutex_unlock(&state.lock);
        erase();
        int cpu_cols = (int)((cpu / 100.0) * (double)visible_width(BANNER_CPU[0]));
        int mem_cols = total > 0.0 ? (int)((used / total) * (double)visible_width(BANNER_MEMORY[0])) : 0;
        /* ----- CPU ----- */
        print_banner(BANNER_CPU, N_ROW, cpu_cols);
        int cpu_start_y = getcury(stdscr) - N_ROW;
        int cpu_width   = visible_width(BANNER_CPU[0]);
        move(cpu_start_y - 1, cpu_width + 1);
        print_percent((int)cpu);
        move(cpu_start_y + N_ROW, 0);
        /* ----- MEMORY ----- */
        print_banner(BANNER_MEMORY, N_ROW, mem_cols);
        int mem_start_y = getcury(stdscr) - N_ROW;
        int mem_width   = visible_width(BANNER_MEMORY[0]);
        move(mem_start_y - 1, mem_width + 1);
        int mem_percent_int = total > 0.0 ? (int)((used / total) * 100.0) : 0;
        print_percent(mem_percent_int);
        move(mem_start_y + N_ROW, 0);
        /* Show exit hint */
        move(mem_start_y + N_ROW + 1, 0);
        printw("press q or esc to exit...");
        refresh();
        /* Small sleep to avoid busy‑looping too hard */
        usleep(50000);
    }
    /* Wait for collector thread to finish */
    pthread_join(cpu_thread, NULL);
    pthread_join(mem_thread, NULL);
    end();
    return 0;
}



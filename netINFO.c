#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include "netINFO.h"

/*
 * readNET: Populate the NETinfo struct with total received (rx) and transmitted (tx)
 * bytes read from /proc/net/dev. The `before` flag is kept for compatibility but
 * does not affect the current implementation because NETinfo stores only a
 * single snapshot.
 */
void readNET(NETinfo *info, bool before){
    // Populate either the BEFORE (if before==true) or AFTER slot of a two‑element
    // NETinfo array. The caller should provide an array of size 2.
    FILE *f = fopen("/proc/net/dev", "r");
    if (!f) {
        perror("open /proc/net/dev");
        return;
    }
    char line[256];
    // Skip header lines
    if (!fgets(line, sizeof(line), f)) { fclose(f); return; }
    if (!fgets(line, sizeof(line), f)) { fclose(f); return; }

    unsigned long long total_rx = 0, total_tx = 0;
    while (fgets(line, sizeof(line), f)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;
        // Parse numbers after the colon
        unsigned long long rx = 0, tx = 0;
        if (sscanf(colon + 1, " %llu %*llu %*llu %*llu %*llu %*llu %*llu %*llu %llu",
                   &rx, &tx) == 2) {
            total_rx += rx;
            total_tx += tx;
        }
    }
    fclose(f);
    // Store into the appropriate slot
    int idx = before ? BEFORE : AFTER;
    info[idx].rx = (double)total_rx;
    info[idx].tx = (double)total_tx;
}

void becomebeforeNET(NETinfo *info){
    /*
     * Manually copy each field from the AFTER slot into the BEFORE slot.
     * This avoids any accidental pointer aliasing or unintended memory
     * de‑allocation that could happen if the struct contained pointers.
     * The NETinfo struct only holds two double values, so we copy them
     * explicitly.
     */
    info[BEFORE].rx = info[AFTER].rx;
    info[BEFORE].tx = info[AFTER].tx;
}

int netTRAFFIC(NETinfo *info){
    double rx_delta = info[AFTER].rx - info[BEFORE].rx;
    double tx_delta = info[AFTER].tx - info[BEFORE].tx;
    unsigned long long total = (unsigned long long)(rx_delta + tx_delta);
    return (int)total;
}


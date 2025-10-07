#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <errno.h>

struct mydata { uint16_t datamem[4*9]; }; /* 36 x 2 = 72 bytes */

int mydatasize = 128;
void *g_p_datas = NULL;

static char *make_timestamp_filename(void) {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) return NULL;

    struct tm tm;
    if (localtime_r(&ts.tv_sec, &tm) == NULL) return NULL;

    char tbuf[32];
    if (strftime(tbuf, sizeof tbuf, "%Y%m%d_%H%M%S", &tm) == 0) return NULL;

    int ms = (int)(ts.tv_nsec / 1000000);

    /* compute required size, allocate, then write */
    int needed = snprintf(NULL, 0, "%s_%03d.txt", tbuf, ms);
    if (needed < 0) return NULL;
    size_t size = (size_t)needed + 1;
    char *name = malloc(size);
    if (!name) return NULL;
    if (snprintf(name, size, "%s_%03d.txt", tbuf, ms) < 0) {
        free(name);
        return NULL;
    }
    return name;
}

#if 0
static char *make_timestamp_filename(void) {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) return NULL;
    struct tm tm;
    if (localtime_r(&ts.tv_sec, &tm) == NULL) return NULL;
    char buf[64];
    if (strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tm) == 0) return NULL;
    int ms = (int)(ts.tv_nsec / 1000000);
    char *name = malloc(64);
    if (!name) return NULL;
    snprintf(name, 64, "%s_%03d.txt", buf, ms);
    return name;
}
#endif

/* Dump every 4 bytes as one hex line "0x12345678\n". Returns 0 on success. */
int dump_all_data_to_file(void) {
    if (!g_p_datas || mydatasize <= 0) {
        fprintf(stderr, "No data or invalid size\n");
        return -1;
    }

    char *filename = make_timestamp_filename();
    if (!filename) {
        fprintf(stderr, "Failed to create filename\n");
        return -1;
    }

    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "fopen(%s) failed: %s\n", filename, strerror(errno));
        free(filename);
        return -1;
    }

    const size_t elem_size = sizeof(struct mydata);
    const unsigned char *base = (const unsigned char *)g_p_datas;

    for (int i = 0; i < mydatasize; ++i) {
        const unsigned char *elem = base + (size_t)i * elem_size;
        size_t remaining = elem_size;
        size_t offset = 0;

        while (remaining > 0) {
            uint32_t word = 0;
            /* copy up to 4 bytes into word in memory order */
            for (size_t b = 0; b < 4; ++b) {
                size_t idx = offset + b;
                uint8_t byte = 0;
                if (idx < elem_size) byte = elem[idx];
                word |= (uint32_t)byte << (8 * (3 - b)); /* produce big-endian hex order */
            }
            /* print as 0xhhhhhhhh lowercase */
            if (fprintf(f, "0x%08x\n", word) < 0) {
                fprintf(stderr, "Write error at element %d offset %zu: %s\n", i, offset, strerror(errno));
                fclose(f);
                free(filename);
                return -1;
            }
            offset += 4;
            if (remaining >= 4) remaining -= 4; else remaining = 0;
        }
    }

    fclose(f);
    printf("Wrote %d elements (each %zu bytes) to %s\n", mydatasize, elem_size, filename);
    free(filename);
    return 0;
}

/* Example usage */
int main(void) {
    g_p_datas = malloc((size_t)mydatasize * sizeof(struct mydata));
    if (!g_p_datas) { perror("malloc"); return 1; }

    /* sample fill: unique bytes per element/offset */
    for (int i = 0; i < mydatasize; ++i) {
        unsigned char *p = (unsigned char *)g_p_datas + (size_t)i * sizeof(struct mydata);
        for (size_t j = 0; j < sizeof(struct mydata); ++j)
            p[j] = (unsigned char)((i + j) & 0xff);
    }

    if (dump_all_data_to_file() != 0) {
        fprintf(stderr, "dump failed\n");
        free(g_p_datas);
        return 2;
    }

    free(g_p_datas);
    return 0;
}

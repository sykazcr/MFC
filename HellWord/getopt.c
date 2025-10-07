// myapp.c
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s [--verbose] [--timer_s N] [--timer_ms M]\n"
        "  --verbose           enable verbose output\n"
        "  --timer_s N         set timer seconds (non-negative integer)\n"
        "  --timer_ms M        set timer milliseconds (0-999)\n",
        prog);
    exit(EXIT_FAILURE);
}

static long parse_nonneg_integer(const char *s, const char *name) {
    errno = 0;
    char *end;
    long val = strtol(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0' || val < 0) {
        fprintf(stderr, "Invalid value for %s: %s\n", name, s);
        usage("myapp");
    }
    return val;
}

int main(int argc, char **argv) {
    bool verbose = false;
    long timer_s = 0;
    long timer_ms = 0;

    static struct option longopts[] = {
        {"verbose",   no_argument,       0, 'v'},
        {"timer_s",   required_argument, 0, 's'},
        {"timer_ms",  required_argument, 0, 'm'},
        {0,0,0,0}
    };

    int opt;
    int idx;
    while ((opt = getopt_long(argc, argv, "vs:m:", longopts, &idx)) != -1) {
        switch (opt) {
        case 'v':
            verbose = true;
            break;
        case 's':
            timer_s = parse_nonneg_integer(optarg, "--timer_s");
            break;
        case 'm':
            timer_ms = parse_nonneg_integer(optarg, "--timer_ms");
            if (timer_ms < 0 || timer_ms > 999) {
                fprintf(stderr, "--timer_ms must be between 0 and 999\n");
                usage("myapp");
            }
            break;
        default:
            usage("myapp");
        }
    }

    // Combine into total milliseconds, check for overflow
    const long MS_PER_SEC = 1000;
    long long total_ms = (long long)timer_s * MS_PER_SEC + (long long)timer_ms;
    if (total_ms < 0 || total_ms > INT64_MAX) {
        fprintf(stderr, "Timer value overflow\n");
        return EXIT_FAILURE;
    }

    if (verbose) {
        printf("Verbose: ON\n");
        printf("Timer: %ld s, %ld ms -> total %lld ms\n", timer_s, timer_ms, total_ms);
    } else {
        printf("Timer set to %lld ms\n", total_ms);
    }

    // Example usage: sleep for the specified duration (rounded to seconds)
    // Remove or replace with your app logic.
    // Here we just print and exit.
    return EXIT_SUCCESS;
}
/*
Compile: gcc -std=c11 -Wall -Wextra -o myapp myapp.c
	Examples:
	./myapp --verbose --timer_s 3 --timer_ms 10
	./myapp --timer_s 1
	./myapp --timer_ms 250
*/

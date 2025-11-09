// CRT
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// POSIX
#include <unistd.h>

const char* argv0;

static void generate_file(
  const int num_count,
  const int max_num,
  const char *out_file)
{
  char tmp_file[L_tmpnam];
  FILE *out;
  if (strcmp(out_file, "-") != 0 && strcmp(out_file, "--") != 0) {
    tmpnam(tmp_file);
    out = fopen(tmp_file, "w");
    if (out == NULL) {
      fprintf(stderr, "%s: can't create %s: %s\n",
              argv0, tmp_file, strerror(errno));
      exit(EXIT_FAILURE);
    }
  } else {
    out = stdout;
    strcpy(tmp_file, "(stdout)");
  }

  const int m = max_num + 1;

  for (int i = 0; i < num_count; ++i) {
    const int n = rand() % m;
    if (fprintf(out, "%d\n", n) > 0) continue;
    fprintf(stderr, "%s: can't write to %s: %s\n",
            argv0, tmp_file, strerror(errno));
    if (out != stdout) {
      fclose(out);
      unlink(tmp_file);
    }
    exit(EXIT_FAILURE);
  }

  if (out != stdout) {
    if (fclose(out) != 0) {
      fprintf(stderr, "%s: can't close %s: %s\n",
              argv0, tmp_file, strerror(errno));
      unlink(tmp_file);
      exit(EXIT_FAILURE);
    }

    if (rename(tmp_file, out_file) != 0) {
      fprintf(stderr, "%s: can't rename %s into %s: %s\n",
              argv0, tmp_file, out_file, strerror(errno));
      unlink(tmp_file);
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char **argv)
{
  argv0 = argv[0];

  if (argc < 4) {
    fprintf(stderr, 
            "%s: missing required arguments.\n"
            "Usage: %s NUMBER_COUNT MAX_NUMBER SEED [OUT_FILE]\n"
            "NUMBER_COUNT must be from 1 to %d\n"
            "MAX_NUMBER must be from 1 to %d\n"
            "SEED must be from 1 to %d (or 'time' to use current time)\n",
            argv0, argv0, INT_MAX, RAND_MAX - 1, INT_MAX
            );
    exit(EXIT_FAILURE);
  }

  const int num_count = atoi(argv[1]);
  if (num_count < 1) {
    fprintf(stderr, "%s: invalid number count: %s\n", argv0, argv[1]);
    exit(EXIT_FAILURE);
  }

  const int max_num = atoi(argv[2]);
  if (max_num < 1 || max_num > RAND_MAX - 1) {
    fprintf(stderr, "%s: invalid max number: %s\n", argv0, argv[2]);
    exit(EXIT_FAILURE);
  }

  const int use_time = strcmp(argv[3], "time") == 0;
  int seed = 0;
  if (!use_time) {
    seed = atoi(argv[3]);
    if (seed == 0 && strcmp(argv[3], "0") != 0) {
      fprintf(stderr, "%s: invalid seed: %s\n", argv0, argv[2]);
      exit(EXIT_FAILURE);
    }
  }

  srand((unsigned)(use_time ? time(NULL) : seed));

  if (argc > 4) {
    for (int i = 4; i < argc; ++i) {
      generate_file(num_count, max_num, argv[i]);
    }
  } else {
    generate_file(num_count, max_num, "-");
  }

  return 0;
}

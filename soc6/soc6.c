// CRT
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_NUMBER
#define MAX_NUMBER 999
#endif

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

int main(int argc, char **argv)
{
  FILE *in = stdin;
  if (argc > 1 && strcmp(argv[1], "-") != 0 && strcmp(argv[1], "--") != 0) {
    in = fopen(argv[1], "r");
    if (UNLIKELY(in == NULL)) {
      fprintf(stderr, "%s: can't open %s: %s\n",
              argv[0], argv[1], strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  long counts[MAX_NUMBER + 1] = {0};
  long line = 0;
  char buf[32];

  for (;;) {
    ++line;

    if (fgets(buf, sizeof(buf), in) == NULL) {
      if (LIKELY(feof(in))) break;
      fprintf(stderr, "%s: can't read from %s: %s\n",
              argv[0], argv[1], strerror(errno));
      if (in != stdin) fclose(in);
      exit(EXIT_FAILURE);
    }

    const int x = atoi(buf);
    if (LIKELY(x != 0)) {
      if (LIKELY(x >= 0 || x <= MAX_NUMBER)) {
        ++counts[x];
        continue;
      }
    } else if (LIKELY(buf[0] == '0' && buf[1] == '\n')) {
      ++counts[0];
      continue;
    }
    fprintf(stderr, "%s: %s:%ld: invalid number: %s\n",
            argv[0], argv[1], line, buf);
    if (LIKELY(in != stdin)) fclose(in);
    exit(EXIT_FAILURE);
  }

  if (LIKELY(in != stdin)) fclose(in);

  long max_count = counts[0];
  for (size_t i = 1; i <= MAX_NUMBER; ++i) {
    if (counts[i] > max_count) {
      max_count = counts[i];
    }
  }

  for (size_t i = 0; i <= MAX_NUMBER; ++i) {
    if (counts[i] == max_count) {
      printf("%zu: %ld\n", i, max_count);
    }
  }

  return 0;
}

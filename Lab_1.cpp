#include <ctype.h>
#include <stdio.h>
#include <openmp/omp.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int i = 0;
  int state = (isspace(argv[1][0]) > 0);
  int answer = 0;
  if (!state)
    answer += 1;
  char cur;
  while (i < strlen(argv[1])) {
    if (isspace(argv[1][i]) && !state) {
      state = 1;
    } else if (!isspace(argv[1][i]) && state) {
      state = 0;
      answer += 1;
    }
    i += 1;
  }
  // for (i = 0; i < strlen(argv[1]); i++) {
  //   answer += (isspace(argv[1][i]) > 0);
  // }

  printf("Num words in %s = %d\n", argv[1], answer);
}
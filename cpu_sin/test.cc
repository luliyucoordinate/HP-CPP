#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
int main() {
  struct timeval tv;
  long long start_time, end_time;
  long long busy_time[100];
  long long idle_time[100];
  int i;
  for (i = 0; i < 100; i++) {
    busy_time[i] = 100000 * 0.5 * (sin(i * 0.0628) + 1);
    idle_time[i] = 100000 - busy_time[i];
  }
  i = 0;
  while (1) {
    gettimeofday(&tv, NULL);
    start_time = tv.tv_sec * 1000000 + tv.tv_usec;
    end_time = start_time;

    while ((end_time - start_time) < busy_time[i]) {
      gettimeofday(&tv, NULL);
      end_time = tv.tv_sec * 1000000 + tv.tv_usec;
    }
    usleep(idle_time[i]);
    i = (i + 1) % 100;
  }
  return 0;
}

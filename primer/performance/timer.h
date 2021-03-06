#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

namespace timer {

static inline long GetMicros() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return static_cast<long>(tv.tv_sec) * 1000000 + tv.tv_usec;
}

static inline int32_t NowTime() {
  return static_cast<int32_t>(GetMicros() / 1000000);
}

static inline int32_t NowTimeStr(char* buf, int32_t len) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  const time_t seconds = tv.tv_sec;
  struct tm t;
  localtime_r(&seconds, &t);
  return snprintf(buf, len, "%02d/%02d %02d:%02d:%02d.%06d",
                  t.tm_mon + 1,
                  t.tm_mday,
                  t.tm_hour,
                  t.tm_min,
                  t.tm_sec,
                  static_cast<int>(tv.tv_usec));
}


class AutoTimer {
  public:
    AutoTimer(double timeout_ms, const char* msg1, const char* msg2 = NULL):
      timeout_(timeout_ms),
      msg1_(msg1),
      msg2_(msg2) {
      start_ = GetMicros();
    }

    ~AutoTimer() {
      long end = GetMicros();
      if (end - start_ > timeout_ * 1000) {
        double t = (end - start_) / 1000.0;
        if (!msg2_) {
          fprintf(stderr, "[AutoTimer] %s use %.3f ms\n", msg1_, t);
        } else {
          fprintf(stderr, "[AutoTimer] %s %s use %.3f ms\n", msg1_, msg2_, t);
        }
      }
    }

  private:
    long start_;
    double timeout_;
    const char* msg1_;
    const char* msg2_;
};

}

#endif

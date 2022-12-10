#include "system_time.h"

String SystemTime::getFormattedDateTime() const {
  time_t rawTime = getEpochTime();
  struct tm *info;
  char buffer[80];
  info = localtime(&rawTime);
  strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
  return buffer;
}

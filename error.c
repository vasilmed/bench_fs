#include "error.h"

void
err_exit(const char* str)
{
  perror(str);
  exit(errno);
}

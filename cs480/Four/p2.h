#include "getword.h"
#include <dirent.h>
#include <errno.h>
#include <features.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXITEM 100
#define PROMPT ":480:"
void parse();

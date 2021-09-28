/* p2.c - implementation of p2.

  Author     : Brandon Nguyen (823630399)
  Course     : CS-480 - Operating Systems
  Instructor : Dr. John Carroll
  Session    : Fall 2021
  School     : San Diego State University
*/

#define TRUE 1
#define FALSE 0
#include "p2.h"
#include "getword.h"
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Shell bitfield flags.
 */
struct {
  unsigned int is_first_run : 1; /* first run of current shell prompt */
  /*
   * Termination flags.
   */
  unsigned int is_eof : 1;           /* EOF encountered */
  unsigned int is_empty_newline : 1; /* empty (newline) input */
  unsigned int is_term : 1;          /* non-empty newline or ; */
  /*
   * Program execution flags. In each shell prompt, this can be at most 1
   * value.
   */
  unsigned int call_cd : 1;  /* run built-in cd */
  unsigned int call_lsf : 1; /* run built-in ls-F */
  unsigned int call_cmd : 1; /* run regular command (fork) */
  /*
   * Extra behavior flags. There can be any combination of the following flags.
   */
  unsigned int bg : 1;                /* background command */
  unsigned int redir_out : 1;         /* redirect stdout */
  unsigned int redir_in : 1;          /* redirect stdin */
  unsigned int multiple_commands : 1; /*multiple commands in single prompt*/
} state;

/*
 * Argument values - n(ew)argv
 * Maximum of MAXITEM words with each word being STORAGE long. Each value is a
 * pointer to a null-terminated "string".
 * Example: [exe, arg0, arg1, arg2, ..., argn]
 */
char *nargv[MAXITEM];
/*
 * Argument count - n(ew)argc
 * Example:
 * nargv = [exe, arg0, arg1]
 * nargc = 3
 */
int nargc = 0;

/*
 * Character buffer separated by null terminators.
 * Example:
 * stdin = ls -l
 * buffer = [l, s, \0, -, l, \0]
 */
char buffer[STORAGE];

/* void spipe() { int filedes[2]; } */
void redirect(char *ofile, char *input) {
  int flags = O_CREAT | O_EXCL | O_WRONLY;
  int output_fd = open(ofile, flags, S_IRUSR | S_IWUSR);
  if (output_fd < 0) {
    fprintf(stderr, "%s: file exists\n", ofile);
    return;
  }

  if (close(output_fd) != 0) {
    fprintf(stderr, "%s: unable to close file\n", ofile);
  }
}

/*
  Shell built-in handler for `cd`. Allows the user to change directories
  specified in `nargv`.

  INPUT: a pointer to the beginning of a character array with the search
  location

  OUTPUT: On successful completion, a value of 0 is returned. Otherwise, -1 is
  returned.
*/
int cd() {
  int cd_status;
  // FIXME: remove
  char cwd[1000];
  if (nargc > 2) {
    perror("cd: Too many arguments\n");
    return -1;
  }

  cd_status = (nargc == 1) ? chdir(getenv("HOME")) : chdir(nargv[1]);

  if (cd_status == -1) {
    fprintf(stderr, "cd: Cannot access '%s'\n", nargv[1]);
  }

  // FIXME: remove
  printf("DEBUG: now in %s\n", getcwd(cwd, sizeof(cwd)));
  return cd_status;
}

/*
  Shell built-in handler for `ls-F`. This performs similarly to `ls` found in
  `/bin/ls` *without* forking.

  If no argument is specified (`location` is empty), the command will be
  executed on the current directory. Otherwise, the specified location will be
  searched. If a file is specified, the filename will be parroted back.

  INPUT: a pointer to the beginning of a character array with the search
  location

  OUTPUT: status code of the command
*/
int lsF() {
  DIR *dirp;
  struct dirent *dp;

  if (nargc == 1) {
    dirp = opendir(".");
  } else {
    struct stat sb;
    int fstatus = stat(nargv[1], &sb);
    // The given file/folder name does not exist (or another error was raised).
    if (fstatus == -1) {
      fprintf(stderr, "'%s': No such file or directory.\n", nargv[1]);
      return -1;
    } else {
      // Determine if the given filename is a directory or not
      // https://stackoverflow.com/a/3828537
      if (S_ISDIR(sb.st_mode)) {
        dirp = opendir(nargv[1]);
      } else {
        // Parrot back the filename and return
        printf("%s\n", nargv[1]);
        return 0;
      }
    }
  }

  // If filename cannot be accessed or cannot malloc(3) enough memory.
  if (dirp == NULL) {
    fprintf(stderr, "%s unreadable\n", nargv[1]);
    return -1;
  }

  while (dirp) {
    // Continue looping through directories as long as a pointer exists
    if ((dp = readdir(dirp)) != NULL) {
      printf("%s\n", dp->d_name);
    } else {
      closedir(dirp);
      break;
    }
  }

  return 0;
}

/*
 *
 *
 */
void cmd() {
  pid_t kidpid;

  nargv[nargc] = NULL;
  /* flush stdout/stderr to prevent children from printing prompt */
  /* p2 < input11 > myout to test */
  fflush(stdout);
  fflush(stderr);
  kidpid = fork();
  if (kidpid == -1) {
    perror("cannot fork\n");
    exit(EXIT_FAILURE);
  } else if (kidpid == 0) {
    if (execvp(nargv[0], nargv) == -1) {
      fprintf(stderr, "%s: Command not found.\n", nargv[0]);
    }
    exit(EXIT_SUCCESS);
  } else {
    (void)wait(&kidpid);
  }
}

void sighandler(int signum) { state.is_eof = TRUE; }

/*
 * Primary shell handler. Calls other functions to do the dirty work.
 */
int main(void) {
  /* signal catcher */
  (void)signal(SIGTERM, sighandler);

  /* Set the process group to the current pid */
  if (setpgid(0, getpgrp()) != 0) {
    perror("Error setting process group, exiting.");
    exit(1);
  }

  printf("DEBUG: PGRP: %d\n", getpgrp());
  while (!state.is_eof) {
    /* reset bitfield */
    memset(&state, FALSE, sizeof(state));
    state.is_first_run = TRUE;
    /* nargc = 0; */
    /* display prompt at beginning of line */
    printf("%s ", PROMPT);

    while (state.is_first_run || state.multiple_commands) {
      nargc = 0;
      state.multiple_commands = FALSE;
      parse();

      // Terminate end of args.
      nargv[nargc] = NULL;

      /* handle parsed flags */
      if (state.is_empty_newline) {
        continue;
      }

      if (state.bg) {
        printf("DEBUG: backgrounding\n");
      } else if (state.redir_out) {
        printf("DEBUG: redirecting stdout to %s\n", nargv[nargc - 1]);
        redirect(nargv[nargc - 1], "test!");
        /* decrement `nargc` since the last value was the redirect file name */
        nargc -= 2;
        nargv[nargc] = NULL;
      }

      if (state.call_cd) {
        cd();
      } else if (state.call_lsf) {
        lsF();
      } else if (state.call_cmd) {
        cmd();
      }
    }
  }
  /* terminate any children that are still running */
  killpg(getpgrp(), SIGTERM);
  printf("p2 terminated.\n");
  exit(0);
}

/*
 * The parse() function handles all of the syntactical analysis for the shell.
 * parse() will set the `nargc`, `nargv`, and `status` variables to pass around
 * useful information. parse() works by calling the `getword()` function
 * repeatedly. It executes no commands on its own, its only job is to set
 * information.
 *
 * INPUT: none
 *
 * OUTPUT: none
 *
 * SIDE EFFECTS: the variables `nargc` and `nargv` will be overwritten. Flags
 * will be overwritten in `status`.
 */
void parse() {
  /*
   * firstRun determines whether or not the first word of `parse` is an
   * executable or not.
   */
  int char_width;
  int total_len;
  total_len = 0;
  for (;;) {
    char_width = getword(&buffer[total_len]);
    nargv[nargc] = &buffer[total_len];
    /* add one to include the null terminator */
    total_len += char_width + 1;

    if (char_width == -1) {
      if (state.is_first_run) {
        state.is_eof = TRUE;
      }
      break;
    } else if (char_width == 0) {
      nargv[nargc] = ";";
      if (state.is_first_run) {
        state.is_empty_newline = TRUE;
        break;
      } else {
        state.is_first_run = FALSE;
        state.multiple_commands = TRUE;
        break;
      }
      /* Technically could be `else` but be explicit. */
    } else if (char_width > 0) {
      if (state.is_first_run) {
        if (strcmp(buffer, "cd") == 0) {
          state.call_cd = TRUE;
        } else if (strcmp(buffer, "ls-F") == 0) {
          state.call_lsf = TRUE;
        } else {
          state.call_cmd = TRUE;
        }
      }

      /*
       * Immediately terminate if we are redirecting - the last arg is now the
       * redirect file.
       */
      if (state.redir_out) {
        nargc++;
        break;
      }

      /*
       * We don't want to copy any arguments if we have a signal to background
       * or redirect our output since we need to have special behavior.
       * Therefore, we just continue parsing while setting the proper flag.
       */
      if (strcmp(buffer, "&") == 0) {
        state.bg = TRUE;
        break;
      } else if (strcmp(buffer, ">") == 0) {
        state.redir_out = FALSE;
      } else if (strcmp(buffer, "<") == 0) {
        state.redir_in = FALSE;
      }

      nargc++;
    }
    /*
     * After the first loop, reset firstRun so everything else is handled as
     * arguments.
     */
    state.is_first_run = 0;
  }
}

/* p2.c - implementation of p2.
 *
 *   Author     : Brandon Nguyen (823630399)
 *   Course     : CS-480 - Operating Systems
 *   Instructor : Dr. John Carroll
 *   Session    : Fall 2021
 *   School     : San Diego State University
 */

#include "p2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Shell bitfield flags.
 */
struct {
  /*
   * Termination flags
   */
  unsigned int complete : 1;         /* terminate shell */
  unsigned int is_empty_newline : 1; /* empty (newline) input */
  unsigned int is_term : 1;          /* non-empty newline or ; */

  /*
   * Syntax errors
   */
  unsigned int syntax_max_item : 1;      /* overflowed maximum item count */
  unsigned int syntax_pipe : 1;          /* more than one pipe exists */
  unsigned int syntax_in_redirect : 1;   /* ambiguous input redirects */
  unsigned int syntax_out_redirect : 1;  /* ambiguous output redirects */
  unsigned int syntax_missing_quote : 1; /* missing matching quotes */
  unsigned int syntax_invalid_redir_target : 1; /* bad redirect target (&) */

  /*
   * Program execution flags. In each shell prompt, this can be at most ONE 1
   * value.
   */
  unsigned int call_cd : 1;  /* run built-in cd */
  unsigned int call_lsf : 1; /* run built-in ls-F */
  unsigned int call_cmd : 1; /* run regular command (fork) */

  /*
   * Extra behavior flags. There can be any combination of the following flags.
   */
  unsigned int bg : 1;        /* background command */
  unsigned int redir_out : 1; /* redirect stdout */
  unsigned int redir_in : 1;  /* redirect stdin */
  unsigned int pipe : 1;      /* pipe command output */
} state;

/*
 * Argument values - n(ew)argv
 * Maximum of MAXITEM words with each word being STORAGE long. Each value is a
 * pointer to a null-terminated "string". Pointers are read consecutively until
 * the value in nargc is reached - there may be dangling pointers after
 * nargv[nargc].
 *
 * Example:
 * stdin = cat < input > output
 * buffer = [c, a, t, \0, <, \0, i, n, p, u, t, \0, >, \0, o, u, t, p, u, t, \0]
 * nargv = [*c]
 * nargc = 1
 */
char *nargv[MAXITEM];

/*
 * Argument count - n(ew)argc
 *
 * Example:
 * stdin = ls -l -a
 * buffer = [l, s, \0, -, l, \0, -, a, \0]
 * nargv = [*l, *-, *-]
 * nargc = 3
 */
int nargc = 0;

/*
 * Word buffer separated by null terminators.
 *
 * Example:
 * stdin = ls -l
 * buffer = [l, s, \0, -, l, \0]
 * nargv = [*l, *-]
 * nargc = 2
 */
char buffer[STORAGE * STORAGE];

/* Output file for redirection (>). Only read when state.redir_out is 1. */
char *redir_out_target = NULL;

/* Input file for redirection (<). Only read when state.redir_in is 1. */
char *redir_in_target = NULL;

/*
 * Pipe argument values - p(ipe)argv
 * Similar to nargv, pargv contains the argument values for the piped (RHS)
 * command. See nargv for implementation and usage details.
 */
char *pargv[MAXITEM];

/*
 * Pipe argument count - p(ipe)argc
 * Similar to nargc, pargc contains the argument count for the piped (RHS)
 * command. See nargc for implementation and usage details.
 */
int pargc = 0;

/*
  Shell built-in handler for `cd`. Allows the user to change directories
  specified in `nargv`. `cd` only accepts 1 argument.

  OUTPUT: On successful completion, an integer 0 is returned. Otherwise, -1 is
  returned.
*/
int cd() {
  int cd_status;
  if (nargc > 2) {
    fprintf(stderr, "cd: Too many arguments.\n");
    return -1;
  }

  cd_status = (nargc == 1) ? chdir(getenv("HOME")) : chdir(nargv[1]);

  if (cd_status == -1) {
    fprintf(stderr, "cd: Cannot access '%s'\n", nargv[1]);
  }

  return cd_status;
}

/*
  Shell built-in handler for `ls-F`. This performs similarly to `ls` found in
  `/bin/ls` *without* forking.

  If no argument is specified (`location` is empty), the command will be
  executed on the current directory. Otherwise, the specified location will be
  searched. If a file is specified, the filename will be parroted back.

  If an invalid location is specified, `ls-F` will print the specific error.

  OUTPUT: On successful completion, an integer 0 is returned. Otherwise, -1 is
  returned.
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
      fprintf(stderr, "%s: No such file or directory.\n", nargv[1]);
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
    fprintf(stderr, "%s unreadable.\n", nargv[1]);
    return -1;
  }

  for (;;) {
    // Continue looping through files as long as a pointer exists
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
 * Signal Handler - terminates shell on EOF (<C-d>).
 */
void catch_term(int signum) { state.complete = 1; }

/*
 * Primary shell handler. Reads and coordinates functions according to flags
 * inside `state`.
 */
int main(void) {
  /* Establish signal handler for SIGTERM. */
  (void)signal(SIGTERM, catch_term);

  /* Set the process group to the current pid */
  if (setpgid(0, 0) != 0) {
    perror("Error setting process group, exiting.");
    exit(EXIT_FAILURE);
  }

  while (!state.complete) {
    /* Save copies of stdin/stdout to restore later on */
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);

    /* reset bitfield */
    (void)memset(&state, 0, sizeof(state));

    /* reset variables */
    nargc = 0;
    pargc = 0;
    redir_in_target = NULL;
    redir_out_target = NULL;

    /* issue prompt */
    printf("%s ", PROMPT);

    parse();

    /* early termination/syntax error cases */
    if (state.syntax_pipe) {
      fprintf(stderr, "Too many pipes.\n");
      continue;
    } else if (state.syntax_in_redirect) {
      fprintf(stderr, "Ambiguous input redirection.\n");
      continue;
    } else if (state.syntax_out_redirect) {
      fprintf(stderr, "Ambiguous output redirection.\n");
      continue;
    } else if (state.syntax_missing_quote) {
      fprintf(stderr, "Missing end quote.\n");
      continue;
    } else if (state.syntax_invalid_redir_target) {
      fprintf(stderr, "Invalid redirection target.\n");
      continue;
    } else if (state.redir_in && redir_in_target == NULL) {
      fprintf(stderr, "Invalid input redirection target.\n");
      continue;
    } else if (state.redir_out && redir_out_target == NULL) {
      fprintf(stderr, "Invalid output redirection target.\n");
      continue;
    } else if (state.redir_in && state.call_cmd == 0) {
      fprintf(stderr, "Invalid command supplied for input redirection.\n");
      continue;
    } else if (state.redir_out && state.call_cmd == 0) {
      fprintf(stderr, "Invalid command supplied for output redirection.\n");
      continue;
    } else if (state.pipe && pargc == 0) {
      fprintf(stderr, "No command for piping specified.\n");
      continue;
    } else if (state.is_empty_newline) {
      continue;
    }

    /* built-ins or fork */
    if (state.call_cd) {
      cd();
    } else if (state.call_lsf) {
      lsF();
      /* do some kind of forking */
    } else if (state.call_cmd) {
      pid_t childpid, grandchildpid;
      /* file descriptors for input/output redirection */
      int fd;

      /* input redirection */
      fd = (redir_in_target == NULL) ? open("/dev/null", O_RDONLY)
                                     : open(redir_in_target, O_RDONLY);
      if (fd < 0) {
        fprintf(stderr, "%s: No such file or directory.\n", redir_in_target);
        continue;
      } else if (dup2(fd, STDIN_FILENO) < 0 || close(fd) < 0) {
        fprintf(stderr, "%s: Error redirecting input.\n", redir_in_target);
        exit(EXIT_FAILURE);
      }

      /* output redirection */
      if (redir_out_target != NULL) {
        /* fail if file already exists */
        int wrflags = O_WRONLY | O_CREAT | O_EXCL;
        /* 0644 = -rw-r--r-- */
        int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        fd = open(redir_out_target, wrflags, perms);
        if (fd < 0) {
          fprintf(stderr, "%s: File exists.\n", redir_out_target);
          close(fd);
          continue;
        } else if (dup2(fd, STDOUT_FILENO) < 0 || close(fd) < 0) {
          fprintf(stderr, "%s: Error redirecting output.\n", redir_out_target);
          exit(EXIT_FAILURE);
        }
      }

      /* null terminate end of arguments */
      nargv[nargc] = NULL;

      /* begin forking children */
      if (state.pipe) {
        /* piping */
        grandchildpid = fork(); /* RHS of pipe */
        if (grandchildpid == -1) {
          /* parent */
          fprintf(stderr, "%s: Cannot fork read pipe end.\n", pargv[0]);
          exit(EXIT_FAILURE);
        } else if (grandchildpid == 0) {
          /* grandchild */
          int fildes[2];
          pipe(fildes);

          childpid = fork();
          if (childpid == -1) {
            /* grandchild */
            fprintf(stderr, "%s: Cannot fork write pipe end.\n", nargv[0]);
            exit(EXIT_FAILURE);
          } else if (childpid == 0) {
            /* child */
            if (dup2(fildes[1], STDOUT_FILENO) < 0 || close(fildes[0]) < 0 ||
                close(fildes[1]) < 0) {
              perror("Error piping.\n");
              exit(EXIT_FAILURE);
            }
            if (execvp(nargv[0], nargv) == -1) {
              fprintf(stderr, "%s: Command not found.\n", nargv[0]);
              exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
          }

          if (dup2(fildes[0], STDIN_FILENO) < 0 || close(fildes[0]) < 0 ||
              close(fildes[1]) < 0) {
            perror("Error piping.\n");
            exit(EXIT_FAILURE);
          }

          if (execvp(pargv[0], pargv) == -1) {
            fprintf(stderr, "%s: Command not found.\n", pargv[0]);
            exit(EXIT_FAILURE);
          }

          if (close(fildes[0]) < 0 || close(fildes[1]) < 0) {
            perror("Error piping.\n");
            exit(EXIT_FAILURE);
          };
          exit(EXIT_SUCCESS);
        }
      } else {
        /* no piping */
        childpid = fork();

        if (childpid == -1) {
          /* parent */
          fprintf(stderr, "%s: Cannot fork child.\n", nargv[0]);
          exit(EXIT_FAILURE);
        } else if (childpid == 0) {
          /* child */
          if (execvp(nargv[0], nargv) == -1) {
            fprintf(stderr, "%s: Command not found.\n", nargv[0]);
            exit(EXIT_FAILURE);
          }
          exit(EXIT_SUCCESS);
        }
      }
      /* parent */
      /* restore original stdin/stdout since they are generally overwritten */
      if (dup2(original_stdin, STDIN_FILENO) < 0 || close(original_stdin) < 0) {
        perror("Unable to recover stdin.\n");
        exit(EXIT_FAILURE);
      }

      if (dup2(original_stdout, STDOUT_FILENO) < 0 ||
          close(original_stdout) < 0) {
        perror("Unable to recover stdout.\n");
        exit(EXIT_FAILURE);
      }

      if (state.bg) {
        printf("%s [%d]\n", nargv[0], childpid);
      } else if (state.pipe) {
        /* waiting for RHS of pipe */
        while (wait(NULL) != grandchildpid) {
          printf("%d\n", grandchildpid);
          /* no-op */
        }
      } else {
        while (wait(NULL) != childpid) {
          /* no-op */
        }
      }
    }
  }

  /* terminate any children that are still running under the same pgrp */
  killpg(getpgrp(), SIGTERM);
  printf("p2 terminated.\n");
  return EXIT_SUCCESS;
}

/*
 * The parse() function handles all the syntactical analysis for the shell.
 * parse() will set the `nargc`, `nargv`, and `state` variables to pass around
 * useful information. parse() works by calling the `getword()` function
 * repeatedly. It executes no commands on its own, its only job is to set
 * information.
 *
 * SIDE EFFECTS: the variables `nargc` and `nargv` will be overwritten. Flags
 * will be overwritten in `state`.
 */
void parse() {
  /* buffer size */
  int word_width;
  /* current location in buffer */
  size_t buffer_idx = 0;
  /* helper variable with the current value saved in the buffer */
  char *current_buffer;
  /* determine whether to save the current buffer input */
  int save_current;

  int firstrun = 1;

  for (;;) {
    save_current = 1;
    word_width = getword(&buffer[buffer_idx]);
    current_buffer = &buffer[buffer_idx];

    if (word_width == -2) {
      state.syntax_missing_quote = 1;
      break;
    } else if (word_width == -1) {
      state.complete = 1;
      break;
      /* term chars encountered: \n or ;*/
    } else if (word_width == 0) {
      if (firstrun) {
        state.is_empty_newline = 1;
      }
      break;
    } else if (word_width > 0) {
      if (state.redir_in && redir_in_target == NULL) {
        if (strcmp(current_buffer, "&") == 0) {
          state.syntax_invalid_redir_target = 1;
        } else {
          redir_in_target = current_buffer;
          save_current = 0;
        }
      } else if (state.redir_out && redir_out_target == NULL) {
        if (strcmp(current_buffer, "&") == 0) {
          state.syntax_invalid_redir_target = 1;
        } else {
          redir_out_target = current_buffer;
          save_current = 0;
        }
      }
      /*
       * We don't want to copy any arguments if we have a signal to background
       * or redirect our output since we need to have special behavior.
       * Therefore, we just continue parsing while setting the proper flag.
       *
       * If we encounter multiple redirects or pipes, we want to continue
       * parsing to "gobble" up the remaining characters.
       */
      else if (strcmp(current_buffer, "&") == 0) {
        state.bg = 1;
        break;
      } else if (strcmp(current_buffer, ">") == 0) {
        /* don't handle multiple output redirects, return a syntax error */
        if (state.redir_out) {
          state.syntax_out_redirect = 1;
        } else {
          state.redir_out = 1;
        }
        save_current = 0;
      } else if (strcmp(current_buffer, "<") == 0) {
        /* don't handle multiple input redirects, return a syntax error */
        if (state.redir_in) {
          state.syntax_in_redirect = 1;
        } else {
          state.redir_in = 1;
        }
        save_current = 0;
      } else if (strcmp(current_buffer, "|") == 0) {
        /* don't handle multiple pipes, return a syntax error */
        if (state.pipe) {
          state.syntax_pipe = 1;
        } else {
          state.pipe = 1;
        }
        save_current = 0;
      } else if (firstrun) {
        if (strcmp(current_buffer, "cd") == 0) {
          state.call_cd = 1;
        } else if (strcmp(current_buffer, "ls-F") == 0) {
          state.call_lsf = 1;
        } else {
          state.call_cmd = 1;
        }
        firstrun = 0;
      }
    }

    if (save_current) {
      if (state.pipe) {
        pargv[pargc] = current_buffer;
        pargc++;
      } else {
        nargv[nargc] = current_buffer;
        nargc++;
      }
    }

    /* add 1 to include the null character */
    buffer_idx += word_width + 1;
  }
}

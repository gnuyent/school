/* p2.c - implementation of p2.
 *
 *   Author     : Brandon Nguyen (823630399)
 *   Course     : CS-480 - Operating Systems
 *   Instructor : Dr. John Carroll
 *   Session    : Fall 2021
 *   School     : San Diego State University
 *
 * Our shell begins inside the `main()` function which coordinates what and how
 * things should be called according to the flags set inside the `state`
 * bitfield. Processing input begins in the `parse()` function. Generally, we
 * store input from `stdin` inside `buffer` (char array) where each "word" is
 * null-terminated. We use pointers inside `nargv` to represent the beginning of
 * each argument inside `buffer`. `nargv` is then read by whatever command (e.g.
 * cd or ls-F) up to the number specified in `nargc`. ALL characters (including
 * pipes and redirection symbols) are stored inside `buffer`, it is the
 * responsibility of `parse()` to not include pointers to these characters
 * inside `nargv`.
 */

#include "p2.h"
#include "getword.h"
#include <dirent.h>
#include <errno.h>
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
  /*
   * Termination flags
   */
  unsigned int complete : 1;         /* terminate shell */
  unsigned int is_empty_newline : 1; /* empty (newline) input */
  unsigned int is_term : 1;          /* non-empty newline or ; */

  /*
   * Syntax errors
   */
  unsigned int syntax_max_item : 1;     /* overflowed maximum item count */
  unsigned int syntax_pipe : 1;         /* more than one pipe exists */
  unsigned int syntax_in_redirect : 1;  /* ambiguous input redirects */
  unsigned int syntax_out_redirect : 1; /* ambiguous output redirects */

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

char *pargv[MAXITEM];
int pargc = 0;

/*
 * Pipe handler.
 */
void spipe() {
  int fildes[2];
  pid_t first, second;
  int status;

  pipe(fildes);

  first = fork();
  if (first == 0) {
    dup2(fildes[1], STDOUT_FILENO);
    close(fildes[0]);
    close(fildes[1]);
    execvp(nargv[0], nargv);
  }

  second = fork();
  if (second == 0) {
    dup2(fildes[0], STDIN_FILENO);
    close(fildes[0]);
    close(fildes[1]);
    execvp(pargv[0], pargv);
  }

  close(fildes[0]);
  close(fildes[1]);

  for (;;) {
    pid_t pid;
    pid = wait(NULL);
    if (pid == second) {
      break;
    }
  }
}

/*
 * Redirection handler for [in|out]put.
 *
 * INPUT: Pointer to the beginning of a character array for the INPUT file and
 * OUTPUT file. These pointers can be NULL, meaning that the respective
 * redirection will be skipped.
 *
 * OUTPUT: On successful completion, an integer 0 is returned. Otherwise, -1 is
 * returned.
 */
int redirect(char *input, char *output) {
  /* handle stdin redirects */
  if (input != NULL) {
    int roflags = O_RDONLY;
    int redir_in_target_fd = open(input, roflags);
    if (redir_in_target_fd < 0) {
      fprintf(stderr, "%s: No such file or directory.\n", output);
      return -1;
    } else if (dup2(redir_in_target_fd, STDIN_FILENO) < 0) {
      fprintf(stderr, "%s: Error redirecting input\n", input);
      return -1;
    }
    close(redir_in_target_fd);
  }

  /* handle stdout redirects */
  if (output != NULL) {
    int wrflags = O_WRONLY | O_CREAT | O_EXCL;
    /* 0644 = -rw-r--r-- */
    int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int redir_out_target_fd = open(output, wrflags, perms);
    if (redir_out_target_fd == -1) {
      fprintf(stderr, "%s: File exists.\n", output);
      state.redir_out = 0;
      return -1;
    } else {
      if (dup2(redir_out_target_fd, STDOUT_FILENO) < 0) {
        fprintf(stderr, "%s: Error redirecting output.\n", output);
        return -1;
      }
    }
    close(redir_out_target_fd);
  }
  return 0;
}

/*
 * `restore_std()` restores the original file descriptor for std[in|out] based
 * on the current state. It is called if std[in|out] was overwritten (e.g. by
 * `redirect()`). On BSD, close(2) is redundant.
 *
 *  INPUT: File descriptors for the original stdin and stdout.
 */
void restore_std(int original_stdin, int original_stdout) {
  if (state.redir_in || state.pipe) {
    if (dup2(original_stdin, STDIN_FILENO) == -1) {
      perror("Unable to close redirected input stream. Exiting.\n");
      exit(EXIT_FAILURE);
    }
    close(original_stdin);
  }

  if (state.redir_out || state.pipe) {
    if (dup2(original_stdout, STDOUT_FILENO) == -1) {
      perror("Unable to close redirected output stream. Exiting.\n");
      exit(EXIT_FAILURE);
    }
    close(original_stdout);
  }
}

/*
  Shell built-in handler for `cd`. Allows the user to change directories
  specified in `nargv`. `cd` only accepts 1 argument.

  INPUT: A pointer to the beginning of a character array with the search
  location

  OUTPUT: On successful completion, an integer 0 is returned. Otherwise, -1 is
  returned.
*/
int cd() {
  int cd_status;
  if (nargc > 2) {
    perror("cd: Too many arguments\n");
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
    fprintf(stderr, "%s unreadable\n", nargv[1]);
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
 * Executable handler for non-built-in commands. Forks a new process which runs
 * the command inside nargv[0]. nargv[1..] represents the arguments passed to
 * the executable.
 */
void cmd() {
  pid_t kidpid;
  int devnull;

  /* null terminate end of arguments */
  nargv[nargc] = NULL;
  devnull = open("/dev/null", O_WRONLY);
  kidpid = fork();
  if (kidpid == -1) {
    /* parent */
    perror("cannot fork\n");
    exit(EXIT_FAILURE);
  } else if (kidpid == 0) {
    /* child */
    /* retrieve input from /dev/null */
    dup2(devnull, STDIN_FILENO);
    if (execvp(nargv[0], nargv) == -1) {
      /* flush stdout/stderr to prevent children from printing prompt */
      fflush(stdout);
      fflush(stderr);
      fprintf(stderr, "%s: Command not found.\n", nargv[0]);
      exit(9);
    }
    exit(EXIT_SUCCESS);
  }
  /* parent */
  /* wait for children to go to bed permanently */
  while (wait(NULL) != -1)
    ;
  if (close(devnull) == -1) {
    perror("Unable to close /dev/null stdin override. Exiting.\n");
    exit(EXIT_FAILURE);
  }
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
  if (setpgid(0, getpgrp()) != 0) {
    perror("Error setting process group, exiting.");
    exit(EXIT_FAILURE);
  }

  while (!state.complete) {
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

    /* if (state.syntax_max_item) { */
    /*   fprintf(stderr, "Too many inputs, truncating command to first %d
     * words.", */
    /*           MAXITEM); */
    /* } */

    if (state.syntax_pipe) {
      perror("Too many pipes.\n");
      continue;
    } else if (state.syntax_in_redirect) {
      perror("Ambiguous input redirects.\n");
      continue;
    } else if (state.syntax_out_redirect) {
      perror("Ambiguous output redirects.\n");
      continue;
    }

    /* handle terminating conditions */
    if (state.is_empty_newline) {
      continue;
    }

    if (state.bg) {
      /* create some kind of child */
      printf("NOT IMPLEMENTED: backgrounding\n");
      printf("%s [1]\n", nargv[0]);
    }

    if (state.redir_in || state.redir_out) {
      if (redirect(redir_in_target, redir_out_target) == -1) {
        restore_std(original_stdin, original_stdout);
        continue;
      }
    }

    if (state.pipe) {
      spipe();
    }
    /* built-ins or fork */
    else if (state.call_cd) {
      cd();
    } else if (state.call_lsf) {
      lsF();
    } else if (state.call_cmd) {
      cmd();
    }

    if (state.redir_in || state.redir_out || state.pipe) {
      restore_std(original_stdin, original_stdout);
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

  for (;;) {
    /* if (nargc > MAXITEM) { */
    /*   state.syntax_max_item = 1; */
    /*   break; */
    /* } */
    save_current = 1;
    word_width = getword(&buffer[buffer_idx]);
    current_buffer = &buffer[buffer_idx];

    if (word_width == -1) {
      state.complete = 1;
      break;
      /* term chars encountered: \n or ;*/
    } else if (word_width == 0) {
      if (buffer_idx == 0) {
        state.is_empty_newline = 1;
      }
      break;
    } else if (word_width > 0) {
      if (buffer_idx == 0) {
        if (strcmp(current_buffer, "cd") == 0) {
          state.call_cd = 1;
        } else if (strcmp(current_buffer, "ls-F") == 0) {
          state.call_lsf = 1;
        } else {
          state.call_cmd = 1;
        }
      }
    }

    if (state.redir_in && redir_in_target == NULL) {
      redir_in_target = current_buffer;
      save_current = 0;
    }

    if (state.redir_out && redir_out_target == NULL) {
      redir_out_target = current_buffer;
      save_current = 0;
    }

    /*
     * We don't want to copy any arguments if we have a signal to background
     * or redirect our output since we need to have special behavior.
     * Therefore, we just continue parsing while setting the proper flag.
     *
     * If we encounter multiple redirects or pipes, we want to continue parsing
     * to "gobble" up the remaining characters.
     */
    if (strcmp(current_buffer, "&") == 0) {
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

    /*
     * After the first loop, reset firstRun so everything else is handled as
     * arguments.
     */
    /* state.is_first_run = 0; */
  }
}

/* p2.c - a modified implementation of p2 for Program 4.
 *
 *   Author     : Brandon Nguyen (823630399)
 *   Course     : CS-480 - Operating Systems
 *   Instructor : Dr. John Carroll
 *   Session    : Fall 2021
 *   School     : San Diego State University
 */

#include "p2.h"

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
  unsigned int bg : 1;              /* background command */
  unsigned int redir_out : 1;       /* redirect stdout */
  unsigned int redir_out_force : 1; /* overwrite output file */
  unsigned int redir_in : 1;        /* redirect stdin */
  unsigned int pipe : 1;            /* pipe command output */
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

int command_count;
int pipe_count;
int lsFargs = 0;

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
  struct stat sb;
  int fstatus;
  char folder[STORAGE];

  for (int i = 0; i < nargc; i++) {
    if (nargv[i] == NULL) {
      break;
    } else if (lsFargs == 0) {
      dirp = opendir(".");
      // Retrieve current absolute directory for fancy ls symbols
      // getcwd(folder, STORAGE);
      // dirp = opendir(folder);
    } else {
      fstatus = stat(nargv[1], &sb);
      // The given file/folder name does not exist (or another error was
      // raised).
      if (fstatus == -1) {
        fprintf(stderr, "%s: No such file or directory.\n", nargv[1]);
        return -1;
      } else {
        // Determine if the given filename is a directory or not
        // https://stackoverflow.com/a/3828537
        if (S_ISDIR(sb.st_mode)) {
          dirp = opendir(nargv[1]);
        } else {
          // Parrot back the filename
          printf("%s\n", nargv[1]);
          continue;
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
        // char filename[1024];
        // snprintf(filename, 1024, "%s/%s", nargv[1], dp->d_name);
        // fstatus = lstat(filename, &sb);
        fstatus = lstat(dp->d_name, &sb);
        if (fstatus != 0) {
          // could not read file
          printf("%s\n", dp->d_name);
        } else if (S_ISDIR(sb.st_mode)) {
          printf("%s/\n", dp->d_name);
        } else if (S_ISLNK(sb.st_mode)) {
          fstatus = stat(dp->d_name, &sb);
          if (fstatus != 0) {
            // broken link
            printf("%s&\n", dp->d_name);
          } else {
            // valid link
            printf("%s@\n", dp->d_name);
          }
        } else if (S_ISREG(sb.st_mode)) {
          if (sb.st_mode & S_IXUSR || sb.st_mode & S_IXGRP ||
              sb.st_mode & S_IXOTH) {
            printf("%s*\n", dp->d_name);
          } else {
            printf("%s\n", dp->d_name);
          }
        }
      } else {
        closedir(dirp);
        break;
      }
    }
  }

  return 0;
}

int get_command_pos(int command_num) {
  if (command_num > 0) {
    int curr_command = 1;
    for (int i = 0; i < nargc; i++) {
      if (curr_command == command_num) {
        return i;
      }
      if (nargv[i] == NULL) {
        curr_command++;
      }
    }
  }
  return -1;
}

/*
 * Command runner for user-specified arguments. Handles piping, backgrounding,
 * redirection, and waiting automatically.
 */
void command() {
  pid_t eldestpid;
  pid_t nextpid;
  int fildes[2 * pipe_count];
  int pos;

  // set up pipes
  for (int i = 0; i < pipe_count; i++) {
    pipe(fildes + (i * 2));
  }

  eldestpid = fork();
  if (eldestpid == -1) {
    perror("Error forking.\n");
    exit(EXIT_FAILURE);
  } else if (eldestpid == 0) {
    // printf("\nRoot. pid: %d\n", getpid());
    for (int i = command_count - 1; i > 0; i--) {
      /* replace middle stdout to right command's input */
      if (dup2(fildes[1], STDOUT_FILENO) < 0 || close(fildes[0]) < 0 ||
          close(fildes[1]) < 0) {
        perror("Error piping.\n");
        exit(EXIT_FAILURE);
      }
      pipe(fildes);

      nextpid = fork();
      if (nextpid == -1) {
        perror("Error forking.\n");
        exit(EXIT_FAILURE);
      } else if (nextpid == 0) {
        /* middle */
        // printf("\nChild. pid: %d parent pid: %d\n", getpid(), getppid());
        /* replace middle stdin to left command's output */
        if (dup2(fildes[0], STDIN_FILENO) < 0 || close(fildes[0]) < 0 ||
            close(fildes[1]) < 0) {
          perror("Error piping.\n");
          exit(EXIT_FAILURE);
        }

        /* execute middle command */
        pos = get_command_pos(i);
        if (execvp(nargv[pos], nargv + pos) == -1) {
          fprintf(stderr, "%s: Command not found.\n", nargv[pos]);
          exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
      } else {
        break;
      }
    }
    /* eldest */
    /* replace eldest stdin */
    if (command_count > 1) {
      if (dup2(fildes[0], STDIN_FILENO) < 0 || close(fildes[0]) < 0 ||
          close(fildes[1]) < 0) {
        perror("Error piping.\n");
        exit(EXIT_FAILURE);
      }
    }

    /* replace eldest stdout (to a file) */
    /* output redirection */
    if (redir_out_target != NULL) {
      /* fail if file already exists */
      int wrflags = O_WRONLY | O_CREAT | O_EXCL;
      /* 0644 = -rw-r--r-- */
      int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
      int fd = open(redir_out_target, wrflags, perms);
      if (fd < 0) {
        fprintf(stderr, "%s: File exists.\n", redir_out_target);
        close(fd);
        return;
      } else if (dup2(fd, STDOUT_FILENO) < 0 || close(fd) < 0) {
        fprintf(stderr, "%s: Error redirecting output.\n", redir_out_target);
        exit(EXIT_FAILURE);
      }
    }

    /* execute eldest command */
    pos = get_command_pos(command_count);
    if (execvp(nargv[pos], nargv + pos) == -1) {
      fprintf(stderr, "%s: Command not found.\n", nargv[pos]);
      exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
  }

  while (wait(NULL) != eldestpid) {
    /* no-op */
  }
  // cleanup pipes
  for (int i = 0; i < 2*pipe_count;i++) {
    // ignore errors here, there will be lots...
    close(fildes[i]);
  }
}

/*
 * Signal Handler - terminates shell on EOF (<C-d>).
 */
void catch_term(int signum) { state.complete = 1; }

/*
 * Determines if there is a syntax error.
 */
int syntax_error() {
  /* early termination/syntax error cases */
  if (state.syntax_in_redirect) {
    fprintf(stderr, "Ambiguous input redirection.\n");
  } else if (state.syntax_out_redirect) {
    fprintf(stderr, "Ambiguous output redirection.\n");
  } else if (state.syntax_missing_quote) {
    fprintf(stderr, "Missing end quote.\n");
  } else if (state.syntax_invalid_redir_target) {
    fprintf(stderr, "Invalid redirection target.\n");
  } else if (state.redir_in && redir_in_target == NULL) {
    fprintf(stderr, "Invalid input redirection target.\n");
  } else if (state.redir_out && redir_out_target == NULL) {
    fprintf(stderr, "Invalid output redirection target.\n");
  } else if (state.redir_in && state.call_cmd == 0) {
    fprintf(stderr, "Invalid command supplied for input redirection.\n");
  } else if (state.redir_out && state.call_cmd == 0) {
    fprintf(stderr, "Invalid command supplied for output redirection.\n");
    // } else if (state.pipe && pargc == 0) {
    //   fprintf(stderr, "No command for piping specified.\n");
    //   continue;
  } else if (state.is_empty_newline) {
  } else {
    return 0;
  }

  return 1;
}

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
    /* reset bitfield */
    (void)memset(&state, 0, sizeof(state));

    /* reset variables */
    nargc = 0;
    command_count = 0;
    lsFargs = 0;
    redir_in_target = NULL;
    redir_out_target = NULL;

    /* issue prompt */
    printf("%s ", PROMPT);

    parse();

    /* check syntax errors */
    if (syntax_error()) {
      continue;
    }

    /* built-ins or fork */
    if (state.call_cd) {
      cd();
    } else if (state.call_lsf) {
      lsF();
    } else if (state.call_cmd) {
      command();
    }
  }

  /* terminate any children that are still running under the same pgrp */
  killpg(getpgrp(), SIGTERM);
  printf("p2 terminated.\n");
  return EXIT_SUCCESS;
}

/*
 * The parse() function handles all the syntactical analysis for the shell.
 * parse() will set the `nargc`, `nargv`, and `state` variables to pass
 * around useful information. parse() works by calling the `getword()`
 * function repeatedly. It executes no commands on its own, its only job is
 * to set information.
 *
 * SIDE EFFECTS: the variables `nargc` and `nargv` will be overwritten.
 * Flags will be overwritten in `state`.
 */
void parse() {
  /* buffer size */
  int word_width;
  /* current location in buffer */
  size_t buffer_idx = 0;
  /* helper variable with the current value saved in the buffer */
  char *current_buffer;
  int firstrun = 1;

  for (;;) {
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
      if (firstrun)
        state.is_empty_newline = 1;
      break;
    } else if (word_width > 0) {
      if (state.redir_in && redir_in_target == NULL) {
        /* save the next word as input redirection target */
        if (strcmp(current_buffer, "&") == 0)
          state.syntax_invalid_redir_target = 1;
        else
          redir_in_target = current_buffer;
      } else if ((state.redir_out || state.redir_out_force) &&
                 redir_out_target == NULL) {
        /* save the next word as output redirection target */
        if (strcmp(current_buffer, "&") == 0)
          state.syntax_invalid_redir_target = 1;
        else
          redir_out_target = current_buffer;
      } else if (strcmp(current_buffer, "&") == 0) {
        /* background */
        state.bg = 1;
        break;
      } else if (strcmp(current_buffer, ">") == 0) {
        /* don't handle multiple output redirects, return a syntax error */
        if (state.redir_out || state.redir_out_force)
          state.syntax_out_redirect = 1;
        else
          state.redir_out = 1;
        continue;
      } else if (strcmp(current_buffer, ">!") == 0) {
        /* don't handle multiple output redirects, return a syntax error */
        if (state.redir_out_force || state.redir_out)
          state.syntax_out_redirect = 1;
        else
          state.redir_out_force = 1;
        continue;
      } else if (strcmp(current_buffer, "<") == 0) {
        /* don't handle multiple input redirects, return a syntax error */
        if (state.redir_in)
          state.syntax_in_redirect = 1;
        else
          state.redir_in = 1;
        continue;
      } else if (strcmp(current_buffer, "|") == 0) {
        /* don't handle multiple pipes, return a syntax error */
        state.pipe = 1;
        nargc++;
        command_count++;
        pipe_count++;
      } else if (firstrun) {
        if (strcmp(current_buffer, "cd") == 0)
          state.call_cd = 1;
        else if (strcmp(current_buffer, "ls-F") == 0)
          state.call_lsf = 1;
        else
          state.call_cmd = 1;

        nargv[nargc] = current_buffer;
        nargc++;
        command_count++;
        firstrun = 0;
      } else {
        /* add arguments to nargv */
        nargv[nargc] = current_buffer;
        nargc++;
        if (state.call_lsf && !state.pipe) {
          lsFargs++;
        }
      }
    }

    /* add 1 to include the null character */
    buffer_idx += word_width + 1;
  }
}

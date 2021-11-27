pid_t eldestpid;
pid_t nextpid;
int fildes[2];
int pos;


if (command_count > 1) {
  pipe(fildes);
}

eldestpid = fork();
if (eldestpid == -1) {
  perror("Error forking.\n");
  exit(EXIT_FAILURE);
} else if (eldestpid == 0) {
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

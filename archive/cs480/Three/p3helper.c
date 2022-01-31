/* p3helper.c
   Program 3 CS480 SDSU Fall 2021

   This is the ONLY file you are allowed to change. (In fact, the other
   files should be symbolic links to
     ~cs480/Three/p3main.c
     ~cs480/Three/p3robot.c
     ~cs480/Three/p3.h
     ~cs480/Three/makefile
     ~cs480/Three/CHK.h    )
   */
#include "CHK.h"
#include "p3.h"
#include <math.h>

/* You may put declarations/definitions here.
   In particular, you will probably want access to information
   about the job (for details see the assignment and the documentation
   in p3robot.c):
     */
#define COUNTFILE "countfile"
extern int nrRobots;
extern int quota;
extern int seed;
char semaphoreMutx[SEMNAMESIZE]; // semaphore name
sem_t *pmutx; // guards access to shared data
int fd; // file descriptor for the countfile.
int total_widgets; // total number of widgets (to place) remaining
void cleanup(void);

void initStudentStuff(void) {
  total_widgets = nrRobots * quota;
  sprintf(semaphoreMutx, "%s%ldmutx", COURSEID, (long)getuid());
  pmutx =
      sem_open(semaphoreMutx, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);

  if (SEM_FAILED != pmutx) { // Initialize program 3
    CHK(sem_wait(pmutx));
    // enter critical region
    
    // create and initialize count file
    CHK(fd = open(COUNTFILE, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR));
    CHK(lseek(fd, 0, SEEK_SET));
    CHK(write(fd, &total_widgets, sizeof(total_widgets)));
    CHK(close(fd));
    // Our count file now has the following structure:
    // <overflow remaining> <filled length> <filled remaining>
    //
    // For example, if total_widgets = 12, the file will look like:
    // 2 4 4

    // exit critical region
    CHK(sem_post(pmutx));
  } else { // Not the initializer, assume it's already created
    pmutx = sem_open(semaphoreMutx, 0);
  }
}

void placeWidget(int n) {
  CHK(sem_wait(pmutx));
  // enter critical region

  CHK(fd = open(COUNTFILE, O_RDWR));
  CHK(lseek(fd, 0, SEEK_SET));
  read(fd, &total_widgets, sizeof(total_widgets));

  // Rows remaining
  int rem_row = trunc((sqrt(8 * total_widgets + 1) - 1) / 2);
  // Columns remaining
  int rem_col = total_widgets - ((rem_row * (rem_row + 1)) / 2);

  printeger(n);
  if (rem_col == 1) {
    printf("N\n");
    fflush(stdout);
  } else if (rem_row == 1) {
    printf("F\n");
    fflush(stdout);
    cleanup();
    return;
  }
  total_widgets--;

  CHK(lseek(fd, 0, SEEK_SET));
  write(fd, &total_widgets, sizeof(total_widgets));
  CHK(close(fd));

  // exit critical section
  CHK(sem_post(pmutx));
}

// Deletes COUNTFILE and closes semaphores. This will only be called by the last
// robot.
void cleanup(void) {
  CHK(unlink(COUNTFILE));
  CHK(sem_close(pmutx));
  CHK(sem_unlink(semaphoreMutx));
}

/* getword.c - implementation of the getword() function. 

  Author     : Brandon Nguyen (823630399)
  Course     : CS-480 - Operating Systems
  Instructor : Dr. John Carroll
  Session    : Fall 2021
  School     : San Diego State University

  Documentation graciously retrieved and modified from `~cs480/One/getword.h`.
*/
#include <stdio.h>
#include "getword.h"

/*
  The getword() function gets one "word" from the input stream.

  A "word" is a [maximal] string consisting of anything except space, newline, and EOF.

  INPUT: a pointer to the beginning of a character array

  OUTPUT: -1, 0, or the number of characters in the word

  SIDE EFFECTS: bytes beginning at *w will be overwritten. Anyone using this routine should have w
    pointing to an area with at least STORAGE available bytes before calling getword().
  
  There are some special rules when retrieving a word:
    1. If a repeated character is detected, the word will be "terminated" and return the already
      collected word. The repeated character will be the first input in the next invocation of
      getword().
    2. Backslashes ('\') escape the following character.
    3. Leading spaces are skipped.

  Example:
  If we input the string "   trickier   t\o master\n", we will get 5 outputs:
    1. *w = [trickier] of length 8 - we retrieve the word, ignoring leading spaces
    2. *w = [to] of length 2 - we retrieve the word to with an escaped 'o'
    3. *w = [master] of length 6 - we retrieve the word master
    4. *w = [] of length 0 - we hit newline
    5. *w = [] of length -1 - we hit EOF
*/
int getword(char *w)
{
  int size = 0;
  char lastvisited = '\0';
  char focus = '\0';

  // Immediately reset word array to handle cases where we immediately hit a terminating condition.
  w[0] = '\0';

  for (;;)
  {
    focus = getchar();
    // Terminating conditions
    if (focus == EOF || focus == '\n' || focus == ' ' || focus == lastvisited)
    {
      if (size == 0)
      {
        switch (focus)
        {
        case EOF:
          return -1;
        case '\n':
          return 0;
        // Skip leading spaces
        case ' ':
          continue;
        }
      }
      // Push character back onto stdin so it can be processed on the next run.
      ungetc(focus, stdin);
      // Null terminate end of string.
      // This is not size + 1 because we increment at the end of each word collection iteration.
      w[size] = '\0';
      return size;
    }

    // Escape character handler
    else if (focus == '\\')
    {
      focus = getchar();
      // This is where special escape characters would be handled
    }

    w[size] = focus;
    size += 1;
    // Since this is assigned at the end of iteration, it will always be the value of the previous
    // iteration.
    lastvisited = focus;
  }
}

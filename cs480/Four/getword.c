/* getword.c - implementation of the getword() function.

  Author     : Brandon Nguyen (823630399)
  Course     : CS-480 - Operating Systems
  Instructor : Dr. John Carroll
  Session    : Fall 2021
  School     : San Diego State University

  Documentation graciously retrieved and modified from `~cs480/One/getword.h`.
*/
#include "getword.h"

// lexstate - represents the current state of the lexer
typedef enum lexstate {
  determine_next,     // Determines the type of the next character
  normal_char,        // Normal (non-meta, non-term) characters
  meta_char,          // < > | ; ' &
  term_char,          // EOF \n <space>
  wait_for_end_quote, // Collect words inside quotation marks
  escape_next_char,   // Anything prefixed by '\'
  meta_gt,            // Handler for >!
  pushback_char,      // Signal to push character back to stdin
  end_lex,            // Housekeeping tasks
  missing_end_quote,  // End quote is missing
} lexstate;

/*
  The getword() function gets one "word" from the input stream.
  It returns -1 iff EOF is encountered, otherwise it returns the number of
  characters in the word.

  A "word" is a [maximal] string consisting of a single metacharacter or a
  string made up of non-metacharacters delimited by EOF, newline, blank, or
  metacharacters.

  INPUT: a pointer to the beginning of a character array

  OUTPUT: -2 if there is an unmatched quote, -1 on EOF, or the number of
  characters in the word

  SIDE EFFECTS: bytes beginning at *w will be overwritten. Anyone using this
  routine should have w pointing to an area with at least STORAGE available
  bytes before calling getword().

  What are our edge cases?
    1. Combination metacharacter '>!'
    2. Leading blanks (skipped)
    3. Characters can be escaped with '\'
    4. Newlines following backslashes proceed as normal
    5. Metacharacters can be "sandwiched" with others (e.g. Null>!void)
    6. Quotation marks can have internally escaped quotation marks
    7. Quotation marks can be terminated early with EOF or newline
    8. ';' metacharacter is treated like a newline
    9. If we exceed STORAGE, truncate the output to whatever STORAGE-1 is
*/
int getword(char *w) {
  int size = 0;
  lexstate state = determine_next;
  char focus = '\0';

  // Immediately reset word array to handle cases where we immediately hit a
  // terminating condition.
  w[0] = '\0';

  // STORAGE - 1 so we can put a null-terminating character at the end.
  while (size < STORAGE - 1) {
    switch (state) {
    case determine_next:
      focus = getchar();
      switch (focus) {
      case EOF:
      case '\n':
      case ' ':
        state = term_char;
        break;
      case '<':
      case '>':
      case '|':
      case ';':
      case '\'':
      case '&':
        state = meta_char;
        break;
      case '\\':
        state = escape_next_char;
        break;
      default:
        state = normal_char;
        break;
      }
      break;
    case normal_char:
      w[size] = focus;
      size += 1;
      state = determine_next;
      break;
    case meta_char:
      // For some hellish reason, people can decide not to put spaces around
      // meta characters. Therefore, we need to determine if this was the first
      // thing encountered (lone meta character) or if it is "sandwiched" by
      // other characters (no spaces between normal and meta character).
      //
      // Example (lone meta character):
      // Hello >! there
      //
      // Example (sandwiched):
      // Hello>!there
      // Hello>! there
      //
      // These inputs will return the same thing but require different ways of
      // handling.
      if (focus == '\'') // we can sandwich quotes
      {
        state = wait_for_end_quote;
      } else if (size != 0) // sandwiched meta character
      {
        state = pushback_char;
      } else if (focus == '>') {
        state = meta_gt;
      }
      // Explicitly handle semi-colons the same as newlines
      else if (focus == ';') {
        state = term_char;
      } else {
        w[0] = focus;
        // This is explicitly set to 1 so there is no possibility of having an
        // incorrect size.
        size = 1;
        state = end_lex;
      }
      break;
    case term_char:
      if (size == 0) {
        switch (focus) {
        case EOF:
          return -1;
        case '\n':
        case ';': // Handle ';' exactly the same as newlines
          return 0;
        // Skip leading spaces
        case ' ':
          state = determine_next;
          continue;
        }
      }
      // Take everything BEFORE the metacharacter, save the metacharacter for
      // next run since it is not part of the already collected word.
      state = pushback_char;
      break;
    case wait_for_end_quote:
      state = determine_next;
      while ((focus = getchar()) != '\'' && size < STORAGE - 1) {
        if (focus == '\\') {
          // Looking ahead to see if we find a !.
          char nextChar = getchar();
          if (nextChar == '\'') {
            focus = nextChar;
          } else {
            // Oops! We've grabbed too many characters and now need to ask
            // stdin for forgiveness (put back the extra character and force
            // focus to be '\'). We only care about the \' sequence, everything
            // else goes into the character array.
            ungetc(nextChar, stdin);
            focus = '\\';
          }
        }
        // Encountered unexpected EOF or newline
        else if (focus == EOF || focus == '\n') {
          state = missing_end_quote;
          break;
        }
        w[size] = focus;
        size += 1;
      }
      break;
    case escape_next_char:
      focus = getchar();
      // Do NOT escape newlines, its meaning is not affected by the backslash.
      if (focus == '\n') {
        state = term_char;
      } else {
        state = normal_char;
      }
      break;
    case meta_gt:
      // We will always return something here. Therefore we can take from stdin
      // at will and push back as needed.
      focus = getchar();
      if (focus == '!') {
        // We can explicitly return >! and a size of 2 since it is guaranteed to
        // not overwrite any data by meta_char.
        w[0] = '>';
        w[1] = '!';
        size = 2;
        state = end_lex;
      } else {
        // We can explicitly return > and a size of 1 since it is guaranteed to
        // not overwrite any data by meta_char. Data needs to be pushed back
        // onto stdin because we looked ahead and did not find ! following >.
        w[0] = '>';
        size = 1;
        state = pushback_char;
      }
      break;
    case pushback_char:
      // We could return from here but send it to end_lex to prevent code
      // duplication.
      // Data MUST be returned to stdin because we will only be in this
      // state if we looked too far ahead and did not achieve our desired
      // result. We will *always* end lexing the current word after this
      // stage as a result.
      ungetc(focus, stdin);
      state = end_lex;
      break;
    case end_lex:
      w[size] = '\0';
      return size;
    case missing_end_quote:
      return -2;
    }
  }

  // When we overflow the STORAGE size, make the last character null-terminating
  // and return. Otherwise, these lines will never be hit.
  w[size] = '\0';
  return size;
}

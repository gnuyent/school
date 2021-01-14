# C Programming Language (1988) - Kernighan & Ritchie

## Common Programming Concepts

### Functions

Functions in C are similar to other programming languages - they specify operations to be executed. Arguments are indicated by values enclosed by parentheses called the *argument list*. C requires braces around function statements similar to Java or Rust. The following code block adds one to a given value:

```c
// Adds one to the input
addOne(input) { // input is the only value in the argument list
    input = input + 1;
    return input;
}
```

### Variables

Variables are defined with variable declarations which consist of the type followed by the variable(s). 

```c
int fahr, celcius;
int lower, upper, step;
```

#### Constants

Constants are referred to as *symbolic constants* or *symbolic names* in C. They are defined with the `#define` keyword. Constant names are series of letters and digits that begin with a letter. They are typically uppercase.

```c
#define name replacement_text

#define LOWER 0
#define UPPER 300
```

### Comments

Comments are denoted by the opening `/*` and closing `*/`. 

### Data Types

* `char` - character, a single byte
* `short` - short integer
* `int` - 16-bit integer (normally)
  * Integer division truncates, discarding any fractional part regardless of its value.
  * If there is one integer and one floating point, the integer will be converted to floating point.
* `float` - 32-bit floating point (normally)
* `long` - long integer
* `double` - double-precision floating point

### Control Flow

#### while

`while` loops evaluates the given condition and executes its body until it is false. Similar to functions, statements within `while` loops must be enclosed in brackets. If the loop has only one statement, brackets are not required (like Java).

```c
int left = 0;
int right = 1;
while (left < right) {
    // statements here
}
```

#### for

For loops look similar to Java - they include the starting value, condition to loop to, and the increment.

```c
int left;
    
for (left = 0; left < 10; left = left + 1) {
	// statements here
}`
```

## String Formatting

### String Constants

Formatting strings in C is explicit. *Character strings* or *string constants* are shown by a sequence of characters enclosed by double quotes. 

```c
printf("Hello world"); // Hello world is the string constant
```

### Escape Sequences

In other languages like Python, newlines are automatically inserted into print statements. In order to replicate this functionality in C, the *newline character* `\n` must be specified. Other *escape sequences* can be defined with backslashes. Common ones are `\t` for tab, `\b` for backspace, `\"` for the double quote, and `\\` for the backslash. 

### String Arguments

String formatting makes heavy use of the `%` symbol. Each `%` denotes where one of the other arguments will be substituted.

```c
// boilerplate code
printf("%d\t%d\n", fahr, celsius);
```

In the example above, `%d` specifies that the argument must be an integer. The line can be read as "integer argument, tab, integer argument, newline". The first `%` is paired with `fahr` and the second with `celsius`. Additionally, the argument can be an expression as long as it evaluates to that specific type (i.e. `celsius` can be replaced by `2+2`).

#### Integer

Formatting with `%` also allows for specification of width. Placing a number after the `%` symbol specifies the width to display and justifies it to the right. `%3d` would be paired with an integer argument and display at least 3 characters wide.

#### Floating-point

Floating-point formatting is extremely similar to integers. Instead of the `d` character, `f` is used. `%3.0f` will print the paired argument at least three characters wide, with no decimal point and no fraction digits. `%6.1f` will print the paired argument at least six characters wide, with one digit after the decimal point.

#### Extra Uses

The `%` operator recognizes `%o` for octal, `%x` for hexadecimal, `%c` for character, `%s` for character string, and `%%` for the actual percent symbol.

## Character Input/Output

Within the standard library, text input/output is treated as a *text stream*. Text streams are defined as sequences of characters divided into lines where each line contains zero or more characters followed by a newline character. `getchar` reads the next input character from a text stream and returns its value.

```c
c = getchar(); // c now has the next character
```

`putchar` prints a character when it is called.

```c
putchar(c); // prints c
```

It is important to distinguish the `char` and `int` types. `char` is internally stored as a bit pattern, specifically meant to store character data. However, character data can also be stored in any integer type. Defining an integer variable that will eventually contain a character is useful because of a specific attribute present in `getchar`. When there is no more input, a special integer called `EOF`, or end of file, is returned. The `char` type is not big enough to fit `EOF`.


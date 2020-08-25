# Math 245 - Discrete Mathematics

Taught by Professor [Vadim Ponomarenko](https://vadim.sdsu.edu/), virtually, during Fall 2020 at San Diego State University.

**Quizzes** are conducted on Canvas.

**Lectures** are conducted on Zoom.

**Homework/Exercises** are conducted on Gradescope.

# 1 Mathematical Definitions

In real, natural languages, we do not rely on definitions of words to convey meaning, moreso the *idea* and mental image that we have of the thing.

## 1.1 Intro

### Mathematical Definitions

In math, we **use** definitions every time a word is **used**. We have to use definitions in order to prove other mathematical concepts.

Mathematical definitions are composed of:

1. Context
   * The latter parts of a definition rely on initial parts of a definition.
2. Category
   * A special subset of objects that we give a name to that belong in a larger category.
3. Correct English

In regular English, it is common to avoid categories by using "when" and "where", writing the definition as a command, or an activity to be performed. This is almost always wrong - try to rewrite the definition to avoid these terms.

Additionally, when mathematical definitions are given, it is important to *immediately* identify the category.

### Example: Slope

> Let $y=mx+b$ be the equation of a line in the x-y plane.
>
> The number $m$ is called the slope of that line.

In this example, we can identify the context of the proof to be the first sentence - the second sentence refers to *that line* which is given in the first sentence. The category refers to *numbers* since we identify *m* as one.

### Example: A Proof using Definitions

Definition:

> We call integer $n$ even if there are exists an integer $m$ with $n=2m$.

Here, the category is identified as *integer* and the property is *if there exists an integer $m$ with $n=2m$*. Using this information, we can solve the following theorem:

> The sum of any two even integers is even.

Proof:

> Let $a, b$ be two even integers.
>
> Because $a$ is even, there exists an integer $c$ with $a=2c$. (**1**)
>
> Because $b$ is even, there exists an integer $d$ with $b=2d$. (**2**)
>
> Adding, we get $a+b=2c+2d=2(c+d)$. (**3**)
>
> Since $c+d$ is an integer, $a+b$ is even. (**3**)

In the proof, we use the given definition but change the variables. This is allowed. Similarly, we use the definition in both directions. The first two times (1, 2) are using the given definition to assign variables that we will use later on. The last time (3) is using the property of the definition in order to prove the initial statement of the definition.

Using the given definition in this manner allows us to write a satisfactory mathematical proof.

### Entry Point: Numbers

To build basic definitions, we begin with a set of entry point "numbers". These include:

* The natural numbers $\mathbb{N} = \{1,2,3,...\}$  
* The whole numbers $\mathbb{N}_0 = \{0,1,2,3,...\}$
* The integers $\mathbb{Z}=\{...,-3,-2,-1,0,1,2,3,...\}$
* The rationals $\mathbb{Q} = \{\frac{a}{b}:a, b \in \mathbb{Z}, b \ne 0 \}$
  * $\in$ - "is an element of"
* The reals $\R$
* The complex numbers $\mathbb{C}$

## Definitions

### 1.1  Discriminant

Category: **Number**

*Let* $f(x) = ax^2 +bx+ c$ *be a quadratic polynomial in* $x$. *The* discriminant *of* $f(x)$ *is the number* $b^2 - 4ac$.


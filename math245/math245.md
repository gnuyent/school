# Math 245 - Discrete Mathematics

Taught by Professor [Vadim Ponomarenko](https://vadim.sdsu.edu/), virtually, during Fall 2020 at [San Diego State University](https://www.sdsu.edu/).

**Quizzes** are conducted on Canvas.

**Lectures** are conducted on Zoom.

**Homework/Exercises** are conducted on Gradescope.

**Chapters** are based upon the textbook, *Mathematical Maturity via Discrete Mathematics*.

[TOC]

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

### def 1.1: Discriminant (Number/be)

*Let* $f(x) = ax^2 +bx+ c$ *be a quadratic polynomial in* $x$. *The* discriminant *of* $f(x)$ *is the number* $b^2 - 4ac$.

### Example: Slope

> Let $y=mx+b$ be the equation of a line in the x-y plane.
>
> The number $m$ is called the slope of that line.

In this example, we can identify the context of the proof to be the first sentence - the second sentence refers to *that line* which is given in the first sentence. The category refers to *numbers* since we identify *m* as one.

### Example: A proof using definitions

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

## 1.2 Evens and Odds

To expand on our previous description of the word *definition*, definitions apply labels to properties. 

The following definitions are seemingly common knowledge, however, they cannot be applied without a true definition. Here, the definition of even and odd only apply to integers.

### def 1.2: Even (Integer/is)

*We say that* $n \in \mathbb{Z}$ *is* even *if there is some* $m \in \mathbb{Z}$ *such that* $n = 2m$.

### def 1.3: Odd (Integer/is)

*We say that* $n \in \mathbb{Z}$ *is* odd *if there is some* $m \in \mathbb{Z}$ *such that* $n = 2m + 1$.

At this point, integers are not provably only odd or even. That will come later.

### theorem 1.4: Even Addition

*Let* $a,b$ *be even. Then* $a+b$ *is even*.

Using the definition of even integers, we can simply prove theorem 1.4:

> Because $a,b$ are even, there are $c,d \in \mathbb{Z}$ such that $a=2c$ and $b=2d$.
>
> We have $a+b=2c+2d=2(c+d)=2e$ for some $e \in \mathbb{Z}$.
>
> Hence $a+b$ is even.



### theorem 1.5: Division Algorithm

Let $a, b$ be integers with $b \geq 1$. Then there are unique integers $q, r$ with $a=bq+r$ and $0 \leq r < b$. 

### Example: A proof using a given theorem

**Definition**: We say an *integer* $n$ is odd if there is an integer $m$ with $n=2m+1$.

Here, we will prove the definition above with the given [Division Algorithm Theorem](#theorem 1.5: Division Algorithm Theorem), [even](#def 1.2: Even (Integer/is)) definition, and [odd](#def 1.3: Odd (Integer/is)) definition:

> Apply the division algorithm theorem with $a=n$ and $b=2$.
>
> We get unique integers $q, r$ with $a=bq+r$ and $0 \leq r < b$.
>
> We have two cases, since $0 \leq r < 2$.
>
> If $r=0$, then $n=2q$. That makes $n$ even.
>
> If $r=1$, then $n=2q+1$. That makes $n$ odd.
>
> In all cases, $n$ is either even or odd.

We use a kind of magical trick in this proof. First off, we use the variables that match the given theorem and definition - $a$ is set to equal $n$ and we select the unique integers $q, r$. Second, we select $r$ smartly. By making it $2$, we minimize the maximum number of cases and allow us to prove this simply. 

Let's see how we can create another proof by using the given definition forwards and backwards.

### Example: Even/Odd

Theorem:

> The sum of an *even* and *odd* number is *odd*.

Notice how there are three italicized words in the theorem, each one representing a unique usage of the definition of even or odd given above. Knowing this information, we can prove the theorem:

> Let $a$ be even, and $b$ be odd. (**1**)
>
> Since $a$ is even, there is an integer $m$ with $a=2m$. (**2**)
>
> Since $b$ is odd, there is an integer $n$ with $b=2n+1$. (**2**)
>
> We add $a+b=2m+(2n+1)=2(m+n)+1$.
>
> Set $p=m+n$, an integer. (**3**)
>
> Since there is an integer $p$ with $a+b=2p+1$, $a+b$ is odd. (**4**)

Within this proof, there are four major points to talk about.

1. It is important to identify the variables you will be using immediately.
2. Here, we use the definition of even/odd in the forwards direction, creating a relationship, and definition, for $a$ and $b$.
3. Make sure to define every variable used.
4. The definition of even/odd must be applied backwards now. We have the result $a+b=2p+1$, what can we do with it? From the definition, $2p+1$ represents an odd number, therefore $a+b$ is odd.

## 1.3 Some Important Definitions








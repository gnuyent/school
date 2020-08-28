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

> Because $a,b$ are even, there are $c,d \in \mathbb{Z}$ such that $a=2c$ and $b=2d$. (**1**)
>
> We have $a+b=2c+2d=2(c+d)=2e$ for some $e \in \mathbb{Z}$. (**2**)
>
> Hence $a+b$ is even. (**3**)

Within this proof, there are five usages of [def 1.2](#def 1.2: Even (Integer/is)). We can break it down to easy-to-digest parts:

The Hypothesis

* The theorem hypothesizes that $a,b$ are even, therefore $c,d$ exist. (1)
* Since the [definition of even](#def 1.2: Even (Integer/is)) specifies that it only applies to integers ($\mathbb{Z}$), $a,b,c,d$ must all be integers as well.

The Body

* Using simple algebra, we can derive $a+b=2(c+d)$ and do simple variable substitution to retrieve $2e$. Note that $e$ is also an integer.

The Conclusion

* Since we derived $2e$, we can use the [definition of even](#def 1.2: Even (Integer/is)) backwards. $2e$ and $a+b$ are integers, $a+b$ must be even.

### theorem 1.5: Division Algorithm (named)

*Let* $a,b \in \mathbb{Z}$ *with* $b \geq 1$. *Then there are unique* $q,r \in \mathbb{Z}$ satisfying $a=bq+r$ *and* $0 \leq r \lt b$.

Using this theorem, we can prove the next theorem:

### theorem 1.6: Odd or Even or Both?

*Take* $n \in \mathbb{Z}$. *Then* $n$ *is odd or* $n$ *is even*.

Proof:

> Apply [Theorem 1.5](#theorem 1.5: Division Algorithm (named)) to $n, 2$ to get $q, r \in \mathbb{Z}$ satisfying $n=2q+r$ and $0 \leq r \lt 2$.
>
> Since we have $r \in \mathbb{Z}$, either $r = 0$ or $r =1$.
>
> If $r=0$, then $n=2q$, so $n$ is even by [Definition 1.2](#def 1.2: Even (Integer/is)).
>
> If $r=1$, then $n = 2q +1$ so $n$ is odd by [Definition 1.3](#def 1.3: Odd (Integer/is))

### theorem 1.7: Only Odd or Even

*Take* $n \in \mathbb{Z}$. *It is not possible for* $n$ *to be both odd and even*.

### corollary 1.8: Only Odd or Even

*Let* $n \in \mathbb{Z}$. *Then* $n$ *is exactly one of {odd, even}*.

Proved by combining [theorem 1.6](#theorem 1.6: Odd or Even or Both?) and [theorem 1.7](#theorem 1.7: Only Odd or Even).

### Example: A proof using a given theorem

This is similar to the proof of [theorem 1.6]($theorem 1.6: Odd or Even or Both?).

**Definition**: We say an *integer* $n$ is odd if there is an integer $m$ with $n=2m+1$.

Here, we will prove the definition above with the given [Division Algorithm Theorem](#theorem 1.5: Division Algorithm (named)), [even](#def 1.2: Even (Integer/is)) definition, and [odd](#def 1.3: Odd (Integer/is)) definition:

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
> In all cases, $n$ is either even or odd, or both.

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
4. The definition of even/odd must be applied backwards now. We have the result $a+b=2p+1$, what can we do with it? From the [definition](#def 1.3: Odd (Integer/is)), $2p+1$ represents an odd number, therefore $a+b$ is odd.

## 1.3 Some Important Definitions

### def 1.9: Less than or Equal to (Whole Numbers/is)

*Consider* $a,b \in \mathbb{Z}$. *We say that* $a$ *is* less than or equal to $b$, *and write* $a \leq b$ *(or* $b \geq a$*), to mean that* $b - a \in \mathbb{N}_0$. *We say that* $a$ *is* less than $b$, *and write* $a < b$ *(or* $b > a$*), to mean that* $b-a \in \mathbb{N}_0$ *and* $a \ne b$.

Similarly, the statements can be negated:

* $a \nleq b$ to mean $a \leq b$ is not true ($b-a \notin \mathbb{N}_0$)
* $a \not\lt b$ to mean that $a < b$ is not true ($b -a \notin \mathbb{N}_0$ or $a = b$).

### theorem 1.10: Inequalities

a. $a \leq a$;

b. $a \not < a$;

c. *If* $a \leq b$ *then* $a \not > b;$

d. *If* $a \leq b$ *and* $b \leq a$, *then* $a=b;$

e. *If* $a \leq b$ *and* $b \leq c$, *then* $a \leq c;$

f. *If* $a \leq b$ *and* $b \lt c$, *then* $a \lt c;$ and

e. *If* $a \lt b$ *and* $b \leq c$, *then* $a \lt c$.

Proof (a):

> We have $a-a=0 \in \mathbb{N}_0$, so $a \leq a$.

Proof (d):

> Set $d=b-a$.
>
> Because $a \leq b$, we must have $d=b-a \in \mathbb{N}_0$.
>
> Because $b \leq a$, we must have $-d=a-b \in \mathbb{N}_0$.
>
> There is only one element of $\mathbb{N}_0$, whose negative is also in $\mathbb{N}_0$, namely $0$.
>
> Hence $d = 0$, so $a=b$.

These inequalities respect some arithmetic operations detailed in the next theorem.

### theorem 1.11: Inequalities and Arithmetic

*Let* $a, b,c,d \in \mathbb{Z}$. *Then*

a. *If* $a \leq b$ *then* $a+c \leq b+c$;

b. *If* $a \leq b$ *and* $c \geq 0$, *then* $ac \leq bc$;

c. *If* $a \leq b$ *and* $c \leq 0$, *then* $ac \geq bc$; *and*

d. *If* $a \leq b$ *and* $c \lt d$, *then* $a+c < b+d$.

Proof (b): 

> Since $a \leq b$, we must have $b-a \in \mathbb{N}_0$.
>
> Since $c \in \mathbb{Z}$ and $c \geq 0$ hold, we have $c \in \mathbb{N}_0$.
>
> The product of two whole numbers is a whole number, so $(b-a)c \in \mathbb{N}_0$.
>
> Expanding, we have $bc-ac \in \mathbb{N}_0$, so $ac \leq bc$.

### theorem 1.12: Inequalities and Integers ($\mathbb{Z}$)

a. *If* $a<b$, *then* $a \leq b-1$;

b. *If* $a\leq b < a + 1$, *then* $a = b$;

c. *If* $a - 1<b \leq a$, *then* $a = b$; *and*

d. *If* $a-1<b < a+1$, *then* $a = b$;

Proof (a):

> Since $a<b$, we must have $b-a \in \mathbb{N}_0$.
>
> Since $b-a \ne 0$, we must have $b-a =k$ for some $k \in \mathbb{N}$.
>
> Subtracting one from both sides, we have $b-a-1=k-1$, so $(b-1)-a = k-1 \in \mathbb{N}_0$.
>
> Hence $a \leq b - 1$.

Proof (b):

> Since $b<a+1$, we apply part (a) to conclude that $b \leq (a+1) - 1 = a$. 
>
> We combine $a \leq b$ with $b \leq a$, using [Theorem 1.10.d.](#theorem 1.10: Inequalities) to conclude that $a=b$.

### def 1.13: Floor (Integer/is)

*Let* $x \in \mathbb{R}$. *Then there is a unique integer* $n$ *such that* $n \leq x < n+1$. *We call* $n$ *the* floor *of* $x$, *and write* $n = \lfloor x \rfloor$.

### def 1.14: Ceiling (Integer/is)

*Let* $x \in \mathbb{R}$. *Then there is a unique integer* $m$ *such that* $m - 1 < x \leq m$. *We call* $m$ *the* ceiling *of* $x$, *and write* $m = \lceil x \rceil$.

The floor and ceiling of $x$ are integers that straddle $x$. If $x$ is an integer, $\lfloor x \rfloor = x = \lceil x \rceil$. Otherwise, $\lfloor x \rfloor < x < \lceil x \rceil$.

### def 1.15: Division (???/divides)

*Let* $m, n \in \mathbb{Z}$. *We say that* $m$ divides $n$ *if there exists some* $s \in \mathbb{Z}$ *such that* $ms =n$. *We can write this compactly as* $m|n$. *If* $m$ *does not divide* $n$, *we write this compactly as* $m\nmid n$.

$m|n$ is the *statement* "$m$ divides $n$", with the verb "divides". This is opposed to $\frac{m}{n}$ and $m/n$ which are numbers (fractions). $m|n$ is a special statement called a proposition.

### def 1.16: Composite and Prime (Natural Number/call)

*Take* $n \in \mathbb{N}$ *with* $n \geq 2$. *If there is some* $a \in \mathbb{N}$ *such that* $1 < a < n$ *and* $a|n$, *then we call* $n$ composite. *If not, then we call* $n$ prime.

Number $1$ is not prime or composite, it is a unit - a number that divides $1$. Within $\mathbb{Z}$, there are only two units: $-1$ and $1$. For prime $p$, if $p|mn$ then $p|m$ or $p|n$.

### def 1.17: Factorial (Natural Number/denoted)

*The factorial is a function from* $\mathbb{N}_0$ *to* $\mathbb{N}$, *denoted by* $!$, *as specified by:* $0! = 1, 1!=1$, *and* $n! = (n-1)! \cdot n$ *for* $n \geq 1$.

### def 1.18: Binomial Coefficient (Function/specified)

*Take* $a,b \in \mathbb{N}_0$ *with* $a \geq b$. *The* binomial coefficient *is a function from such pairs* $a, b$ *to* $\mathbb{N}$, *denoted by* $(^a_b)$, *as specified by* $(^a_b) = \frac{a!}{b!(a-b)!}$.


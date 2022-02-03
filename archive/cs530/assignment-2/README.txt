rdParser.py - LL recursive descent parser
    Author     : Brandon Nguyen (823630399)
    Course     : CS-530 - Systems Programming
    Instructor : Ben Shen
    Session    : Fall 2021
    School     : San Diego State University

EBNF Grammar:

<op> ::= and | or | nand | xor | xnor
<relop> ::= < | > | <= | >= | = | != | not
<expr> ::= <term> {<op> <term>}
<term> ::= int - int | <relop> int | (<expr>)

Learning Outcomes:

During this assigment, I had to (re)learn what EBNF grammars are and how to use
them to generate my own grammar. I started off with BNF grammars which had a lot
of useful tools to generate the correct grammar for this assigment, but was made
simpler with EBNF. Using this grammar, I was able to implement it with a recursive parser in Python (a language I'm very comfortable with!).
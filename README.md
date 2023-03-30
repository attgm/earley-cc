# earley-cc
Earley perser for the probabilistic context-free grammar on C++

## What is this?
I'm going to try to correct the code [I wrote as a master degree student](https://github.com/attgm/earley-c) into a modern code.

## Overview

The probabilistic context context-free grammars (or stochastic context-free grammars) are extensions of context-free grammars that assign a probability to each production, like;
$$S \overset{0.3}{\longrightarrow} A B$$
The Earley parser is a popular algorithm for parsing strings that belong to a given context-free grammar, and this code is a extended earley parser for probabilistic context-free grammar.

This code can solves the two problems:

- Authorize Problem :  Calc the total probability that the input string is generated.
- Parse Problem : Find the top-k sets of productions that generate the input string.

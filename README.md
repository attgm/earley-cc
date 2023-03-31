# earley-cc
Earley parser for the probabilistic context-free grammar on C++

## What is this?
I'm going to try to correct the code [I wrote as a master degree student](https://github.com/attgm/earley-c) into a modern code.

## Overview

The probabilistic context context-free grammars (or stochastic context-free grammars) are extensions of context-free grammars that assign a probability to each production, like;
$$S \overset{0.3}{\longrightarrow} A B$$
The Earley parser is a popular algorithm for parsing strings that belong to a given context-free grammar, and this code is a extended earley parser for probabilistic context-free grammar.

This code can solve the two problems:

- Authorize Problem :  Calc the total probability that the input string is generated.
- Parse Problem : Find the top-k sets of productions that generate the input string.

## How it works

You need to prepare two files; a production rule `rule.txt` and an input string `input.txt`. 

### Production rule
```
S
1.0 S A B
0.2 A a
0.8 A A B
0.6 B A B
0.4 B b
```

The first line is a start variable, and this code allows only one variable. 
The second and subsequent lines are the production rules, in the following order: a generation probability, variable on the left hand side, and variables and terminals on the right hand side. This code does not distinguish between non-terminal (variable) and terminal symbols.


## License

[MIT](https://choosealicense.com/licenses/mit/)
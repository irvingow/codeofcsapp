In this repo, there are some c codes of csapp.

In chapter8, I write a makefile to make it easier to compile and run the code in the book.
for example, we have a c file named sample.c, if this is the first time we want to compile
and run the code, we should first type
> gcc -c csapp.c

to generate csapp.o, then we can just type
> make sample

in the command line, then the execute file `sample` will be generated automatic, the we can
type
> ./sample

to run the code we have written.

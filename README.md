# linux-nl-duplication
A recreation of the Linux nl command

**build:** `gcc nl.c -o nl`

**usage:**

**nl [-bSTYLE] [-sSTRING] [FILE...]**

###### STYLE: -b with options a, n, and t.

-ba (All Lines) Print line number on all lines 
-bn (No Lines) Print line number on no lines
-bt (Non-Empty Lines) Prine line number on non empty lines

###### STRING: -s with a string following.

> -s" : "

Will print:

> 1 : The line

###### FILE: filename.

Every argument afterwards will be assumed to be a file to read.

Examples:

```
./nl file
./nl file file 
./nl -ba file
./nl -bt file file
./nl -bn -s" - " file
```

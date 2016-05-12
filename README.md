#rshell
https://github.com/jkuzn001/rshell.git
Installation
------------
Please run the following commands in terminal to run our rshell (Turtle Shell) on your computer
```Installation
$ git clone https://github.com/jkuzn001/rshell.git

$ cd rshell

$ make

$ bin/rshell
```

Introduction
------------
This is the rshell project of Jacob Kuznicki and Lam Le for cs 100 spring 2016. It takes in commands (from bash and special comands such as exit) and connectors and executes them.  We accomplished this by using a composite design pattern connisting of connector class (which acts as the composite class) and a command class (which acts as the leaf).

UML DIAGRAM
------------

![Alt text](UML_Diagram.png?raw=true "Optional Title")

Execute based on Tree Representation
------------
![Alt text](tree.png?raw=true "Optional Title")

Parsing with Tokens!!!
------------
![Alt text](Tokens.png?raw=true "Optional Title")

rshell (Turtle Shell) Features
------------
*Can run all bash commands within /bin

*Can support all connectors such as;
* ";" = The following command will always be exectued.
* "||" = If the current command fails, the following command will be executed.
* "&&" = If the current command succeeds, the following command will be executed.
* "#" = Everything after "#" will be a comment.

Bugs
----
Current Bugs:
*Echo Command: There is currently a bug with the echo command being placed in the middle of the user inputs.
For example, the command "ls; echo Hello World && exit" will run the "ls" command and the "echo" command with the flag
"Hello World", but the current parsing algorithm will cause the "&&" connector to be recognized as a command instead of a connector. There for the last command would be "&&" as the command and "exit" as the connector. We will solve this by restructing the parsing algorithm to meet this case.

A bug was encountered where a child process would not properly exit on a failed call to execvp but this was fixed by changing the wait system call to waitpid and using the WEXITSTATUS macro to determine the success of the child process which was killed using the exit command.
Authors
-------
[Jacob Kuznicki]: https://github.com/jkuzn001
[Lam Le]: https://github.com/lepatrick714

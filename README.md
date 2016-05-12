#rshell
https://github.com/jkuzn001/rshell.git
Introduction
------------
This is the rshell project of Jacob Kuznicki and Lam Le for cs 100 spring 2016. It takes in commands (from bash and special comands such as exit) and connectors and executes them.  We accomplished this by using a composite design pattern connisting of connector class (which acts as the composite class) and a command class (which acts as the leaf).

UML DIAGRAM
------------

![Alt text](UML_Diagram.png?raw=true "Optional Title")

rshell (Turtle Shell) Features
------------
*Can run all bash commands within /bin

*Can support all connectors such as;
  
<tab>-";" = the following command will always be exectued.


Bugs
----
A bug was encountered where a child process would not properly exit on a failed call to execvp but this was fixed by changing the wait system call to waitpid and using the WEXITSTATUS macro to determine the success of the child process which was killed using the exit command
Authors
-------
[Jacob Kuznicki]: https://github.com/jkuzn001
[Lam Le]: https://github.com/lepatrick714

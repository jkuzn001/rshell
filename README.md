![Alt text](/pictures/Turtle.jpeg?raw=true "Optional Title")

#Turtle Shell
Full Documentation of Turtle Shell designs and coding strategies can be found in

https://drive.google.com/open?id=0B63l6C_S7h9vTTU5UERYaWpPZFU

Installation
------------
Please run the following commands in terminal to run our Turtle Shell on your computer
```Installation
$ git clone https://github.com/jkuzn001/rshell.git

$ cd rshell

$ make

$ bin/rshell
```

Introduction
------------
This is the Turtle Shell project of Jacob Kuznicki and Lam Duy Le (aka Patrick Le) for CS 100 Spring 2016. It takes in commands (from bash and special comands such as exit) and connectors and executes them.  We accomplished this by using a composite design pattern consisting of connector class (which acts as the composite class) and a command class (which acts as the leaf).

For the second part of this assignment we added some functionality to our Turtle Shell.  We added in the test command that can be called using either the command "test" or by using [].  This command will test if a given file or directory exists in a given path.  The test command's behavior can be augmented by giving it one of the following flags: -e tests if the file or directory exists (is assumed if no flag is provided), -f tests if the file or directory given is a regular file, -d tests if the file or directory given is a directory.  We also added in precedence of commands with ().

For the final part of this assignment we added in input and output redirection and piping.  The input redirection operator (<) will take in input from an input file for use by a command.  There are two different output redirection operators.  The first one (>) will take the output from a command line and output it to a file, if the file does not exist then it will be created.  The second one (>>) will take the output from a command line and append it onto the end of a file if it already exists, if it does not exist then the file will be created.  The pipe operator (|) will take the output from a command and pipe it to another command.

UML DIAGRAM
------------

![Alt text](/pictures/Diagram1.png?raw=true "Optional Title")

Parsing with Tokens!!!
------------
We used the C Token library to help us parse the user's string. From there we combined the Tokens to create both our Cmd's objects and Connector's objects

![Alt text](/pictures/Tokens.png?raw=true "Optional Title")

Execute based on Tree Representation
------------
From there we will construct a tree that will recursively call execute(), which will call execvp to run the specific commands and flags. Each iteration of a leaf will return a boolean value which will be evaluated by the Connectors to determine whether the tree should travel to the right leaf or skip and return back up the tree.

![Alt text](/pictures/tree.png?raw=true "Optional Title")

Tree Representation For Precedence
------------
We decided to expand the Turtle shell by implementing the Precedence Operator. To accomplish this, we decided to encapsulate the tree representation algorithm into a **Base *grabTree(char *cstr)** which will pass in a cstring and return a Base tree representation. From there we can parse the User's input to determine whether a string is between a precedence operator. To accomplish this, we were inspired by the post-fix calucator algorithm thus we constructed our own version of a stack familiar algorithm. Lastly, pushed all returned Base tree representation into a __queue<Base *> treeBranches__. A picture representation is shown below of the example:  "(echo A && echo B) && (echo C && echo D)".

![Alt text](/pictures/PrecedenceParsing.png?raw=true "Optional Title")

From There we shall reuse our previous algorithms to connect all tree branches into one whole tree. A picture representation is shown below.

![Alt text](/pictures/PrecedenceTree.png?raw=true "Optional Title")

Turtle Shell Features
------------
**Can run all bash commands within /bin**

**Can support all connectors such as;**
* ";" = The following command will always be exectued.
* "||" = If the current command fails, the following command will be executed.
* "&&" = If the current command succeeds, the following command will be executed.
* "#" = Everything after "#" will be a comment.

**Can support precedences operators so instead of <br />**
echo A && echo B || echo C && echo D which will yield <br /><br />
A <br />
B <br />
D <br /><br />
You can now do <br />
(echo A && echo B) || (echo C && echo D) which will yield <br /><br />
A <br />
B <br /><br />

**Can executes the test command by using "test" or "[ ]" with flags specified below;**
* if flag == -e it will be tested to see if it is either a directory or a regular file
* if flag == -f it will be tested to see if it is a regular file
* if flag == -d it will be tested to see if it is a directory
* if no flag is provided then -e will be assumed
test command will print out (true) and (false) based on returned boolean value respectively. <br />
Example: test -e main.cpp or [-e main.cpp] will return (true) or (false) based on whether main.cpp is in the current directory. <br />
Lastly the test command can be combined with other connectors discussed above.

**User information work!!!**<br />
Instead of having Turtle Shell print a boring "$" before each command.
Our Turtle Shell can print out your entire user's info.<br />

For example, suppose your username is "Turtle" and that you're logged into the machine named "shellTheFirst",
our Turtle Shell will prompt "Turtle@shellTheFirst$ " on your terminal!

Bugs
------------
Turtle Shell does not support quotes combined flags.
* Normal Shell: cat "food in can" => cat: can't open food in cans
* RShell (Turtle Shell): cat "food in can" => cat: "food: No such file or directory...cans": No such file or directory

Turtle Shell does not support the cd command.

A bug was encountered where a child process would not properly exit on a failed call to execvp but this was fixed by changing the wait system call to waitpid and using the WEXITSTATUS macro to determine the success of the child process which was killed using the exit command.

Authors
------------
Jacob Kuznicki: https://github.com/jkuzn001

Lam Duy Le (aka Patrick Le): https://github.com/lepatrick714

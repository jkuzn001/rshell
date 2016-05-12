#!/bin/sh
### commented command test ###
$ cd rshell
$ ./a.out

echo hello #world
ls -a || echo hello world #testing comments on multiple commands
ps #exit
echo test # || ls 
mkdir #test_directory
mkdir test_directory #create a test directory
echo test && echo test2 # echo both statements
history || ps # ls -a 
ls -R #### test comment
echo #commented echo test 
touch # testing touch with no file name
touch file.cpp #testing touch with a file name
cat file.cpp #testing cat on empty file
echo mkdir test_dir # echo test 
history -q # testing history with a flag

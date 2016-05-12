#!/bin/sh
### multiple command tests ###
$ cd rshell
$ ./a.out

echo hello world || ls -a; ps
ls -a && mkdir test_directory
history; ps
echo "this is a test" || echo "the first test failed" && ls; ps && history
mkdir test_directory2 && echo made test directory
echo testing clear; clear
echo || ls -m && echo testing || mkdir test_dir
ps && ls -r
cat nonexistantfile || echo testing invalid cat
ls -q; ps
touch test.cpp; echo created test.cpp
echo || mkdir || ls
ls -R && ps
ps; ps || ls && ps
ls -a || ls && ls

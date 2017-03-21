#! /bin/bash

echo "hello world">tmp.txt

myfile=./a.txt

{
read line1 line11
read line2
read line3
}<$myfile

echo "first word in $myfile is : $line1"
echo "remain words in $myfile is : $line11"
echo "second line in $myfile is : $line2"
echo "third line in $myfile is : $line333"




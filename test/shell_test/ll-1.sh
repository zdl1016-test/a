#! /bin/bash

for f in "$@"
do
    echo $f;
done

echo $#

if [ $# == 0 ]
then
    echo "param num != 0"
else
    echo "param num ==0"
fi

if [ $# == 0 ]; then echo "param num != 0";else echo "param num ==0";fi

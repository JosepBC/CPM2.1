#!/bin/bash

echo "file_name;time;procs_per_node;nodes"
for file in out/*
do
    file_name=$(basename $file | cut -f 1 -d .)
    time=$(grep elapsed $file | cut -f 3 -d ' ' | cut -f 1 -d 'e')
    procs=$(echo $file_name | cut -f 1 -d _)
    nodes=$(echo $file_name | cut -f 2 -d _)
    echo "$file_name;$time;$procs;$nodes"
done
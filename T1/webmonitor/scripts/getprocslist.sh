#!/bin/sh
#
# Iterates over every process file and prints it's status
# which is located in /proc/$PID/stat
#
for i in /proc/[0-9]*; do
	awk '{sub(/\(/,"");sub(/\)/,""); print $1,$2}' $i/stat
done

# https://stackoverflow.com/questions/12316167/does-linux-shell-support-list-data-structure
# "${mdcNo[@]}"


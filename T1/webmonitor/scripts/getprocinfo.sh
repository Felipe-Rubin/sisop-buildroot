#!/bin/sh
#
# Get processor information
# Core ID, Processor Model, CPU MHz
#
#
#CORE ID
# awk '/^core id/ {sub(/.*:\s*/,"");print $0}' /proc/cpuinfo

#MODEL NAME
# awk '/^model name/ {gsub(/(.*:\s*)|(\s*CPU.*)/,""); print $0}' /proc/cpuinfo

#SPEED
# awk '/^cpu MHz/ {gsub(/^.*:\s*/,""); print $0}' /proc/cpuinfo

#NAMED PIPE [https://unix.stackexchange.com/questions/310500/how-to-emulate-process-substitution-in-dash]
#Why ? 
#Because our distribution doesn't have PROCESS SUBSTITUTION: paste <(comand_1) <(comand_2) <(comand_3)

tmp=$(mktemp -d)
mkfifo "$tmp/f1" "$tmp/f2"
awk '/^core id/ {sub(/.*:\s*/,"");print $0}' /proc/cpuinfo >"$tmp/f1" &
awk '/^model name/ {gsub(/(.*:\s*)|(\s*CPU.*)/,""); print $0}' /proc/cpuinfo >"$tmp/f2" &
awk '/^cpu MHz/ {gsub(/^.*:\s*/,""); print $0}' /proc/cpuinfo > "$tmp/f3" &
paste "$tmp/f1" "$tmp/f2" "$tmp/f3"
rm -r "$tmp"


















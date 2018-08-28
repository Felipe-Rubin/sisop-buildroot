#!/bin/sh
#
# Get CPU load average from 1 min,5 min, 10min
#
awk '{print $1*100,$2*100,$3*100}' /proc/loadavg

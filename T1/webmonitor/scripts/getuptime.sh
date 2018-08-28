#!/bin/sh
#
# Get how many seconds the system has been running since boot (drop decimal)
#
awk ' {sub(/\..*/,"") ; print $1}' /proc/uptime

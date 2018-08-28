#!/bin/sh
#
# Get RAM information
# Total RAM, USED RAM (MB)
#
# https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=34e431b0ae398fc54ea69ff85ec700722c9da773
# https://access.redhat.com/solutions/406773
memtotal=$(awk 'NR==1 {printf "%.0f\n",$2/1000}' /proc/meminfo) #MEMTOTAL IN MB Already
# MemAvailable: An estimate of how much memory is available for starting new
# +              applications, without swapping.
memfree=$(awk 'NR==2 {printf "%.0f\n",$2/1000}' /proc/meminfo) #MEMFREE IN MB Already

memused=$(( $memtotal - $memfree ))

echo $memtotal","$memused

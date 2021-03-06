#!/bin/sh
echo $BASE_DIR
#Configure network
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

#Compile the syscall_test.c
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/syscall_test $BUILDROOT_DIR/custom-scripts/syscall_test.c


make -C $BASE_DIR/../modules/simple_driver/
make -C $BASE_DIR/../modules/sstf-iosched/

#Create fstab
#mkdir -p $BASE_DIR/target/sys/kernel/tracing
#echo "tracefs /sys/kernel/tracing tracefs 0 0" > $BASE_DIR/../output/target/etc/fstab

#Simple Tracer Application
make -C $BASE_DIR/../simple_tracer/

# Compile thread_runner
make -C $BASE_DIR/../modules/T3/

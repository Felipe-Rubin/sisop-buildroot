#!/bin/sh

#Configure network
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

#Configure Dropbear
cp $BASE_DIR/../custom-scripts/S41nosshpass-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41nosshpass-config
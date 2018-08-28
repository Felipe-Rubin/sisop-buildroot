#!/bin/sh
echo $BASE_DIR
#Configure network
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

#Configure Dropbear
cp $BASE_DIR/../custom-scripts/S42nosshpass-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S42nosshpass-config

#Configure Web Monitor
unzip $BASE_DIR/../custom-scripts/webmonitor-1.0.zip -d $BASE_DIR/target/opt/
cp $BASE_DIR/target/opt/webmonitor/initscripts/S90webmonitor $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S90webmonitor

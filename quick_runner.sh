ptxdist clean game &&
ptxdist clean driver-gamepad &&
ptxdist compile game &&
ptxdist compile driver-gamepad &&
ptxdist targetinstall game &&
ptxdist targetinstall driver-gamepad &&
ptxdist image root.romfs &&
ptxdist test flash-rootfs
exec miniterm.py -b 115200 -p /dev/ttyS0

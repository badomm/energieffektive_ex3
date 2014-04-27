ptxdist clean game
ptxdist compile game 
ptxdist clean driver-gamepad
ptxdist compile driver-gamepad
ptxdist test flash-rootfs
exec miniterm.py -b 115200 -p /dev/ttyS0

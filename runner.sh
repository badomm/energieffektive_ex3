ptxdist clean game &&
ptxdist clean driver-gamepad &&
ptxdist images &&
ptxdist test flash-all &&
exec miniterm.py -b 115200 -p /dev/ttyS0

MODULE = board

# Set UICR.PSELRESET[0] and UICR.PSELRESET[1] registers at 0x10001200
# to pin P0.18 (0x12), which is connected to the onboard Reset button
JLINK_POST_FLASH = Write4 0x10001200 00000012 00000012
OPENOCD_PRE_VERIFY_CMDS = -c 'flash fillw 0x10001200 0x00000012 2'

include $(RIOTBASE)/Makefile.base

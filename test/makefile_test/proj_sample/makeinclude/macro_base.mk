###############################################################################
#
#
###############################################################################

# gtest add by  hadilu 2010-11-17 below
MACHINE_PLATFORM  = $(shell $(CC) -dumpmachine)
# gtest add by  hadilu 2010-11-17 above

############################ outerlib begin ###########################
# path defines
PATH_OUTLIB           = $(project_home)/outerlib

# test 
INC_B                = -I$(PATH_OUTLIB)/libb/inc
LIB_B                = $(PATH_OUTLIB)/libb/lib/b.a 

INC_C                = -I$(PATH_OUTLIB)/libc/inc
LIB_C                = $(PATH_OUTLIB)/libc/lib/c.a 

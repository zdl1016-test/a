# author: baozhou(周龄)
#   desc: server Makefile框架

######################## Makefile固定部分开始 ########################
patterns = "/base_class|/innerlib|/application|/makeinclude|/outerlib|/sources"
project_home = $(shell pwd | awk -F$(patterns) '{print $$1}')
include $(project_home)/makeinclude/macro_base.mk
######################## Makefile固定部分结束 ########################

########################### 自定义部分开始 ###########################
#
# BASED变量定义需要依赖的底层文件, 下面的名字加上INC_或者LIB_前缀
# 之后对应makeinclude/appbase.mk或者plib/makefile.plib
# 里面的头文件跟库文件的宏定义

# 重定义MYSQL库, 强制使用.a库
LIB_MYSQL = $(LIB_MYSQL_A)

C_FLAGS_EX = -Werror

BASED    =  \
		  B \
		  C \

		  

# 依赖BASED以外的文件才需要定义
INC      = \

LIB      = \

# 定义server目标文件名字 如果后缀为so 则自动编译为动态库
#SERVER_TARGET = spp_appwork.so
SERVER_TARGET = test
#
########################### 自定义部分结束 ###########################


######################## Makefile固定部分开始 ########################
include $(project_home)/makeinclude/rules_server.mk
######################## Makefile固定部分结束 ########################

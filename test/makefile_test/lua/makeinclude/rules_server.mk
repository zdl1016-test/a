# author: baozhou(周龄)
#   desc: server Makefile规则框架

C_FLAGS := \
		   -O -ggdb3 -pipe -Wall -MMD -MP -D_REENTRANT -D_GNU_SOURCE -pthread \
		   -DQZONE_PROJECT_ROOT=\"/usr/local/qzone_v3.0/\" \
		   $(C_FLAGS_EX)

C_TARGET_FLAGS := $(C_FLAGS)

SERVER_TARGET  := bin/$(SERVER_TARGET)

# 当前编译的是否是so
ifneq (, $(findstring .so, $(SERVER_TARGET)))
    C_TARGET_FLAGS += -shared -Wl,-e,DisplayVersion
endif

INC           := $(INC) $(foreach i, $(BASED), $(INC_$(i))) -Iinc
LIB           := $(LIB) $(foreach i, $(BASED), $(LIB_$(i)))

CPP_SOURCES   := $(wildcard src/*.cpp)
CPP_OBJECTS   := $(patsubst src/%.cpp, obj/%.o, $(CPP_SOURCES))
CPP_DEPENDS   := $(patsubst src/%.cpp, dep/%.d, $(CPP_SOURCES))

C_SOURCES     := $(wildcard src/*.c)
C_OBJECTS     := $(patsubst src/%.c, obj/%.o, $(C_SOURCES))
C_DEPENDS     := $(patsubst src/%.c, dep/%.d, $(C_SOURCES))

TOOL_SOURCES  := $(wildcard tools/*.cpp)
TOOL_DEPENDS  := $(patsubst tools/%.cpp, dep/%.d, $(TOOL_SOURCES))
TOOL_TARGETS  := $(patsubst tools/%.cpp, tools/%, $(TOOL_SOURCES))

RED = \\e[1m\\e[31m
DARKRED = \\e[31m
GREEN = \\e[1m\\e[32m
DARKGREEN = \\e[32m
BLUE = \\e[1m\\e[34m
DARKBLUE = \\e[34m
YELLOW = \\e[1m\\e[33m
DARKYELLOW = \\e[33m
MAGENTA = \\e[1m\\e[35m
DARKMAGENTA = \\e[35m
CYAN = \\e[1m\\e[36m
DARKCYAN = \\e[36m
RESET = \\e[m

.DELETE_ON_ERROR:
.PHONY: help check all clean tool

help:
	@echo -e "usage:\n" \
	         "      make help(print current usage message)\n" \
			 "      make check(check dangerous function)\n" \
	         "      make all/clean\n" \
	         "      make tool"

tool: $(TOOL_TARGETS)

all: check $(SERVER_TARGET)
	file $(SERVER_TARGET)
	@echo
	ldd -r $(SERVER_TARGET) 2>&1 | grep -v "tbase\|Tencent\|write_log\|dl"
	@echo
	md5sum $(SERVER_TARGET)

$(SERVER_TARGET): $(CPP_OBJECTS) $(C_OBJECTS)
	@echo -e "g++ -o $(RED)$@$(RESET) \
	      $(DARKYELLOW)$(C_TARGET_FLAGS)$(RESET) $^ \
	      $(DARKGREEN)$(INC)$(RESET) $(DARKCYAN)$(LIB)$(RESET)\n"
	@g++ -o $@ $(C_TARGET_FLAGS) $^ $(INC) $(LIB)

$(CPP_OBJECTS) $(C_OBJECTS): Makefile

tools/%: tools/%.cpp $(LIB_TARGET)
	@echo -e "g++ -o $(RED)$@$(RESET) -MF dep/$*.d \
	      $(DARKYELLOW)$(C_FLAGS)$(RESET) $< \
	      $(LIB_TARGET) $(DARKGREEN)$(INC)$(RESET) $(DARKCYAN)$(LIB)$(RESET)\n"
	@g++ -o $@ -MF dep/$*.d $(C_FLAGS) $< $(LIB_TARGET) $(INC) $(LIB)

obj/%.o: src/%.cpp
	@echo -e "g++ -o $(RED)$@$(RESET) -MF dep/$*.d -c \
          $(DARKYELLOW)$(C_FLAGS)$(RESET) $< $(DARKGREEN)$(INC)$(RESET)\n"
	@g++ -o $@ -MF dep/$*.d -c $(C_FLAGS) $< $(INC)

obj/%.o: src/%.c
	@echo -e "gcc -o $(RED)$@$(RESET) -MF dep/$*.d -c \
       	  $(DARKYELLOW)$(C_FLAGS)$(RESET) $< $(DARKGREEN)$(INC)$(RESET)\n"
	@gcc -o $@ -MF dep/$*.d -c $(C_FLAGS) $< $(INC)

clean:
	rm -f $(C_DEPENDS) $(CPP_DEPENDS) $(TOOL_DEPENDS) \
		   $(C_OBJECTS) $(CPP_OBJECTS) $(SERVER_TARGET) $(TOOL_TARGETS)
	@echo

check:
	$(project_home)/makeinclude/make_check
	@echo

-include /dev/null $(CPP_DEPENDS) $(C_DEPENDS) $(TOOL_DEPENDS)

# author: baozhou(周龄)
#   desc: 功能模块 Makefile规则框架

define rule_group
$(1): $(2)
#下面的命令等价于: $1目录存在, 并且group_all中包含了$1, 那么进入$1目录执行make all
#[[ -d $(1) && "$(group_all)" == *$(1)* ]] && make -C $(1) all
	[[ ! -d $(1) || "$(group_all)" != *$(1)* ]] || make -C $(1) all
	@echo
clean_$(1):
	[[ ! -d $(1) || "$(group_all)" != *$(1)* ]] || make -C $(1) clean
	@echo
endef

ifndef group_all
group_all      = share api server cgi
endif
group_clean    = $(foreach g,$(group_all),clean_$(g))
dirs_sub       = share api server cgi
dirs_sub_clean = $(foreach g,$(dirs_sub),clean_$(g))

.PHONY: help check all clean unittest clean_unittest test execute \
		$(dirs_sub) $(dirs_sub_clean) $(dirs_unittest)

help:
	@echo -e "usage:\n" \
	         "      make help(print current usage message)\n" \
			 "      make check(check dangerous function)\n" \
	         "      make all/clean\n" \
			 "      make share/clean_share\n" \
			 "      make api/clean_api\n" \
			 "      make server/clean_server\n" \
			 "      make cgi/clean_cgi\n" \
			 "      make unittest/clean_unittest\n" \
			 "      make test(execute unittest bin and generate an XML report)\n" \
			 "      make execute(execute unittest bin only)"

#make all:直接编译all将导致顺序编译share api server cgi
all: $(group_all) unittest
clean: $(group_clean) clean_unittest

unittest: api
	for dir in $(dirs_unittest); do make -C $$dir all; done

clean_unittest:
	for dir in $(dirs_unittest); do make -C $$dir clean; done

test execute: api
	for dir in $(dirs_unittest); do make -C $$dir $@; done

check:
	$(project_home)/makeinclude/make_check
	@echo
#make share:导致进入share目录, build all
$(eval $(call rule_group,share))
#make cgi:导致先编译api, 然后进入cgi目录, build all
$(eval $(call rule_group,cgi,api))
#make server:导致先编译share, 然后进入server目录, build all
#make api:导致先编译share, 然后进入api目录, build all
$(foreach g,api server,$(eval $(call rule_group,$(g),share)))

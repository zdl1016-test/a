# author: baozhou(周龄)
#   desc: 工程总控Makefile 

# module之间可能有依赖, 所以module顺序不能乱调
module_list   = \
				app_analy \
				comm_server \
				app_mng \
				third_party_api \
				app_mng/unity/ctlv_service \
				app_mng/unity/appinfo \
				app_mng/unity/appaccess_service \
				app_mng/unity/index_service \
				game_request/appifeed_index \
				game_request/app_updatecenter \
				game_request/appifeed_main \
				game_request/request_main \
				openqqcom \
				openapi_v3 \
               

group_all     = outer base inner share api server cgi unittest
group_clean   = $(foreach g,$(group_all),clean_$(g))

dirs_outer    = outerlib
dirs_base     = base_class/src
dirs_inner    = innerlib
dirs_module   = $(foreach m,$(module_list),application/platform/$(m))
dirs_all      = $(dirs_outer) $(dirs_base) $(dirs_inner) $(dirs_module)

define loop_op
	for dir in $(1); do \
		make -C $$$$dir $(2); \
	done
endef

define rule_base
$(1): $(2)
	$(call loop_op,$(dirs_$(1)),all)
clean_$(1):
	$(call loop_op,$(dirs_$(1)),clean)
endef

define rule_group
$(1): $(2)
	$(call loop_op,$(dirs_module),$$@)
clean_$(1):
	$(call loop_op,$(dirs_module),$$@)
endef

.PHONY: help all clean check test execute \
	    $(group_all) $(group_clean) $(dirs_all)

help:
	@echo -e "usage:\n" \
	         "      make help(print current usage message)\n" \
			 "      make check(check dangerous function)\n" \
	         "      make all/clean\n" \
			 "      make outer/clean_outer\n" \
			 "      make inner/clean_inner\n" \
			 "      make base/clean_base\n" \
			 "      make share/clean_share\n" \
			 "      make api/clean_api\n" \
			 "      make server/clean_server\n" \
			 "      make cgi/clean_cgi\n" \
			 "      make unittest/clean_unittest\n" \
			 "      make test(execute unittest bin and generate an XML report)\n" \
			 "      make execute(execute unittest bin only)"

#make all:导致依次编译 outer base inner share api server cgi unittest
all: $(group_all)
clean: $(group_clean)

unittest: inner
	for dir in $(dirs_module); do make -C $$dir $@; done

clean_unittest:
	for dir in $(dirs_all); do make -C $$dir $@; done

test execute: inner
	for dir in $(dirs_all); do make -C $$dir $@; done

check: 
	./makeinclude/make_check

#下面的定义一些规则!!!
#make outer:导致编译dirs_outer(outerlib) all, 递归调用每个目录下的all
$(eval $(call rule_base,outer))
#make base导致编译dirs_base(base_class/src) all, 依赖outer
$(eval $(call rule_base,base,outer))
#make inner:导致编译dirs_inner(innerlib) all, 依赖outer
$(eval $(call rule_base,inner,base))

#make share:导致编译 make base, 以及各个dirs_module(也就是platform下的各个顶级目录)下的share
$(eval $(call rule_group,share,base))
#make api:导致编译 make share, 以及各个dirs_module(也就是platform下的各个顶级目录)下的api
$(eval $(call rule_group,api,share))
#make server:导致编译 make api, 以及各个dirs_module(也就是platform下的各个顶级目录)下的api
#make cgi:导致编译 make api, 以及各个dirs_module(也就是platform下的各个顶级目录)下的api
$(foreach m,server cgi,$(eval $(call rule_group,$(m),api)))

#!/bin/bash
# author: baozhou
# 重写了check.sh, 作了以下优化
# 1：不用check .svn、二进制、以及其他大量无关文件
# 2：对于每个文件，只调用一次grep, 而不是每个高危函数都grep一遍
# 3: 不生成临时文件
# 4: 高危函数扫描之前，先去掉头文件包含和注释，由于扫描的不是原始
#    文件，所以不显示扫描出来的高危函数的行号,但高亮显示扫出来的内容

if (($# < 2)); then
    echo "$0 source_path funcfile"
    exit 1
fi

source_path=$1
funcfile=$2
funcs=
filter=
targets=
check_helper=

get_funcs()
{
    read first <${funcfile}
    funcs="\\<$first\\>"

    while read func; do
        if [[ ${func} != "${first}" ]]; then
            funcs="${funcs}\\|\\<${func}\\>"
        fi
    done <${funcfile}
}

get_targets()
{
    filter="\( -name banned_tx.h \) -prune"
    targets="\( -name \*.cpp -o -name \*.cxx -o -name \*.C -o -name \*.cc \
             -o -name \*.c -o -name \*.h -o -name \*.hpp \) -print0"
}

analyze()
{
    eval find ${source_path} ${filter} -o ${targets} | while read -d '' file; do
        ret=$(sed 's/#\|include//' "$file" | cpp -w | \
              grep -v "# " | grep --color=always "${funcs}");
        if [[ "$ret" != "" ]]; then
            echo "$file: \n$ret\n";
        fi;
    done
}

do_check()
{
    get_funcs
    get_targets

    result=$(analyze)

    if [[ "$result" != "" ]]; then
        echo -e "$result"
        exit 2
    fi 
}

do_check


g++ -o symbol_in_multi_obj symbol_in_multi_obj.cpp ./helper2.o ./helper1.o // 编译报错
g++ -o symbol_in_multi_obj symbol_in_multi_obj.cpp ./helper2.a ./helper1.a // 编译不报错, 以前面的为准


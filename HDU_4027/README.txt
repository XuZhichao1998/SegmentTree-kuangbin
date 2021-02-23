HDU 4027 Can you answer these queries?
线段树求区间和。 更新操作为区间每个数开根号。
这个题询问和更新的区间可能 left > right, 要自己swap
题目输入保证初始值Ei >= 1

makedata.cpp用于生成大测试样例，已经编译为makedata可执行文件。需要传入一个参数，为输入文件的名字。
比如
./makedata 4.in
这个命令可以生成一组样例，并且保存在4.in中
我们自己运行hdu4027.cpp的时候也可以穿入参数，代表输入文件名。比如
g++ -o a hdu4027.cpp
./a in.txt
也可以将输入重定向到文件中，比如：
./a 3.in > 3.out


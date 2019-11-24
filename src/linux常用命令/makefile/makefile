#定义变量保存.c文件的名称
#通过wildcard函数获取当前目录下所有.c文件名称
src=$(wildcard ./*.c)
#定义变量保存.o文件名称
#通过patsubst函数获取src变量中.c文件对应的.o文件名称
objs=$(patsubst %.c,%.o,$(src))
#定义变量保存可执行程序的名称
target=app

#规则含义：通过objs中保存的.o文件生成名称为target变量值的可执行程序
#：号前面表示目标，：后面表示依赖
$(target):$(objs)
#gcc命令
	gcc $(objs) -o $(target)

#规则含义：任何.o,都通过同名.c生成
#%号是通配符，表示一个字符串
#两个百分号表示同一个字符串
%.o:%.c
# $< 自动变量，表示这条规则的第一个依赖
# $@ 自动变量，表示这条规则的第一个目标 
	gcc -c $< -o $@

#.PHINY:声明当前目标是伪目标，不会检测依赖的时间
.PHINY:clean
#这条规则用于执行 程序的清理
clean:
#清理所有当前make生成的所有.o文件和可执行程序
	rm -f $(objs) $(target)




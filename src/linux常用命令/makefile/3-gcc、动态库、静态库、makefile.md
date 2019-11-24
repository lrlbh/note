# 1.gcc

![1556900159755](assets/gnu-gcc.png)

> GNU编译器套件（GNU Compiler Collection）原名為**GNU C語言编译器**（**GNU C Compiler**），因為它原本只能处理[C語言](https://zh.wikipedia.org/wiki/C語言)。GCC在发布后很快地得到扩展，包括C、C++、Objective-C、Fortran、Java、Ada和Go语言的前端，也包括了这些语言的库（如libstdc++、libgcj等等）。GCC的初衷是为GNU操作系统专门编写的一款编译器。GNU系统是彻底的自由软件。此处，“自由”的含义是它尊重用户的自由。
>
> **GUN 编译器套件包含多种前端处理器，以翻译各种不同语言。当然，我们重点讨论的是基于C语言的前端处理器 GCC。**
>
> GCC 也是一种多目标（multitarget）编译器；换句话说，它通过使用可互换的后端处理器，为多种不同的计算机架构生成相应的可执行程序。  
>
> <font color="red">GCC 不仅支持C的许多“方言”，也可以区别不同的C语言标准；也就是说，可以使用命令行选项来控制编译器在翻译源代码时应该遵循哪个C标准。例如，当使用命令行参数`-std=c99`启动 GCC 时，编译器支持 C99 标准。</font>
>
> GUN99   c99



## 1.1 安装

- 安装命令

  ```shell
  sudo apt install gcc g++
  
  # 在Linux中写程序: gcc版本 > 4.8.5
  ```

- 查看版本

  ```shell
  gcc -v/--version
  g++ -v/--version
  ```

- gcc与g++有什么区别？ 

  > gcc和g++都是GNU(组织)的一个编译器。
  >
  > - 误区一：gcc只能编译c代码，g++只能编译c++代码。两者都可以，但是请注意：
  >   - 后缀为.c的，gcc把它当作是C程序，而g++当作是c++程序；
  >   - 后缀为.cpp的，两者都会认为是c++程序，C++的语法规则更加严谨一些。
  >   - 编译阶段，g++会调用gcc，对于c++代码，两者是等价的，但是因为gcc命令不能自动和C++程序使用的库联接，所以通常用g++来完成链接，为了统一起见，干脆编译/链接统统用g++了，这就给人一种错觉，好像cpp程序只能用g++似的。
  >
  > - 误区二：gcc不会定义`__cplusplus`宏，而g++会
  >   - 实际上，这个宏只是标志着编译器将会把代码按C还是C++语法来解释
  >   - 如上所述，如果后缀为.c，并且采用gcc编译器，则该宏就是未定义的，否则，就是已定义。
  >
  > - 误区三：编译只能用gcc，链接只能用g++
  >   - 严格来说，这句话不算错误，但是它混淆了概念，应该这样说：编译可以用gcc/g++，而链接可以用`g++`或者`gcc -lstdc++`。
  >   - gcc命令不能自动和C++程序使用的库联接，所以通常使用g++来完成联接。但在编译阶段，g++会自动调用gcc，二者等价

## 1.2 gcc 工作流程

> GCC 编译器在编译一个C语言程序时需要经过以下 4 步：
>
> 1. 将C语言源程序预处理，生成`.i`文件。
>    - 头文件展开
>    - 宏替换
>      - define NUM 10
>      - int a = NUM
>    - 注释删除
>    - xxx.c -> xxx.i
>    - gcc -E xxx.c -o xxx.i
> 2. 预处理后的.i文件编译成为汇编语言，生成`.s`文件。
>    - xxx.i   ->   xxx.s (得到汇编文件)
>    - gcc -S xxx.i -o xxx.s
> 3. 将汇编语言文件经过汇编，生成目标文件`.o`文件。
>    - xxx.s   -> xxx.o  (二进制文件)
>    - gcc -c xxx.s -o xxx.o
> 4. 将各个模块的`.o`文件链接起来生成一个可执行程序文件。
>    - xxx.o -> 可执行程序 (二进制的)
>    - gcc xxx.o -o xxx

![](assets/gcc.jpg)

## 1.3 gcc常用参数选项

|             **gcc编译选项**             |                        **选项的意义**                        |
| :-------------------------------------: | :----------------------------------------------------------: |
|                   -E                    |                预处理指定的源文件，不进行编译                |
|                   -S                    |               编译指定的源文件，但是不进行汇编               |
|                   -c                    |            编译、汇编指定的源文件，但是不进行链接            |
| -o [file1] [file2] / [file2] -o [file1] |             将文件 file2 编译成可执行文件 file1              |
|  <font face="宋体">-I</font> directory  |               指定 include 包含文件的搜索目录                |
|                   -g                    |      在编译的时候，生成调试信息，该程序可以被调试器调试      |
|                   -D                    |                 在程序编译的时候，指定一个宏                 |
|                   -w                    |                      不生成任何警告信息                      |
|                  -Wall                  |                       生成所有警告信息                       |
|                   -On                   | n的取值范围：0~3。编译器的优化选项的4个级别，-O0表示没有优化，-O1为缺省值，-O3优化级别最高 |
|                   -l                    |                在程序编译的时候，指定使用的库                |
|                   -L                    |               指定编译的时候，搜索的库的路径。               |
|               -fPIC/fpic                |                     生成与位置无关的代码                     |
|                 -shared                 |            生成共享目标文件。通常用在建立共享库时            |
|                  -std                   |         指定C方言，如:-std=c99，gcc默认的方言是GNU C         |

```c
// 参数 -D 使用场景: 程序调试的时候使用, 通过-D指定的宏打开log输出
// test.c
#include <stdio.h>
//#define DEBUG
int main()
{
    int a = 10;
#ifdef DEBUG
    printf("我是一个程序猿, 我不会爬树...\n");
#endif
    for(int i=0; i<3; ++i)
    {
        printf("hello, GCC!!!\n");
    }
    return 0;
}
// 默认编译第7行代码不输出, 如果想要输出这句话, 可以使用-D


// 参数 -On 优化程序, 有4个级别
// n 取值范围: 0-3
// O0 -> 不优化, O1 -> 默认的级别, O3 -> 最高级别
// 最简单的例子:
int a, b, c, d;
a = 10;
b = a;
c = b;
d = c;

// 优化:
b = 10;
c = 10;
d = 10;

// 参数 -std , 指定编译器规范
// c最早的标准:c89
// c99开始支持现在的书写习惯

int a = 10;
func();
int b  =5;
for(int i=0; i<5; ++i)

gcc -std=c99/gnu99 test.c -o test

```

```shell
-DDEBUG 参数和值直接的空格可有可无
gcc test.c -o test -DDEBUG   # define DEBUG
```

# 2 静态库和动态库

## 2.1 扫盲

- 库是什么?
  - 通过gcc编译得到的
  - 本质: 
    - 还是一个文件
    - 这个文件里边就是程序猿编写的代码
- 为什么要使用库?
  - 代码保密
  - 为了部署和分发
    - 100个源文件 -> 打包成一个库
- 库有了如何使用?
  - 要有动态库/静态库
    - 来自于 .c  .cpp, 里边是若干个函数实现
    - 需要有头文件声明源文件中实现的函数
  - 有头文件
    - 里边声明了可被调用的函数
- 制作出的库如何使用?

## 2.2 静态库

- 命令规则

  - Linux
    - libxxx.a
      - lib -> 前缀, 固定
      - xxx -> 库的名字, 自己起
      - .a -> 后缀, 固定
  - windows
    - libxxx.lib

- 静态库制作

  ![](assets/static-lib.png)

  ```shell
  # 1. 通过gcc 得到  .o 文件 -c
  .
  ├── add.c
  ├── div.c
  ├── head.h
  ├── main.c
  ├── mult.c
  └── sub.c
  gcc -c add.c div.c mult.c sub.c 
  # 2. 将得到的.o 打包 -> 使用 ar 工具 (archive) 
  ar rcs 
  	- r: 替换
  	- c: 创建
  	- s: 索引
  ar rcs libcalc.a add.o div.o mult.o sub.o
  ```

- 静态库使用

  ```shell
  # 静态库的分发
  #  1. 静态库: libcalc.a   2. 头文件: head.h
  # 静态库的使用:
  #  1. 根据头文件中的api, 编写测试程序
  #  2. 编译测试程序, 指定静态库
  .
  ├── include
  │   └── head.h
  ├── lib
  ├── libcalc.a
  ├── main.c
  └── src
      ├── add.c
      ├── div.c
      ├── mult.c
      └── sub.c
  
  $ gcc main.c -o app -I ./include/
  /tmp/ccF515Fs.o: In function `main':
  main.c:(.text+0x38): undefined reference to `add'
  main.c:(.text+0x58): undefined reference to `subtract'
  main.c:(.text+0x78): undefined reference to `multiply'
  main.c:(.text+0x98): undefined reference to `divide'
  collect2: error: ld returned 1 exit status
  
  # 错误原因: 找不到函数定义, 函数体实现, 函数实现代码在静态库中
  # 因此在 gcc 时候需要指定静态库
  
  $ gcc main.c -o app -I ./include/ -L -l
  	-L: 静态库的路径
  	-l: 静态库的名字: 去掉lib, 去掉.a, 剩下的是名字, libcalc.a -> calc
  $ gcc main.c -o app -I ./include/ -L ./ -l calc
  ```

## 2.3 动态库/共享库

> 共享 -> 共享内存中的库

- 命名规则

  - linux:
    - libxxx.so
      - lib -> 前缀, 固定
      - xxx -> 库的名字, 随便起名
      - .so -> 后缀, 固定
    - 在linux下是一个可执行文件
  - windows:
    - 非vs制作
      - libxxx.dll
    - 如果是使用vs制作的:
      - libxxx.lib
      - libxxx.dll

- 动态库制作

  ![](assets/dll-0.png)

  ```shell
  $ ls
  add.c  div.c  head.h  main.c  mult.c  sub.c
  # 1. 使用gcc 得到 .o, 得到和位置无关的代码
  gcc sub.c add.c div.c mult.c -c -fpic
  	-fpic/-fPIC: 使用相对地址记录代码的位置
  # 2. 使用gcc 得到动态库
  gcc -shared add.o sub.o div.o mult.o -o libcalc.so
  gcc -shared -o libcalc.so add.o sub.o div.o mult.o
  ```

- 动态库使用

  ```shell
  # 分发: 头文件, 动态库
  # 使用:
  # 编写测试文件, 对头文件中声明的函数测试
  .
  ├── app
  ├── include
  │   └── head.h
  ├── lib
  │   └── libcalc.a
  ├── libcalc.so
  ├── main.c
  └── src
      ├── add.c
      ├── div.c
      ├── mult.c
      └── sub.c
  $ gcc main.c -o app -I ./include/ -L ./ -lcalc
   - 这是只是保证在连接的时候没问题
  ```

  

- 解决动态库加载失败的问题

  ```shell
  ./main: error while loading shared libraries: libcalc.so: cannot open shared object file: No such file or directory
  
# 工作原理: 
  - 静态库: gcc进行链接之后, 静态库中的代码和测试文件代码被打包到了 可执行程序中
  # 假设动态库中有一add方法
  - 动态库: gcc进行链接之后, 动态库不会和可执行程序打包到一起
  	- 程序启动之后, 动态库会被动态加载到内存中
  		- 什么时候加载? 程序需要调用add函数的时候, 动态库被加载到内存
  
  # 可以通过命令检测是不是能加载到对应的库文件:
  $ ldd 可执行程序的名字
  $ ldd main
          linux-vdso.so.1 =>  (0x00007ffee23ae000)
          libcalc.so => not found
          libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa67f32b000)
          /lib64/ld-linux-x86-64.so.2 (0x00007fa67f6f5000)
  ```
  
  > **在执行的时候是如何定位共享库文件的呢？**
  >
  > - 当系统加载可执行代码时候，能够知道其所依赖的库的名字，但是还需要知道绝对路径。此时就需要系统的动态载入器(dynamic linker/loader)来获取该绝对路径。
  >- **对于elf**格式的可执行程序，是由**ld-linux.so来完成的**，它先后搜索elf文件的 `DT_RPATH段` ---> `环境变量LD_LIBRARY_PATH` ---> `/etc/ld.so.cache文件列表` ---> `/lib/，/usr/lib目录`找到库文件后将其载入内存。
  
  ```shell
  # 解决动态库找不到的问题:
  # 第一种: 将动态库的绝对路径添加到 LD_LIBRARY_PATH 中
  #    1. 测试时候使用的, 终端关闭, 设置失效:
  		在当前终端中执行: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/itcast/library
  #     2. 长久设置, 将export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/itcast/写到配置文件中
  		- 用户级别: ~/.bashrc
  			添加: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/itcast/library
  		- 系统级别: /etc/profile
  			添加: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/itcast/library
  		添加完成执行命令, 使修改生效:
  			.  ~/.bashrc     -> . 相当于 source
  			. /etc/profile
  			source /etc/profile
  
  # 第二种: 在 /etc/ld.so.cache 添加动态库路径
  # 需要间接完成这个操作:
  #  1. 找一个文件: /etc/ld.so.conf
  #  2. 执行一个命令, 进行更新: sudo ldconfig
  ```

## 2.4 对比

- 静态库

  - 优点：

  - - 静态库被打包到应用程序中加载速度快
    - 发布程序无需提供静态库，移植方便

  - 缺点：

  - - 销毁系统资源，浪费内存
    - 更新、部署、发布麻烦。

![](assets/dll-1.png)

- 动态库

  - 优点：

  - - 可实现进程间资源共享
    - 程序升级简单
    - 程序猿可以控制何时加载动态库

  - 缺点：

  - - 加载速度比静态库慢
    - 发布程序需要提供依赖的动态库

![](assets/dll-2.png)

# 3. Makefile

![1556959679409](assets/1556959679409.png)

> 什么是makefile？或许很多Winodws的程序员都不知道这个东西，因为那些Windows的IDE都为你做了这个工作，但我觉得要作一个好的和professional的程序员，makefile还是要懂。这就好像现在有这么多的HTML的编辑器，但如果你想成为一个专业人士，你还是要了解HTML的标识的含义。特别在Unix下的软件编译，你就不能不自己写makefile了，会不会写makefile，从一个侧面说明了一个人是否具备完成大型工程的能力。
>
> 因为，makefile关系到了整个工程的编译规则。一个工程中的源文件不计数，其按类型、功能、模块分别放在若干个目录中，makefile定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作，因为makefile就像一个Shell脚本一样，其中也可以执行操作系统的命令。
>
> <font color="red">makefile带来的好处就是——“自动化编译”，一旦写好，只需要一个make命令，整个工程完全自动编译，极大的提高了软件开发的效率。</font><font color="blue">**make是一个命令工具，是一个解释makefile中指令的命令工具**</font>，一般来说，大多数的IDE都有这个命令，比如：Visual C++的nmake，Linux下GNU的make。

## 3.1 文件命名

- 两种书写格式: 
  - makefile
  - Makefile

## 3.2 makefile中的规则

```makefile
gcc a.c -o app
# 一个makefile文件中可以有一个或多个规则
# 语法:
目标 ...: 依赖 ...
	命令(shell命令)
	...
目标: 最终要生成的文件, 伪目标不算
依赖: 生成目标的原材料
命令: 一通过命令对依赖操作生成目标, 命令前边是tab缩进


# makefile中的其他规则一般都是为第一条规则服务的
# 一个makefile中有多条规则, 对应有多个目标, 执行make生成一个最终目标 == 第一天规则中的目标
```

## 3.3 makefile中的知识点

### 3.3.1 工作原理

- 命令在执行之前, 需要先检测规则中的依赖是否存在
  - 如果存在: 执行命令
  - 如果不存在: 向下检测其他的规则
    - 看一看有没有一个规则是用来生成这个依赖的
    - 如果找到了, 执行该规则中的命令
- 检测更新
  - 在执行规则中的命令的时候, 会比较 目标和依赖文件时间
    - 如果依赖的时间比  目标的时间晚
      - 需要重新生成目标
    - 如果依赖的时间比  目标的时间早
      - 目标不需要更新, 对应规则中的命令不需要执行

### 3.3.2 变量

- 自定义变量

  ```makefile
  # 变量的定义
  # 自定义变量, 变量名小写
  变量名=变量值
  var=hello,world
  # 取变量的值
  $(变量)
  value=$(var)
  ```

- 默认的自带变量

  ```makefile
  # 变量名大写
  CC 默认等于 cc
  ```

- 自动变量

  ```makefile
  app:main.c hello.c a.c b.c
  	gcc -c main.c hello.c a.c b.c
  $@: 规则中的目标
  	- app
  $<: 规则中的第一个依赖
  	- main.c
  $^: 规则中的所有依赖
  	- main.c hello.c a.c b.c
  # 自动变量只能在规则的命令中使用
  app:main.c hello.c a.c b.c
  	$(CC) -c $^ -o $@
  ```

### 3.3.3 模式匹配

```makefile
add.o:add.c
    gcc -c add.c

div.o:div.c
    gcc -c div.c

sub.o:sub.c
    gcc -c sub.c

mult.o:mult.c
    gcc -c mult.c

main.o:main.c
    gcc -c main.c
    
%.o:%.c
	- %: 通配符, 匹配一个字符串
	- 两个%匹配的是同一个字符串
%.o:%.c
	gcc -c $< -o $@
```

![1557049517189](assets/1557049517189.png)

### 3.3.4 函数

- 获取指定目录下指定类型的文件列表 -- **wildcard**

  ```makefile
  $(wildcard PATTERN...) 
  ```

  - 功能:
    - PATTERN 指的是某个或多个目录下的对应的某种类型的文件
    - 如果有多个目录，一般使用空格间隔
  - 返回值：
    - 得到的若干个文件的文件列表， 文件名之间使用空格间隔
  - 示例：`$(wildcard *.c  ./sub/*.c)`
    - 返回值格式: a.c b.c c.c d.c e.c f.c

- 模式字符串替换函数——**patsubst**

  ```makefile
  $(patsubst <pattern>,<replacement>,<text>)
  %.c, %.o, a.c b.c d.c e.c
```
  
  - 功能:
    - 查找<text>中的单词(单词以“空格”、“Tab”或“回车”“换行”分隔)是否符合模式<pattern>，如果匹配的话，则以<replacement>替换。
    - 这里，<pattern>可以包括通配符`%`，表示任意长度的字串。如果<replacement>中也包含`%`，那么，<replacement>中的这个`%`将是<pattern>中的那个`%`所代表的字串。(可以用“\”来转义，以`\%`来表示真实含义的`%`字符)
  - 返回值:
    - 函数返回被替换过后的字符串。
  - 示例: `$(patsubst %.c, %.o, x.c.c bar.c)` 
    - 把字串“x.c.c bar.c”符合模式[%.c]的单词替换成[%.o]
    - 返回结果是“x.c.o bar.o”

## 3.4 makefile的编写

```shell
# 举例:
.
├── add.c
├── div.c
├── head.h
├── main.c
├── mult.c
└── sub.c
```

### 版本1

```makefile
# 缺点: 任意一个源文件被修改, 所有的源文件都需要被重新编译
app:add.c  div.c  main.c  mult.c  sub.c
        gcc add.c  div.c  main.c  mult.c  sub.c -o app
```

### 版本2

```makefile
# 缺点: 书写麻烦, 不适用于文件比较多的情况
# 改进: 引入一些变量
app:add.o  div.o  main.o  mult.o  sub.o                        
    gcc add.o  div.o  main.o  mult.o  sub.o -o app

add.o:add.c
    gcc -c add.c

div.o:div.c
    gcc -c div.c

sub.o:sub.c
    gcc -c sub.c

mult.o:mult.c
    gcc -c mult.c

main.o:main.c
    gcc -c main.c
```

### 版本3

```makefile
# 缺点: 书写麻烦,这些.o文件名, 需要手写
# 需要使用函数
objs=add.o  div.o  main.o  mult.o  sub.o
target=app
$(target):$(objs)
        gcc $(objs) -o $(target)

%.o:%.c
        gcc -c $< -o $@
#add.o:add.c
#       gcc -c add.c
#
#div.o:div.c
#       gcc -c div.c
#
#sub.o:sub.c
#       gcc -c sub.c
#
#mult.o:mult.c
#       gcc -c mult.c
#
#main.o:main.c
#       gcc -c main.c
```

### 版本4

```makefile
# sub.c add.c main.c mult.c div.c
src=$(wildcard ./*.c)
# sub.o add.o main.o mult.o div.o
objs=$(patsubst %.c, %.o, $(src))
target=app
$(target):$(objs)
        gcc $(objs) -o $(target)

%.o:%.c
        gcc -c $< -o $@
```

### 版本5

```makefile
# sub.c add.c main.c mult.c div.c
src=$(wildcard ./*.c)
# sub.o add.o main.o mult.o div.o
objs=$(patsubst %.c, %.o, $(src))
target=app
$(target):$(objs)
        gcc $(objs) -o $(target)

%.o:%.c
        gcc -c $< -o $@

# 清理项目
# 声明clean为伪目标
.PHONY:clean
clean:
        rm $(objs) $(target) -f
```





```http
# vim插件
https://github.com/chxuan/vimplus
```

```shell
# 基于ubuntu 安装程序
# tree
tree.deb
sudo dpkg -i tree.deb
sudo dpkg -r 程序名
```


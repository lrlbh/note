## RUST基础

### 环境搭建

~~~shell
# 1.需要安装Visual Studio 2013或更高版本的C++构建工具

# 2.安装rust
https://www.rust-lang.org/zh-CN/tools/install # windows	需要下载rustup-init.exe安装包
curl https://sh.rustup.rs -sSf | sh	# linux	执行获取安装脚本的命令	# 当前终端也生效 source $HOME/.cargo/env
~~~



### RUST管理（命令）

~~~shell
############################################# 基础 ##########################################################
# 更新
rustup update

# 卸载
rustup self uninstall

# 在浏览器,打开本地文档
rustup doc

# 版本号 
rustc --version

# 编译
rustc main.rs


############################################# Cargo ########################################################
# 版本号
cargo --version

# 创建项目	
cargo new 项目名称

# 如果在一个git目录中将不会生成git文件,强制生成git文件 
cargo new --vcs=git

# 编译项目 在项目目录执行
cargo build

# 编译加运行 
cargo run

# 检查项目是否编译通过
cargo check

# 发行版
cargo build --release
cargo run --release

# 升级依赖
# 只会升级兼容版本,比如0.11升级到0.12,不兼容的版本升级需要手动在toml文件中指定
cargo update


# 生成并查看依赖文档
cargo doc --open


#############################################rustfmt####################################################################
# 格式化文件
rustfmt 文件名

# 格式化项目
cargo fmt
~~~



### 格式化输出

~~~rust
// 格式化输出由 std::fmt 中定义的一系列宏来处理
// format!      格式化字符串,然后返回一个字符串
// print!       格式化字符串,然后标准输出(io::stdout)
// eprint!      格式化字符串,然后标准错误输出(io::stderr)
// println!     同print!结尾会换行
// eprintln!    eprint!结尾会换行

// format！的
fn format的使用() {
    // format
    let a: String = format!("{}", "abc");
    let b: String = format!("{}d{}", &a, "ef");

    println!("a={}", a); // a=abc
    println!("b={}", b); // b=abcdef

    // 通过format格式化一个字符串,然后输出
    println!("{}", format!("{}", 1) + "23"); // 123
}

fn 占位符输出() {
    // 占位符输出
    println!("{}{}", 1, "2"); // 12

    // 占位符位置输出
    println!("{1}{1}{1}{0}", 2, 1); // 1112

    // 占位符参数输出
    println!("{a}{b}{a}", a = "a", b = "b"); // aba
}

// 特定格式输出也是通过实现特质实现的
fn 特定格式输出() {
    // 二进制
    println!("{:b}", 10); // 1010
    // 八进制
    println!("{:o}", 10); // 12
    // 十六进制
    println!("{:x}", 10); // a
    // 十六进制大写
    println!("{:X}", 10); // A
}

fn 对齐输出() {
    // 对齐输出 宽度小于6,左边补空格,最最终宽度>=6
    println!("{a:>宽度$}", a = 1, 宽度 = 6);
    println!("{a:>宽度$}", a = 22, 宽度 = 6);
    println!("{a:>宽度$}", a = 88888888, 宽度 = 6);
    //     1
    //    22
    //88888888

    // 同上对齐输出,指定填充物为 *
    println!("{a:*>宽度$}", a = 1, 宽度 = 6); // *****1
}

fn 自定义类型和泛型打印() {
    // 自定义类型和泛型打印,可以使用Debug打印,也通过手动实现fmt::Display来打印
    let t = vec![1, 2, 3];
    println!("{:?}", t); // [1, 2, 3]
                         // println!("{:#?}",t);

    #[derive(Debug)]
    struct 测试(i32);
    println!("{:?}", 测试(007)); // 测试(7)
    println!("{:#?}", 测试(007));
    /*  美化打印
    测试(
        7,
    )
    */
}

fn 手动实现Display来打印() {
    struct 示例结构体 {
        t: String,
        tt: String,
    }
    impl std::fmt::Display for 示例结构体 {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            write!(f, "{}***{}", self.t, self.tt)
        }
    }
    let 结构体 = 示例结构体 {
        t: "abc".to_string(),
        tt: "efg".to_string(),
    };
    println!("{}", 结构体); //abc***efg
}

#[allow(non_snake_case)]
fn 手动实现Display来打印2() {
    struct 测试 {
        t: Vec<u32>,
        t2: String,
    }

    impl std::fmt::Display for 测试 {
        fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            let a = &self.t;
            write!(f, "t2={}\n", self.t2)?;
            for (i, d) in a.iter().enumerate() {
                write!(f, "t1编号={},t1值={}\n", i, d)?
            }
            write!(f, "")
        }
    }

    // 测试
    let temp = 测试 {
        t: vec![1, 2, 3],
        t2: "abc".to_string(),
    };

    println!("{}", temp)
}

fn main() {
    特定格式输出();
    println!("分割线--------------------------------");
    手动实现Display来打印2()
}

~~~



### 猜数字例子

~~~rust
use std::{cmp::Ordering, io::stdin};

use rand::Rng;

fn main() {
    let mut 标准输入 = String::new();
    
    let 生成的随机数 = rand::thread_rng().gen_range(1..101);

    loop {
        // 获取一行标准输入
        标准输入.clear();
        stdin()
            .read_line(&mut 标准输入)
            .expect("简单捕获标准输入错误。");

        // 一行标准输入转数字
        let 标准输入: i32 = match 标准输入.trim().parse() {
            Ok(n) => n,
            Err(_) => {
                println!("输入有误");
                continue;
            }
        };

        // 比较随机数和输入数是否相等
        match 标准输入.cmp(&生成的随机数) {
            Ordering::Less => println!("小于"),
            Ordering::Greater => println!("大于"),
            Ordering::Equal => {
                println!("等于");
                break;
            }
        }
    }
}

~~~







### 变量

#### 基本使用

~~~rust
// rust变量默认不可变
// 如果后续会更改,使用mut显式声明
let mut a = 0;	// 可修改 a = 1;
let a = 0; // 不可以修改

// 常量
// 常量不可以修改,需要标注数据类型
const A_A_A:u32 = 10*10;

// 变量遮盖
let a = 0;	// 输出0
{
    let a = 3.14;	// 输出3.14
}
println!("{}",a);	// 输出0
let a = "abc";	// 输出abc
~~~

#### 基本类型

~~~rust
fn 标准类型() {
    // 整数
        // 有符号整数,size是指针宽度
        // i8,i16,i32,i64,i128,isize
        // 无符号整数,size是指针宽度
        // u8,u16,u32,u64,u128,usize
    // 字符
        // char: 单个Unicode,每个都占用4字节。
        // 示例: 'a','@','呵'
    // 布尔
        // bool
        // 示例: true,false
    // 单元类型
        // (): 唯一的值就是()这个空元组

    // 整数的一些特殊写法
    // 末尾可以指定类型
    let a: u8 = 1u8;    
    println!("{}",a);   // 1
    // 可以用下划线怎加可读性
    let a: u32 = 100_00u32; 
    println!("{}",a);   // 10000
    // 8进制给值
    let a = 0o10;   
    println!("{}",a);   // 8
    // 2进制给值
    let a = 0b10;
    println!("{}",a);   // 2
    // 16进制给值
    let a = 0x10;   // 16
    println!("{}",a);
}


fn main() {
    标准类型();
    println!("-------------------------------");
    复合类型();
}

~~~



#### 复合类型

~~~rust
fn 复合类型() {
    // 数组
    let a = [1;5];  // 定义,初始化为长度5,值为1的数组
    println!("{:?}",a); // [1, 1, 1, 1, 1]
    let mut a: [&str; 3] = ["aaa","bbb","ccc"]; // 定义
    println!("{:?}",a); // ["aaa", "bbb", "ccc"]  
    a[0] = "aa";    //索引
    println!("{:?}",a); // ["aa", "bbb", "ccc"]
    let  b = &mut a[0..2];  // 数组创建切片
    b[1] = "bb";    // 修改切片，数组的值也会被修改
    println!("{:?}",b); // ["aa", "bb"]
    println!("{:?}",a); // ["aa", "bb", "ccc"]

    println!("-----------");

    // 元祖
    let mut a: (i32, &str, bool) = (1,"a",true);    //创建
    println!("{:?}",a); // (1, "a", true) 
    a.0 = 2;    //索引
    println!("{:?}",a); // (2, "a", true)
    let (b,c,_) = a;    // 解构
    println!("{}{}",b,c);   // 2a
    println!("{:?}",a); // (2, "a", true)
    
    println!("-----------");
    
    // 元祖结构体,相当于给元祖起了个名字
    // 同成员,不同名的元祖不可以当作同一种变量使用
    #[derive(Debug)]
    struct 测试(i32, bool);
    let a = 测试(1, false);
    println!("{:?}",a);
}
~~~





### 运算符

同C语言，除法运算会舍弃余数



### dbg

~~~rust
// 接受一个表达式的所有权，并且输出到标准错误，然后在返回表达式的所有权
// 调试打印,在发行版的时候这些代码不会被编译
dbg!(30*40);
dbg!(&(1,2));	// 也可以传引用进入
~~~





### 函数



~~~rust
// 作为函数参数时,实现copy特质的类型,会被拷贝后再传入
// 整数类型，比如 u32。
// 布尔类型，bool，它的值是 true 和 false。
// 浮点数类型，比如 f64。
// 字符类型，char。
// 数组，仅当其包含的类型也都实现 Copy 的时候。
// 元组，仅当其包含的类型也都实现 Copy 的时候。比如，(i32, i32) 实现了 Copy，但 (i32, String) 就没有。
fn a( a :i32,b:i32) ->u32 {
    if a>10{
        return  56
    }else if b>10 {
       return  4
    }
    // 最后一行可以不加retrun
    5  
}
~~~



### 流程控制

#### IF语句

~~~RUST
// if语句也是一个表达式,所以可以用来赋值
let number = if condition { 5 } else { 6 };
~~~



#### loop循环

~~~rust
loop {
    println!("死循环");
}
~~~



#### 循环标签

~~~rust
fn main() {
    let mut a = 0;
    '外层循环: loop {
        a += 1;
        loop {
            if a == 3 {
                println!("跳出外层循环");
                break '外层循环;
            } else {
                println!("跳出内层循环");
                break;
            }
        }
    }
}
~~~



#### 循环也可以返回值

~~~rust
fn main() {
    let a =  loop{

        if true{
            break 3	// 加不加分号无所谓
        }
        break 3; // 最后一行也需要使用break
    };

    println!("{}",a)
}
~~~



#### while循环

~~~rust
while true {
    println!("while循环");
}
~~~



#### for循环

~~~rust
let 数组 = [10, 20, 30, 40, 50];
for 元素 in 数组 {
    println!("{}", 元素);
}

// 输出1，2，3
for number in 1..4 {
    println!("{}", number);
}
~~~



### 所有权

- rust中每个值都有一个所有者，同时只能有一个所有者
- 当所有者离开作用域后值将被释放,drop
  - 离开 } 括号后会被释放
  
    - 包括结束单次循环
    - 包括离开 if 语句
  
    - 。。。



#### copy特质

**实现了copy特质的类型,在赋值时会拷贝数据,不会转移所有权**

- 整数类型，比如 u32。
- 布尔类型，bool，它的值是 true 和 false。
- 浮点数类型，比如 f64。
- 字符类型，char。
- 数组，仅当其包含的类型也都实现 Copy 的时候。
- 元组，仅当其包含的类型也都实现 Copy 的时候。比如，(i32, i32) 实现了 Copy，但 (i32, String) 就没有。



#### 引用与借用

- 所有者修改数据时会产生一个可变引用，读取数据时会产生一个不可变引用

- 任何引用的作用域，都只持续到最后一次使用

- 可变引用只能创建一个

- 不可变引用可以创建多个

- 两种引用不能同时存在





  ### 结构体



#### 定义结构体

~~~rust
#[derive(Debug)]
struct User {
    姓名: String,
    介绍: String,
    年龄: u8,
}

// 结构体可以没有变量
// 常常在你想要在某个类型上实现 trait 但不需要在类型中存储数据的时候发挥作用。
struct AlwaysEqual;
fn main() {
    let subject = AlwaysEqual;	// 实例
}
~~~



#### 创建实例

~~~rust
// 实例化结构体
// 实例化结构体,必须初始化所有字段,但是不需要顺序
let mut u1 = User{
    介绍:String::from("哈哈"),
    姓名:String::from("小明"),
    年龄:18,
};

// 两个结构体赋值，简写方法 
let  u2 = User{
    介绍:String::from("哈哈"),
    ..u1	// 剩下的值使用结构体u1赋值,必须写在最后一行
};

// 实例结构体，简写方法
fn new_user(姓名:String,介绍:String)->User{
    User{
        姓名:姓名,	// 简写前
        介绍:介绍,	// 简写前
        年龄:18,
    }
}
fn new_user(姓名:String,介绍:String)->User{
    User{
        姓名,	// 简写后
        介绍,	// 简写后
        年龄:18,
    }
}

~~~



#### 方法

~~~rust
#[derive(Debug)]
struct 四方形 {
    width: u32,
    height: u32,
}

// 为结构体增加方法
impl 四方形 {
    fn 面积(&self) -> u32 {	// &self 实际上是 self: &Self 的缩写
        self.width * self.height
    }
}

fn main() {
    let 四方形1 = 四方形 {
        width: 30,
        height: 50,
    };

    println!("四方形的面积= {}", 四方形1.面积());
}
~~~



#### 关联函数

~~~rust
#[derive(Debug)]
struct 四方形 {
    width: u32,
    height: u32,
}

impl 四方形 {
    fn 面积(&self) -> u32 {
        
        self.width * self.height
    }
    
	// 关联函数
    fn 创建四方形(h: u32,w :u32) -> 四方形 {
        四方形 {
            width: w,
            height: h,
        }
    }
}

fn main() {
    // 调用关联函数
    // 关联函数实际上与结构体没有关系
    let 四方形1 = 四方形::创建四方形(10, 20);	
    println!("四方形的面积= {}", 四方形1.面积());
}

~~~



###   枚举

#### 定义

~~~rust
// 格式
enum 枚举名称{
    成员名称,
    成员名称(成员值),
    ...
}

// 示例
#[derive(Debug)]
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}

// 枚举添加方法
impl Message {
    fn call(&self) {
        match self {
            Self::Quit => println!("quit"),
            Self::Write(tt) => println!("枚举是Write,值是={:?}",tt),
            Self::Move{x,y} => println!("枚举是Write,值是="),
            _ => println!("其他枚举"),
        }
    }
}
~~~



#### match

~~~rust
// 格式
// 必须匹配所有可能性
match 枚举实例 {
    成员名称 => {
        匹配成功执行的函数	
    },
    成员名称(接收的成员值) => {
        匹配成功执行的函数	
    },
    成员名称{接收结构体的值} => {
        匹配成功执行的函数	
    },
    _ => println!("其他枚举"),	// _ 匹配剩下的所有可能性,同时只有一行可以不写花括号
}


// 示例
match self {
    Self::Quit => println!("quit"),
    Self::Write(tt) => println!("枚举是Write,值是={:?}",tt),
    Self::Move{x,y} => println!("枚举是Write,值是="),
    _ => println!("其他枚举"),
}
~~~



#### if let

~~~rust
// 格式
if let 需要匹配的成员名称(如果成员有值,在这里接收) = 需要匹配的枚举实例 {
    匹配成功执行
}else{
    匹配失败执行
}

// 创建一个枚举
let a: Message = Message::ChangeColor(1, 2, 3);
// 使用 if let 进行简单的匹配
if let Message::ChangeColor(0,b,c) = a {
    println!("{}{}",b,c)
}else{
    println!("没有匹配到！")
}

// 示例2,接收值时不转移所有权,加上一个 ref 接受到的就是借用
let  b= Message::Write("abc".to_string());
if let Message::Write(ref 接收到的字符串) = b {
    println!("{}",接收到的字符串);
}
~~~



  

### 项目管理

- 每一个包都是一个独立的单元
- 一个项目中只允许存在一个库包，但是可以存在多个二进制包，放在src目录下的bin目录下
- 可以使用相对路径导入或者绝对路径导入
  - 绝对路径 crate 开头
  - 相对路径，直接模块名开头或者 super 和 self 开头
- 使用 use 引入模块 
- 使用pub声明的模块才能被外部访问
  - 子模块可以访问父模块下的私有内容
  - 父模块不可以访问子模块的私有内容
  - 结构体每个成员值需要单独 pub 声明,枚举不需要
- 分文件编写时，每个文件或目录都是一个mod，需要导入到目录下的mod.rs，再导入到lib.rs





### 集合

#### vector

~~~rust
// 创建
let mut a: Vec<i32> = Vec::new();
let mut a = vec![1, 2, 3];

// 添加数据
a.push(4);

// 获取数据 get
match a.get(0) {
    Some(data) => println!("{}", data),
    None => println!("没有获取到数据"),
}
// 获取数据，索引
println!("{}", a[0]);

//遍历
for i in &mut a {
    *i += 1;
}
for i in &a {
    println!("{}", i)
}


// 使用枚举存储不同类型
enum SpreadsheetCell {
    Int(i32),
    Float(f64),
    Text(String),
}
let row = vec![
    SpreadsheetCell::Int(3),
    SpreadsheetCell::Text(String::from("blue")),
    SpreadsheetCell::Float(10.12),
];
~~~



#### 字符串

~~~rust
// 常用的字符串创建方法
let s = "ab".to_string();
let s = String::new();
let mut s = String::from("ab");

// 拼接字符串
// 可以使用格式化参数,format!()
s.push_str("cd");
s.push('e');
s += "f";
s+= &"中".to_string();    // 被加的需要是地址

// 遍历字符串
// 字符串不支持 [0] 索引操作方式
for i in s.chars() {
    println!("{}",i)    // 遍历单个字符
}
for i in s.bytes() {
    println!("{}",i)    // 遍历单个字节的数据
}

// 获取字符串常用数据
// 获取字节数
println!("{}",s.len());

println!("{}",s)
~~~



#### map

~~~rust
use std::collections::HashMap;

fn main() {
    // 创建hashmap
    // hashmap数据是乱序的
    let mut s = HashMap::new();

    // 增加数据,key重复的数据会变成修改
    s.insert("a".to_string(), 123);
    s.insert("a".to_string(), 223);
    s.insert("b".to_string(), 223);

    //获取数据
    match s.get("a") {
        Some(data) => println!("{}",data),
        None => println!("没有找到数据")
    } 

    // 遍历
    for (key, value) in &s {
        println!("{}: {}", key, value);
    }

    println!("{:?}", s);
}

~~~



### 错误处理

#### 终止

~~~toml
# 程序出错时不在展开清理程序，交有操作系统终止，这可以缩小二进制程序的大小
[profile.release]
panic = 'abort'
~~~



#### 可以恢复错误

#### 简单处理

~~~rust

// unwrap 如果出错直接panic,如果成功返回ok值
let f = File::open("hello.txt").unwrap();
// expect 同上,可携带错误信息
let f = File::open("hello.txt").expect("同上,可携带错误信息");

// 常用函数自己查
unwrap_or_else(|error| {

~~~

#### 错误传播

~~~rust
fn 打开文件() ->Result<File,io::Error>{
   	// ? 返回错误时将错误继续返回,成功时返回ok值继续执行
    let f = File::open("hello.txt")?;
    Ok(f)
}
~~~



### 泛型

~~~rust
// 结构体使用泛型
#[derive(Debug)]
struct 测试<泛型类型1, 泛型类型2> {
    变量1: 泛型类型1,   // 同名泛型,变量类型需要相同
    变量2: 泛型类型1,   // 同名泛型,变量类型需要相同
    变量3: 泛型类型2,
}


// 泛型方法
impl<换个名字1, 换个名字2> 测试<换个名字1, 换个名字2> {
    fn a(&self,s:换个名字2) -> &换个名字1 {
        // println!("{:?}",s);
        &self.变量1
    }

    fn b(&self) -> &换个名字2 {
        &self.变量3
    }
}

// 泛型中规定特定类型能使用的方法
impl 测试<i32,i32> {
    fn aa(&self) -> &i32 {
        &self.变量1
    }
}

// 方法也带泛型参数
impl <AA,BB>测试<AA,BB> {
    fn aaa<CC,DD>(self,传入 :测试<CC,DD>) -> 测试<AA,DD> {
        测试 { 变量1: self.变量1, 变量2:self.变量2 , 变量3: 传入.变量3 }
    }
}


fn main() {

    let a: 测试<i32, String> = 测试 {
        变量1: 1,   // 同名泛型,变量类型需要相同
        变量2: 1,   // 同名泛型,变量类型需要相同
        变量3: "abc".to_string(),
    };
    println!("{:?}", a);
    println!("{}", a.a("aa".to_string()));
    // println!("{}", a.a());
    println!("{}", a.b());


    let b:测试<i32, i32>  = 测试 {
        变量1: 1,   
        变量2: 1, 
        变量3: 1,
    };
    println!("{}",b.aa());  // aa 方法限定<i32,i32> 才可以使用


    let c = a.aaa(b);
    println!("{:?}",c)
}

~~~



### 特质

提供一个函数签名，其他类型可以实现它。表示不同类型有相同的功能。

~~~rust
use std::fmt::Debug;

// 创建特质
pub trait 特质 {
    fn 特质需要实现的方法1(&self, s: String) -> String;

    // 提供了实现的特质，可以不实现,也可以实现，会覆盖默认特质
    fn 特质需要实现的方法2(&self, s: String) -> String {
        format!("{}", self.特质需要实现的方法1(s) + "-->特质2")
    }
}

pub trait 特质2 {
    fn 特质2需要实现的方法1(&self) -> String;
}

struct 结构体1 {
    name: String,
}

// 为结构体实现特质
impl 特质 for 结构体1 {
    fn 特质需要实现的方法1(&self, s: String) -> String {
        format!("{}是{}", self.name, s)
    }
    fn 特质需要实现的方法2(&self, s: String) -> String {
        format!("{}是{}", self.name, s + "-->覆盖了默认实现的特质")
    }
}

// 为结构体实现第二个特质
impl 特质2 for 结构体1 {
    fn 特质2需要实现的方法1(&self) -> String {
        format!("这是特质2需要里面的方法1")
    }
}

struct 结构体2 {
    代号: i32,
}

// 为结构体实现特质
impl 特质 for 结构体2 {
    fn 特质需要实现的方法1(&self, s: String) -> String {
        format!("{}号是{}", self.代号, s)
    }
}

// 特质作为参数可以传入实现的特质的类型
fn 特质作为参数传入函数(tz: &impl 特质) {
    println!("{}", tz.特质需要实现的方法1("动物".to_string()));
}

// 使用 + 号连接需要实现的特质
fn 特质作为参数传入函数_2(tz: &(impl 特质 + 特质2)) {
    println!("{}", tz.特质需要实现的方法1("动物".to_string()));
    println!("{}", tz.特质2需要实现的方法1())
}

// impl 只是一个语法糖，特质真正的写法
fn 特质作为参数传入函数_3<T: 特质 + 特质2, U: Clone + Debug>(tz: &T, u: U) {
    println!("特质传入函数3：{}-->{:#?}", tz.特质2需要实现的方法1(), u)
}

// 需要的实现的特质太多可以这样写
fn 特质作为参数传入函数_4<T, U>(tz: &T, u: U) -> i32
where
    T: 特质 + 特质2,
    U: Clone + Debug,
{
    println!("特质传入函数4：{}-->{:#?}", tz.特质2需要实现的方法1(),u);
    1
} 

// 特质作为返回值
fn 特质作为返回值()->impl 特质{
    let a =0;
    if a>0{
        return  结构体1{
            name : "asb".to_string(),
        }
    }
    // else // 虽然都实现的 特质 不过rust不允许一个函数中返回不同的类型
    // {
    //     结构体2{
    //        代号: 09,
    //     }
    // }


    结构体1{
        name : "asb".to_string(),
    }
}


fn main() {
    // 实例结构体
    let a = 结构体1 {
        name: "小明".to_string(),
    };

    let b = 结构体2 { 代号: 7 };

    // 调用特质
    println!("{}", a.特质需要实现的方法1("人".to_string()));
    println!("{}", b.特质需要实现的方法1("人".to_string()));
    println!("{}", a.特质需要实现的方法2("人".to_string()));
    println!("{}", b.特质需要实现的方法2("人".to_string()));

    // 特质作为参数传入函数
    特质作为参数传入函数(&a);
    特质作为参数传入函数_2(&a);
    特质作为参数传入函数_2(&a);
    // 特质作为参数传入函数_2(&b);	// 无法调用，b只实现了特质，没有实现特质2
    特质作为参数传入函数_3(&a,007);
    特质作为参数传入函数_4(&a,008);

    // 特质作为返回值
    println!("{}",特质作为返回值().特质需要实现的方法1("啊啊啊".to_string()));  
}



// 只为实现了某些特质类型实现方法
use std::fmt::Display;

struct Pair<T> {
    x: T,
    y: T,
}

impl<T> Pair<T> {
    fn new(x: T, y: T) -> Self {
        Self {
            x,
            y,
        }
    }
}

// 只有 T 实现了 Display + PartialOrd 这两个特质时才可以使用该方法
impl<T: Display + PartialOrd> Pair<T> {
    fn cmp_display(&self) {
        if self.x >= self.y {
            println!("The largest member is x = {}", self.x);
        } else {
            println!("The largest member is y = {}", self.y);
        }
    }
}
~~~



### 手动标注生命周期

#### 说明

- rust中的每个引用都有自己的生命周期

- 生命周期 是 引用的有效作用域

- 大多数情况下，生命周期是可以被编译器自动推断的，不需要自己写

- 当引用的生命周期可能以不同的方式互相关联时，需要手动标注生命周期 ？？？

- 生命周期标注不会改变引用的实际生命周期
- 当指定了泛型生命周期参数，函数可以接受带有如何生命周期的引用？？？



#### 语法

- ' 号开头
- 放在引用符号 & 后面
- 使用空格将 标注 与 类型分开
- 函数声明周期放在函数名与参数列表中间，用 < > 包裹  
- 建议全小写，短一点
- 很多人使用 'a 

#### 示例

~~~rust
// 标注生命周期后，返回值的生命周期，以生命周期最短的数据为准
// 这里 'a 的生命周期实际上就是, x 和 y 中短的那个 
fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}

fn main() {
    let string1 = String::from("aaaaaaaa");
    let result;
    {
        let string2 = String::from("bbb");
        result = longest(string1.as_str(), string2.as_str());
    }
    
    // 返回的是string1,string1实际上没有被释放，但是string2被释放了，所以string1同样不可用
    println!("The longest string is {}", result);
}

~~~



#### 什么时候可以省略标注

1. 每个引用类型的参数都自己的生命周期
2. 如果只有一个输入参数，那么这个生命周期会被赋值给所有输出生命周期
3. 即使有多个输入生命周期，但只要有输入生命周期是 &self 或 &mut self，那么 self 的生命周期会被赋值给所有输出生命周期






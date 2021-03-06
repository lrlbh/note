## python基础

### 变量

#### 三个特征

1. ID 
   - 变量 ***值*** ，在内存中的地址
   - 使用 ***id()*** 函数，获取地址
2. type
   - 变量 ***值*** ，的类型
   - 使用 ***type()*** 函数，获取变量类型
3. value
   - ***值***，具体保存的数据



#### 基本数据类型

##### 数值型

1. 整型		int
2. 浮点型    float

##### 字符串类型

1. 单引号  

2. 双引号 

   - 双引号中可以保护单引号

   - ~~~python
     msg = "My name is Tony , I'm 18 years old!"
     ~~~

   - 

3. 三引号

   - 三引号可以换行编写

   - ~~~python
     msg = '''
             天下只有两种人
             照例第一种人应该乐观
             不过事实却适得其反
           '''
     
     ~~~

     

##### 列表 list

- 说明

  - 类似数组，但是无需指定长度，可以存储任意数据类型

- 使用

  - 使用中括号定义

    - ~~~python
      list_1 = ['张三','李四','王五']
      list_2 = []
      ~~~

      

  - 可以嵌套，类似多维数组

    - ~~~python
      list_1 = [['张三',17],[True,19]]
      list_1[1][1] # 取出数字 19
      ~~~

    - 



##### 字典 dict

- 说明

  - 使用 **key : value** 的方式存储数据

- 使用大括号定义

  - ~~~python
    dict_1 = {"name":'tony','age':18,44:22}
    dict_1 = {}
    
    dict_1["name"] #取出name对应的值
    ~~~

    

  - value 可以嵌套使用，key 不可以

##### 布尔类型

- True 为真，False 为假









### 基本数据类型的内置方法

#### 数值型

- ***int()***  字符串 转 整形

  - ~~~python
    # 十进制 字符串 转 整形
    int("123") # 输出 123
    
    # 二进制格式 字符串 转 整形 
    int("0b110111100",2) # 输出结果 444 
    int("110111100",2) # 输出结果 444
    
    # 八进制格式 字符串 转 整形 
    int('0o11',8) # 输出接过 9
    
    # 十六进制格式 字符串 转 整形 
    int("0xffff",16) # 输出结果 65535
    ~~~

  

- 进制转换  

  - ~~~ python
    # 整形 转 二进制字符串
    bin(444) # 输出结果 0b开头的字符串 '0b110111100'
    
    # 整形 转 八进制字符串
    oct(9) # 输出为 0o开头的字符串 '0o11'
    
    # 整形 转 十六进制字符串
    hex(17) # 输出为 0x开头的字符串 0x11
    ~~~



#### 字符串

- str()  类型转换

  - ~~~python
    # 所有数据类型都可以转字符串
    
    # 列表 转 字符串
    str({"123",456}) # 输出结果 "{456, '123'}"
    ~~~

- 索引操作

  - ~~~python
    # 取值
    "abc"[0] # 输出 'b'
    
    # 反向取值
    "abc"[-1] # 输出 'c'
    
    # 切片
    "abcdef"[0:2] # 输出 'ab'
    
    # 步长切边
    "abcdef"[0:4:2] # 输出 'ac'
    
    #反向切片 逆序字符串
    "abc"[::-1] # 输出 cba
    
    ~~~

- 内置方法

  - ~~~	python
    #内置方法使用  temp_str.  的方式调用
    
    # 删除左右两边指定字符串
    "BDBDaaBD".strip("BD") # 输出 'aa'
    "  aa  ".strip() # 输出 'aa'   
    
    # 删除两边边字符串,同上
    lstrip() # 删除左边
    rstrip() # 删除右边
    
    
    # 分割字符串
    " a a a ".split() # 输出结果 ['a', 'a', 'a']
    "a.b.c".split(".") # 输出结果 ['a', 'b', 'c']
    "a.b.c".split(".",1) # 输出结果 ['a', 'b.c']
    "a.b.c".rsplit(".",1) # 反向分割 ['a.b', 'c']
    
    # 英文字符 全大写 全小写
    lower()  # 变小写
    upper()  # 变大写
    
    # 判断字符串是否已指定字符开头
    "ASD_123".startswith("ASD_") # 输出结果 True
    "ASD_123".endswith("123") # 输出结果 True
    
    #格式化字符串
    # 方式1,名称获取
    "name={name},name={name},age={age}".format(age=18,name ='tony')
    # 输出结果
    'name=tony,name=tony,age=18'
    # 方式2，把传入值当做列表，通过索引获取
    "name={0},age={1}".format("tony",12,"213") 
    # 输出结果
    'name=tony,age=12'
    
    # 拼接字符串
    "-->".join("BBB") # 输出结果 'B-->B-->B'
    "-->".join(["lr","lt","lsl"]) # 输出结果 'lr-->lt-->lsl'
    
    # 替换指定字符串
    "abb".replace("b","B") # 返回结果 'aBB'
    "abb".replace("b","B",1) # 返回结果 'aBb'
    
    # 判断字符串是否由纯数字组成
    "123".isdigit() # 返回值 True
    
    # 查找指定指定字符串是否存在，存在则返回第一次出现的下标，不存在返回 -1
    "GHHhh".find("h",0,4) # 查找下标0-3中是否存在字符“h” 返回 3
    
    # 统计某个字符串出现的次数
    "GGHH".count("H",0,3) # 返回 1
    "GGHH".count("H") # 返回 2
    
    # 字符串居中
    
    ~~~



#### 列表 []

- 类似数组

- list() 类型转换

  - ~~~python
    '''
    但凡能被for循环遍历的数据类型都可以传给list()转换成列表类型
    list()会跟for循环一样遍历出数据类型中包含的每一个元素然后放到列表中
    '''
    
    # 字符串 转 列表
    list("ABC")  # 返回 ['A', 'B', 'C']
    
    # 字典 转 列表
    list({"name":"lsl","age":15}) # 返回 ['name', 'age']
    
    
    ~~~

    

- 索引操作

  - ~~~python
    # 取值
    ['a','b',3][0] # 返回 'a'
    
    # 修改
    list_1 = ['A','B',3,4]
    list_1[0] = 'AB'
    
    # 反向取值
    ['a','b',3][-1] # 返回 3
    
    # 切片
    ['A','B',3,4][0:2] # 返回 ['A', 'B']
    
    # 步长切边，[0:4:2],0表示起始位置，4表示切片长度，2表示几个中留一个
    ['A','B',3,4,5,6][0:5:2] # 返回 ['A', 3, 5]
    
    #反向切片 逆序列表
    ['A','B','C'][::-1] # 返回 ['C', 'B', 'A']
    ~~~

- 内置方法

  - ~~~ python
    # 尾插 插入一个元素 可以插入任意元素
    [1,2].append('a') # 插入一个字符串元素 
    [1,2].append(['a',1]) # 插入一个列表元素
    [1,2]append({"name":"lr","age":16}) # 插入一个字典元素 
    
    # 批量尾插 （拼接列表）
    [1,2].extend([3,4])  
    
    #指定位置 插入一个元素 可以插任意元素
    [2,3].insert(0,"A") 
    
    # 删除指定元素
     del [1,2,3][0]
    
    # 删除指定元素，默认删除最后一个
    [1,2,3].pop(0) # 返回 1
    [1,2,3].pop() # 返回 3
    
    # 通过 元素值 删除元素
    ["1","2","3"].remove("1")
    
    # 逆序列表
    [1,2,3].reverse() # 逆序当前列表 无返回
    
    # 字符的大小取决于 ASCII 中的先后顺序
    # 排序 元素类型相同才可以排序
    [32,345,44].sort() # 排序当前列表 无返回
    
    # 列表比较大小，从第一个元素开始比较，只要分出大小就结束
    [1,2,3,50] > [1,2,4] # 返回 False
    
    
    
    
    ~~~



#### 元祖组 ()

- 元组和列表相同，当时元组中的元素不允许改变

- 定义

  - ~~~python
    temp = ("1",2,3)
    
    # 特别注意 元组中只有一个元素是需要在第一个元素后面加个 ， 作区分
    temp = ("1",)
    ~~~

    

- tuple() 类型转换

  - ~~~python
    '''
    但凡能被for循环遍历的数据类型都可以传给tuple()转换成列表类型
    tuple()会跟for循环一样遍历出数据类型中包含的每一个元素然后放到列表中
    '''
    
    # 字符串 转 元组
    tuple("ABC")  # 返回 ('A', 'B', 'C')
    
    # 字典 转 元祖
    tuple({"name":"lsl","age":15}) # 返回 ('name', 'age')
    ~~~

  

- 索引操作

  ~~~python
  # 正向取值
  (1,2)[0] # 返回 1
  
  # 反向取值
  (1,2)[-1] # 返回 2
  
  # 元祖的元素不能修改
  
  # 切片
  (1,2,3)[0:2] # 返回 (1, 2)
  (1,2,3,4)[0:3:2] # 返回 (1, 3) 从下标0开始，到下标2结束,没两个元素保留一个
  ~~~



#### 字典 {}

- 字典以 key:value 形式保存数据

- 定义

  ~~~python
  dict_1 = {"name":'tony','age':18, 44:22}
  dict_1 = {}
  
  dict_1["name"] #取出name对应的值
  ~~~



- 类型转换

  ~~~python
  dict([['name','tony'],('age',18)]) # 返回 {'name': 'tony', 'age': 18}
  
  {}.fromkeys(('name','age',),None) # 返回 {'name': None, 'age': None}
  
  ~~~



- 索引操作

  ~~~python
  # 取值
  {"name":"lr","age": 18}["name"] # 返回 'lr'
  {"name":"lr","age": [12,14]}["age"][0] # 返回 12
  
  # 赋值
  {"name":"lr"}["age"] = 18 # 如果 key 不存在则创建一个新的元素
  {"name":"lr"}["name"] = "lsl" # 如果 key 存在则修改value的值
  
  ~~~



- 内置方法

  ~~~python
  # 通过 key 删除
  {"name":"lr","age":16}.pop("name") # 返回 'lr'
  
  # 取值 
  {"name":"lr","age":14}.get("name") # 返回'lr'
  {"name":"lr","age":14}.get("name1") # 返回 None
  {"name":"lr","age":14.get("name1",-1) # 返回 -1 可以设置不在时的返回值
   
  # 尾删
  {"name":"lr","age":14}.popitem() # 返回 ('age', 14)
   
  # 更新字典，有则修改，无则添加
  {"name":"lr","age":18}.update({"age":12,"sex":"男"})
  
  # 设置 一个元素
  # 没有这个元素则添加
  {"name":"lr","age":18}.setdefault('sex',"男") # 返回 '男'
  # 有这个元素则返回 key 对应的 value
  {"name":"lr","age":18}.setdefault('age',14) # 返回 18
   
   
  # 获取所有key,可以用于for循环遍历
  {"name":"lr","age":16}.keys() # 返回 dict_keys(['name', 'age'])
  # 获取所有value,可以用于for循环遍历
  {"name":"lr","age":16}.values() # 返回 dict_values(['lr', 16])
  # 获取所有 键值对,可以用于for循环遍历
  {"name":"lr","age":16}.items() # 返回 dict_items([('name', 'lr'), ('age', 16)])
  # 遍历
  for item in {"name":"lr","age":12}.items():
  	print(item)
  # 输出
  ('name', 'lr')
  ('age', 12)
  
  
  ~~~

  



#### 集合 {}

- 主要用于去重、关系运算

- 定义

  ~~~python
  # 空集合
  my_set = set()
  
  # 定义
  my_set={1,"2"}
  ~~~

- 类型转换

  ~~~python
  '''
  但凡能被for循环遍历的数据类型都可以传给set()转换成集合类型
  '''
  
  # 字符串 转 集合
  set("ABC")  # 返回 {'A', 'B', 'C'}
  
  # 字典 转 集合
  set({"name":"lsl","age":15}) # 返回 {'age', 'name'}
  ~~~

  





















### 流程控制

#### 分支结构

PASS

#### 循环结构

- 语法

- for 变量名 in 可迭代对象 :

- Dome：

  ~~~python
  for item in ['a','b','c']:
      print(item)
  # 运行结果
  a
  b
  c
  ~~~





### 运算符

#### 成员运算符

- in 判断一个对象，是否被另外一个对象包含，包含会Ture

  - ~~~python
    # 字符串“123” 中是否包含 字符串“1”
    "1" in "123"  # True
    
    # 列表中是否包含指定对象
    list_1 =["asd","dsa",11]
    11 in list_1 # True
    "asd" in list_1 # True
    "asdd" in list_1 # False
    ~~~

- not in 判断一个对象，是否被另外一个对象包含，不包含返回Ture



#### 身份运算符

- is 判断 两个对象地址是否相同，身份运算符是唯一已知的，进行地址比较的运算符



- is not   相同返回false  不通返回ture



#### 位运算

- & : 按位与
  - 同1为1，其他为0，常用于将某几位置0

- |：按位或
  - 同0为0，其他为1，常用于将某几位置1 



### 文件操作

#### 文件操作权限

~~~python
r	只读不存在则创建
w	只写不存在则创建
a	追加不存在则创建

~~~





#### 打开文件

~~~python
# 文本方式打开文件
f = open('a.txt','r',encoding="utf-8")

# 二进制方式打开文件
f = open("a.txt",mode="rb")


~~~



#### 读文件

~~~python
# 文本模式，读取所有内容，返回 字符串
res = f.read()

# 二进制方式，读取所有内容 返回 bytes 数组
res = f.read()

# 读取指定长度的内容，文本模式单位是字符，二进制模式单位是字节，字符串长度为0表示读完了
res = f.read(1024)

# 文本模式，读取每一行内容,存放于列表中
 res = f.readlines()

# 文本模式，读取一行内容,光标移动到第二行首部，返回字符串
res = f.readlines()


~~~



#### 写文件

~~~ python
# 文本模式，返回的是写入的字符个数
f.write("啊啊12") # 返回 4

# 二进制模式，返回的是写入的字节数
f.write(b'\xff\x0f') # 返回 2

# 文本模式，列表写入
f.writelines(['333\n','444\n'])  # 文件模式
~~~



#### 文件指针操作

~~~python
'''
f.seek(0,0)
参数1：移动多少个字节或字符（二进制模式移动字节，文本模式移动字符）
参数2：模式
	0：文件起始位置开始移动
	1：从当前位置开始移动，只有二进制模式可以使用
	2：从末尾开始移动，只有二进制模式可以使用
''' 
~~~





#### 关闭文件

~~~python
# 关闭文件
f.close()
~~~



#### 其他

~~~python
f.readable()  # 文件是否可读
f.writable()  # 文件是否可读
f.closed  # 文件是否关闭
f.encoding  # 如果文件打开模式为b,则没有该属性
f.flush()  # 立刻将文件内容从内存刷到硬盘
f.name
~~~



### 函数

函数也是对象

函数名代表地址



#### 函数使用

~~~python
# 定义
def f(name,age):
    print("name={0} age={1}".format(name,age))

#调用
a("lr",14)
a(age=14,name="lr")
a('lr',age='123')
~~~



#### 默认参数

~~~python
'''
1、有默认值的参数必须在末尾,有可变长度参数时，可以在可变长度参数前面
2、参数的默认值，只在定义时被赋值一次
'''

# 定义
x = 14
def f(name,age=x):
    print("name={0} age={1}".format(name,age))
x = 18

# 调用
f("lr")	# 输出name=lr age=14
f("lr",16) # 输出 name=lr age=16
~~~



#### 可变长度参数 列表

~~~python
'''
1、可变参数可以传0-n个值，函数内部使用列表接收
2、可变参数需要在末尾
'''

# 定义 
def f(name,age=14,*argv):
	print("name={0} age={1} argv={2}".format(name,age,argv))

# 调用    
f("lr",1,2,3) # 输出 name=lr age=1 argv=(2, 3)
f("lr",*[1,2,3]) # 输出 name=lr age=1 argv=(2, 3)
f("lr",1,[1,2,3]) # 输出 name=lr age=1 argv=([1, 2, 3],)
~~~



#### 可变长度参数 字典

~~~python
'''
1、可变参数可以传0-n个值，函数内部使用字典接收
2、可变参数需要在末尾
'''

# 定义
def f(name,age=14,**kv_agrv):
    print("name ={0} age={1} kv_agrv={2}".format(name,age,kv_agrv))

#调用
f("lr",14,aa="a") # 输出 name =lr age=14 kv_agrv={'aa': 'a'}
f("lr",18,aa = "a",bb="b") # 输出 name =lr age=18 kv_agrv={'aa': 'a', 'bb': 'b'}
f("lr",**{"aa":"a", "bb":"b"}) # 输出 name =lr age=14 kv_agrv={'aa': 'a', 'bb': 'b'} 
~~~





#### 强制使用关键字传参

~~~python
# 定义 
# * 号后面的参数必须使用关键词调用，否则报错
def f(name,age=18,*,sex,height):
    print("name ={0} age={1} sex={2} height={3}".format(name,age,sex,height))

# 调用
f("lr",sex = "man" ,height = 1.76) # 输出 name =lr age=18 sex=man height=1.76 
~~~



#### 装饰器

在不修改源函数的情况下 给函数添加功能



##### 无参装饰器

~~~python
 '''
 无参装饰器的实现，装饰器原理
 '''
  
 import time

# 装饰器 给源函数添加 功能
 def print_test():
     print("test")
     return "tt"

 # 装饰器函数                                    
 def f(func):
        
     # 给写好的函数添加功能
     def ff():
            
        # 添加打印时间的功能
        print("当前时间戳：{0}".format(time.time()))
        
         # 调用源函数
         res = func()
        
         # 返回源函数返回结果
         return res
    
     # 装饰器 返回内部函数
     return ff

 # 修改源函数的地址
 print_test = f(print_test)

 # 调用源函数
 print(print_test())
~~~

~~~python
'''
装饰器语法实现
'''
import time
from functools import wraps

# 装饰器 给源函数添加 功能
def print_name_time(func):

    # 文档注释之类的信息跟随
    @wraps(func)
    
    #内部函数写要添加的功能
    def f(*argv,**kv_argv):
        
        # 添加打印时间的功能
        print("当前时间戳：{0}".format(time.time()))
        
        # 调用源函数，并且返回源函数的返回值
        return func(*argv,**kv_argv)
    
    # 返回内部函数的地址
    return f


# 装饰器语法糖 替代修改源函数地址的步骤 print_name = print_name_time(print_name)
@print_name_time 
# 源函数
def print_name(name,age):
    print("name ={0} age = {1}".format(name,age))
    return "ok"

# 调用
print(print_name(name = "lr", age = 14))
~~~









### 名称空间与作用域

名称空间的加载顺序是：内置名称空间->全局名称空间->局部名称空间，

名称空间的查找顺序是：局部名称空间->全局名称空间->内置名称空间。



```python
# 局部中 使用 全局变量 
global x
```





### 模块的导入语法

~~~python
# 使用都需要加前缀 模块名.
import 模块名

# 可以直接使用模块内部名称
from 模块名 import 模块内名称1,模块内部名称2
# 导入所有内部名称，可以由 __all__控制，# __all__=['模块内名称1','模块内部名称2'] 
from 模块名 import *

# 给模块起别名
import 模块名 as xx




~~~




















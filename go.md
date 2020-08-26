





## Go基础

### 编译

~~~shell
go build go文件
~~~



### go env(环境)

~~~shell
set GO111MODULE=on
set GOARCH=amd64 #生成二进制文件的架构
set GOBIN=
set GOCACHE=C:\Users\Administrator\AppData\Local\go-build
set GOENV=C:\Users\Administrator\AppData\Roaming\go\env
set GOEXE=
set GOFLAGS=
set GOHOSTARCH=amd64
set GOHOSTOS=windows
set GOINSECURE=
set GONOPROXY=example.com/org_name
set GONOSUMDB=example.com/org_name
set GOOS=linux #生成二进制文件的平台
set GOPATH=D:\zm\project\go #GO语言工作目录
set GOPRIVATE=example.com/org_name
set GOPROXY=https://goproxy.cn,direct
set GOROOT=C:\lr\kf_exe\go #GO语言安装目录
set GOSUMDB=sum.golang.org
set GOTMPDIR=
set GOTOOLDIR=C:\lr\kf_exe\go\pkg\tool\windows_amd64
set GCCGO=gccgo
set AR=ar
set CC=gcc
set CXX=g++
set CGO_ENABLED=1
set GOMOD=NUL
set CGO_CFLAGS=-g -O2
set CGO_CPPFLAGS=
set CGO_CXXFLAGS=-g -O2
set CGO_FFLAGS=-g -O2
set CGO_LDFLAGS=-g -O2
set PKG_CONFIG=pkg-config
set GOGCCFLAGS=-fPIC -m64 -pthread -fmessage-length=0 -fdebug-prefix-map=C:\Users\ADMINI~1\AppData\Local\Temp\go-build611383410=/tmp/go-build -gno-record-gcc-switches

#生成二进制文件的平台
GOOS=linux; 
#生成二进制文件的架构
GOARCH=arm; 
#是否使用CGO 1为使用
CGO_ENABLED=1; 
#
CC=gcc

CXX=g++

~~~







### 内建数据类型

#### 分类

- 值类型
  - int
  - float
  - bool
  - string
  - 数组
  - struct
- 引用类型
  - 指针
  - slice
  - map
  - chan

#### string的常用操作

~~~go
//返回字符串的长度
len()

//返回格式化字符串
fmt.Sprintf()

//分割字符串，返回一个string切片
ret := strings.Split("abc@abc","@")

//查看字符串是否包含某个子串，返回bool
ret2 := strings.Contains("abcdefg","fg")

//是否已某个子串开头,返回bool
ret2 = strings.HasPrefix("abcdefg","abc")

//是否已某个子串结尾,返回bool
ret2 = strings.HasSuffix("abcdefg","efg")

//返回子串第一次出现的下标，输出0
ret3 := strings.Index("abcabc","abc")

//返回子串最后一次出现的下标，输出3
ret3 = strings.LastIndex("abcabc","abc")

//使用某个子串连接切片，返回字符串
str2 := strings.Join([]string{"1","2","3","4"},"+")


~~~



### 内建函数

~~~go
//获取数据的长度
func len(v Type) int
返回值：返回传入数据的长度
参数：
	v Type:
		数组名
		指针名
		切片名称


~~~









## 密码



### 对称加密

#### 说明

- 加解密使用相同的秘钥
- 相当于非对称加密来说，加密效率和安全性高



#### 常见算法

- DES	
- 3DES
- AES



#### AES

##### 秘钥长度

go语言中：长度可选择16、24、32(byte)字节，用以选择AES-128、AES-192、AES-256。



##### 分组

- AES加密采用分组加密模式，分组的长度为16(byte)字节一组
- 加密时，最后一组数据，不够16个字节的不够几个填充几个，填充字节的值为，不够的字节数。
- 加密时，刚好16个字节的，填充16个字节，填充字节的值为，16。



##### 初始化向量

初始化向量，加解密时必须使用相同的数据，长度和分组的长度一样。



##### 加密模式

某些加密模式不需要采用初始化向量和分组填充。这里采用的CBC模式，需要填充和初始化向量。



##### 示例

- 封装好的对象

~~~go
package EAD

import (
	"crypto/aes"
	"crypto/cipher"
	"errors"
	"math/rand"
)

//加密时使用的默认参数
var (
	GROUP_SIZE = 16  //对称加密时分组的长度,单位字节
	AES_KEY_SIZE = 32 //aes秘钥的长度，单位字节  --长度只能是16、24、32，用以选择AES-128、AES-192、AES-256。
)


//AES对象
type Aes struct{
	sKey []byte	//生成秘钥的字符串
	iv []byte	//随机向量
	oKey cipher.Block	//AES接口
	fEnc cipher.BlockMode	//加密接口
	fDec cipher.BlockMode	//解密接口
}

//创建一个对称加密对象
//传入非空值，将会根据传入数据加载秘钥，放弃随机生成
func NewAes(buf []byte)(*Aes,error){

	//创建aes加解密对象
	myAes := new(Aes)

	//生成str_key
	if buf != nil {
		myAes.sKey = buf
	}else {
		myAes.createKey()
	}

	//判断生成的skey字符串是否可靠
	if len(myAes.sKey) != AES_KEY_SIZE {
		return nil,errors.New("create key error ! key size:"+string(len(myAes.sKey)))
	}

	//根据skey生成iv
	myAes.createIV()
	if len(myAes.iv) != GROUP_SIZE {
		return nil,errors.New("create iv error ! iv size:"+string(len(myAes.iv)))
	}

	//创建aes接口
	var err error = nil
	myAes.oKey, err =  aes.NewCipher(myAes.sKey)
	if err != nil {
		return nil,err
	}

	//创建加密接口
	myAes.fEnc = cipher.NewCBCEncrypter(myAes.oKey, myAes.iv)

	//创建一个解密接口
	myAes.fDec = cipher.NewCBCDecrypter(myAes.oKey, myAes.iv)

	//返回加密对象
	return myAes,nil
}

//AES CBC模式加密
func (this* Aes) Enc (buf []byte )([]byte)  {

	if len(buf) <= 0 {
		return nil
	}

	//对加密内容进行填充
	buf = fill(buf)

	//加密
	this.fEnc.CryptBlocks(buf,buf)


	//返回加密后的字符串
	return buf
}

//AES CBC模式解密
func (this* Aes) Dec (buf []byte)([]byte)  {

	//判断非法数据
	//if (len(buf) % 16) != 0 {
	//	return nil
	//}
	//if buf[len(buf)-1] >=16 {
	//	return nil
	//}

	//解密
	this.fDec.CryptBlocks(buf, buf)

	//解密后对填充内容进行删除,并返回
	return fileDele(buf)


}

//返回生成秘钥的字符串
func (this* Aes) GetKey()([]byte){
	return this.sKey
}

//随机生成 skey
func(this * Aes)createKey()(){

	for i:= 0;i< AES_KEY_SIZE;i++{
		this.sKey = append(this.sKey,byte(rand.Intn(255)))
	}

	//fmt.Println("生成的key：",this.sKey)
}

//生成 iv 随机向量
func (this* Aes)createIV()(){
	for i:=0 ;i < GROUP_SIZE / 2;i++{
		this.iv = append(this.iv,this.sKey[len(this.sKey) - i -1])
	}


	for i:=0 ;i < GROUP_SIZE / 2;i++{
		this.iv = append(this.iv,this.sKey[i])
	}


	//fmt.Println("生成iv后 iv的数据为：",this.iv)
	//fmt.Println("生成iv后 key的数据为：",this.sKey)

}

//填充函数,对加密内容进行填充
func fill(buf []byte)([]byte){
	// 求出需要填充的长度
	num := GROUP_SIZE - (len(buf) % GROUP_SIZE)

	//进行填充
	if(num > 0 && num < GROUP_SIZE ){
		for i:=0;i< num ;i++{
			buf = append(buf,byte(num))
		}
	} else if  num == 0{
		for i:=0;i< GROUP_SIZE ;i++{
			buf = append(buf,byte(GROUP_SIZE))
		}
	}else {
		//理论上不可能出现
		return nil
	}

	//返回填充后的数据
	return buf
}

//删除填充内容
func fileDele(buf []byte)([] byte){

	//校验数据是否合法
	if buf==nil || len(buf) < 16{
		return nil
	}

	//需要删除多少字节
	n:= int(buf[len(buf) -1])


	//删除填充内容
	for i:=0;i < n;i++ {
		//利用append删除下标为1的数据

		buf = buf[:len(buf)-1]
	}

	return buf
}


func init()(){
	rand.Seed(time.Now().UnixNano())
}
~~~



- 调用

~~~go
package main

import (
	"code/study/加解密/EAD"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
)

func testAes()(){
	//创建加解密对象1,模拟程序1
	a,err := EAD.NewAes(nil)
	if err != nil{
		fmt.Println("加密对象1创建失败！",err)
	}

	//创建加解密对象2,模拟程序2
	b,err := EAD.NewAes(a.GetKey())
	if err != nil{
		fmt.Println("加密对象2创建失败！",err)
	}

	//加密
	encStr := a.Enc([]byte("asdasd奥术大师大多所"))

	//解密
	fmt.Println(string(b.Dec([]byte(encStr))))

}


func main()(){
	testAes()
}
~~~







### 非对称加密

#### 说明

- 有两把秘钥，公钥和私钥
- 使用公钥加密，私钥解密
- 相当于对称加密来说，加密效率和安全性低



#### RSA

##### 秘钥的长度

 建议选择 1024,2048,4096  长度单位为：bit，位



##### 示例

- 封装好的对象

```go
package EAD

import (
   "crypto/rand"
   "crypto/rsa"
   "crypto/x509"
   "errors"
)

//默认参数
var (
   RSA_KEY_SIZE = 2048    //秘钥长度单位bit  建议选择 1024,2048,4096
)

//RSA加密对象
type Rsa struct{
   priKey *rsa.PrivateKey //生成的密钥对
   pubKey *rsa.PublicKey  //公钥
}

//生成Rsa加密对象
//传入非空值，将会根据传入数据加载公钥，同时放弃秘钥的生成
func NewRsa(buf []byte)(*Rsa, error){

   //创建对象
   myRsa := new(Rsa)

   //通过公钥初始化对象
   if buf != nil {
      //私钥设置为nil
      myRsa.priKey = nil

      //通过公钥切片获取公钥
      pubInterface, err := x509.ParsePKIXPublicKey(buf)
      if err != nil {
         return nil, err
      }
      myRsa.pubKey = pubInterface.(*rsa.PublicKey)   //类型断言

      return myRsa,nil
   }

   // 生成非对称加密的秘钥
   var err error = nil
   myRsa.priKey,err = rsa.GenerateKey(rand.Reader, RSA_KEY_SIZE)
   if err != nil {
      return nil,err
   }

   //设置可以使用的公钥
   temp, err := x509.MarshalPKIXPublicKey(myRsa.priKey.Public())
   if err != nil {
      return nil,err
   }
   //通过公钥切片获取公钥
   pubInterface, err := x509.ParsePKIXPublicKey(temp)
   if err != nil {
      return nil, err
   }
   myRsa.pubKey = pubInterface.(*rsa.PublicKey)   //类型断言

   return myRsa,nil

}

//加密
func(this* Rsa) Enc(buf []byte)([]byte,error){

   //加密
   buf, err := rsa.EncryptPKCS1v15(rand.Reader,this.pubKey, buf)
   if err != nil {
      return nil,err
   }

   return buf,nil
}

//解密
func(this* Rsa) Dec(buf []byte)([]byte,error){

   //通过公钥创建的对象，不具备解密功能
   if this.priKey == nil {
      return nil,errors.New("没有私钥不能加密！")
   }

   //使用私钥解密
   buf, err := rsa.DecryptPKCS1v15(rand.Reader, this.priKey, buf)
   if err != nil {
      return nil,err
   }
   return buf,nil

}

//获取可以传输的公钥
func(this* Rsa) GetKey()([]byte,error){

   //生成可传输的公钥
   buf, err := x509.MarshalPKIXPublicKey(this.priKey.Public())
   if err != nil {
      return nil,err
   }

   return buf,nil
}
```

- 调用案例

```go
package main

import (
   "code/study/加解密/EAD"
   "crypto/sha256"
   "fmt"
)

func testRes()(){

   //模拟私钥端
   testRsa,err := EAD.NewRsa(nil)
   if err != nil {
      fmt.Println("生成非对称加密秘钥失败",err)
      return
   }
   //获取公钥
   pub_key,err :=testRsa.GetKey()
   if err != nil {
      fmt.Println("获取公钥失败",err)
   }

   //模拟公钥端
   testRsa2,err := EAD.NewRsa(pub_key)
   if err != nil {
      fmt.Println("生成非对称加密秘钥失败2",err)
      return
   }

   //公钥加密
   data,err := testRsa2.Enc([]byte("阿发送到发送到发斯蒂芬"))
   if err!=nil {
      fmt.Println("公钥加密失败！",err)
   }

   //私钥解密
   data,err = testRsa.Dec(data)
   if err!=nil {
      fmt.Println("私钥解密失败失败！",err)
   }

   fmt.Println(string(data))
}


func main()(){
   testRes()
}
```





### 单向散列函数(哈希)

#### 说明

- 输入任意二进制数据，输出固定长度的二进制数据
- 输入相同的数据，输出相同的结果
- 输出结果无规律(强抗碰撞性)
- 不可逆(?)



#### 常见算法

- md4/md5	长度128bit
- sha1    长度160bit
- sha224
- sha256
- sha384
- sha512



#### 使用示例

```go
func testHash()(){

   //创建对象
   myHash := sha256.New()
   //添加数据
   myHash.Write([]byte("阿萨大大"))
   myHash.Write([]byte("十大歌手电饭锅"))
   //计算结果
   buf := myHash.Sum(nil);
   //格式化为16进制字符串
   str := hex.EncodeToString(buf)
   fmt.Println(str)


   //方式 2
   buf2 := sha256.Sum256([]byte("asdfasdf"))
   fmt.Println(buf2)

}
```





### 消息验证码

#### 说明

- 通信中避免数据被篡改
- 验证不通过的消息不处理，防止第三方发送的无用消息



#### 实现

~~~ shell
# 1、通信双方都有一份相同的数据(例如秘钥)
# 2、对"发送的数据"加上"相同的数据"进行哈希运算，获取到消息验证码
# 3、对"收到的数据"加上"相同的数据"进行哈希运算，校验消息验证码是否正确
~~~



#### 问题

- 协商相同的数据
- 通信双方都可以生成一样的消息验证码，导致双方可以否认自己发送过数据





### 数字签名

#### 说明

- 通信中避免数据被篡改
- 验证不通过的消息不处理，防止第三方发送的无用消息
- 解决 消息验证码 中，不能第三方证明的问题



#### 实现

~~~ shell
# 私钥持有端，签名
# 1、对需要发送的数据进行哈希运算
# 2、对散列值进行私钥加密
# 3、将数据和散列值发送给对方

# 公钥持有端，验证
# 1、对收到的数据进行拆分，分出 数据 和 散列值
# 2、对散列值进行公钥解密
# 3、对数据进行哈希运算
# 4、比较两个散列值是否相同

# 防止否认
# 私钥持有者生成的签名，除了他自己别人无法伪造。解决的消息验证码中，消息否认的问题。
~~~





#### 问题

- 秘钥协商时客服端无法判断公钥的来源是否是服务器，如果公钥被拦截篡改，拦截者可以伪造一份密钥对，在中间进行通信





### 证书

#### 说明

- 引第三方机构，对秘钥分发者的 公钥和组织信息 进行私钥加密，确认公钥的持有者。





### 网络文章

#### 数字证书

https://www.lizenghai.com/archives/85404.html



#### 消息验证吗和数字签名

https://www.lizenghai.com/archives/82415.html
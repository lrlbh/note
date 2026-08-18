### 更新系统

~~~shell
sudo apt update
apt list --upgradable
sudo apt upgrade
sudo apt dist-upgrade
sudo apt autoremove


~~~



### vim配置

~~~shell
# 设置root用户密码
sudo passwd root

# 常用配置添加
# 为当前用户添加
echo "
set mouse=v 
set tabstop=4 
set shiftwidth=4 	
set nu	
syntax on 
" >> .vimrc

# 为root用户添加
su - 

echo "
set mouse=v 
set tabstop=4 
set shiftwidth=4 	
set nu	
syntax on 
" >> .vimrc

~~~





### ssh登录

~~~shell
# 打开配置文件
sudo vim /etc/ssh/sshd_config

# 取消注释
PasswordAuthentication yes
~~~





### python虚拟环境

~~~shell
# 安装虚拟环境
sudo apt-get install python3-venv

# 创建虚拟环境
python3 -m venv 虚拟环境名称
~~~


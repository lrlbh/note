### docker安装

ubuntu22.04安装

~~~shell
# 卸载已安装的
for pkg in docker.io docker-doc docker-compose docker-compose-v2 podman-docker containerd runc; do sudo apt-get remove $pkg; done


# 设置存储库
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl gnupg
sudo install -m 0755 -d /etc/apt/keyrings
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
sudo chmod a+r /etc/apt/keyrings/docker.gpg

# Add the repository to Apt sources:
echo \
  "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update


# 安装最新版
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# 查看是否安装成功
sudo docker run hello-world
~~~





### 镜像操作

~~~shell
# 查看镜像
sudo docker images 

# 删除镜像
sudo docker rmi   
sudo docker rmi 镜像名 -f # 强制删除
sudo docker image prune # 删除没有使用的镜像

# 拉取镜像
sudo docker pull

# 推送镜像
sudo docker push

# 编译自己的镜像
sudo docker build

# 镜像打包到本地
sudo docker save	# docker save 镜像名称 -o 输出文件名称

# 加载本地镜像
docker load	# sudo docker load -i 镜像名称
~~~



### 容器操作

~~~shell
# 镜像不存在自动下载 + 创建容器 + 运行
sudo docker run

# 运行容器
sudo docker start

# 停止容器
sudo docker stop

# 删除容器
sudo docker rm

# 查看运行中的容器
sudo docker ps	# 查看运行所有 sudo docker ps -a

# 查看某个容器的日志
sudo docker logs 

# 进入某个容器
sudo docker exec # sudo docker exec -it test_nginx /bin/sh
~~~



### 数据卷

~~~shell
# 在创建容器时挂载
sudo docker run -v 数据卷名称:容器内位置
sudo docker run -v 自定义主机目录:容器内位置
# sudo docker run -d --name test_nginx -p 80:80 -v test:/usr/share/nginx/html nginx

# 查看挂载到本地的目录
sudo docker volume ls

# 查看某个目录具体信息
sudo docker volume inspect 

# 删除某个目录
sudo docker volume rm
~~~





docker -v

sudo docker images

systemctl enable docker # 开机自动启动docker
systemctl start docker # 启动docker
systemctl restart docker # 重启dokcer
systemctl stop docker # 关闭docker 

systemctl status docker # 状态 docker 

docker system prune -a





~~~shell
# mysql 示例
sudo docker run -d  --name mysql -p 3306:3306 -e TZ=Asia/Shanghai -e MYSQL_ROOT_PASSWORD=123 mysql

run 								# 镜像不存在自动下载 + 创建容器 + 运行
--name mysql 						# 自定义容器名称
-d									# 后台运行
-p 3306:3306 						# 端口转发 --》 主机端口:docker内部使用端口
-e TZ=Asia/Shanghai 				# 设置mysql时区
-e MYSQL_ROOT_PASSWORD=123 			# 设置mysql密码
mysql 								# 镜像名称 完整语法需要镜像名称加版本号 --> mysql:5.7
~~~


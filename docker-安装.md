# docker 安装

## 卸载docker

```
删除已安装的Docker
# Uninstall installed docker
sudo yum remove docker \
                  docker-client \
                  docker-client-latest \
                  docker-common \
                  docker-latest \
                  docker-latest-logrotate \
                  docker-logrotate \
                  docker-selinux \
                  docker-engine-selinux \
                  docker-engine
     

```

## 配置阿里云Docker Yum源

```
# Set up repository
sudo yum install -y yum-utils device-mapper-persistent-data lvm2

# Use Aliyun Docker
sudo yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo

```

## 安装指定版本

查看Docker版本：

```
yum list docker-ce --showduplicates
```

安装较旧版本（比如Docker 17.03.2) 时需要指定完整的rpm包的包名，并且加上`--setopt=obsoletes=0` 参数：

```
# Install docker
# on a new system with yum repo defined, forcing older version and ignoring obsoletes introduced by 17.06.0
yum install -y --setopt=obsoletes=0 \
   docker-ce-17.03.2.ce-1.el7.centos.x86_64 \
   docker-ce-selinux-17.03.2.ce-1.el7.centos.noarch

```

或安装Docker较新版本（比如Docker 18.03.0)时加上rpm包名的版本号部分：

```
sudo yum install docker-ce-18.03.0.ce
```

或安装Docker最新版本，无需加版本号：

```
sudo yum install docker-ce
```

## 启动Docker服务

```
# Start docker service
systemctl enable docker
systemctl start docker


```


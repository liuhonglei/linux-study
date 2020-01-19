# kubernetes源配置

## 配置k8s仓库 yum源

```
cat > /etc/yum.repos.d/kubernetes.repo << EOF
[kubernetes]
name=Kubernetes
baseurl=https://mirrors.aliyun.com/kubernetes/yum/repos/kubernetes-el7-x86_64
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://mirrors.aliyun.com/kubernetes/yum/doc/yum-key.gpg https://mirrors.aliyun.com/kubernetes/yum/doc/rpm-package-key.gpg
EOF
```

## 内核配置

```
# 开启路由转发功能以及iptables的过滤策略。

cat <<EOF >  /etc/sysctl.d/k8s.conf
#开启iptables中ipv6的过滤规则
net.bridge.bridge-nf-call-ip6tables = 1
#开启ipv4的过滤规则
net.bridge.bridge-nf-call-iptables = 1
#开启服务器的路由转发功能
net.ipv4.ip_forward = 1
EOF

```

## 

## 安装相关组件

```
#查看所有的kublet版本，选择适合自己的版本。
yum list kubelet --showduplicates | sort -r
#安装相关版本的组件
yum -y install kubeadm-1.13.0-0 kubelet-1.13.0-0 kubectl-1.13.0-0  kubernetes-cni-0.6.0-0 
#所有服务都设置为开机自启动
systemctl enable kubelet

```

## kubernetes启动master节点

```
kubeadm init  --kubernetes-version=v1.13.3  --pod-network-cidr=10.244.0.0/16 --apiserver-advertise-address=192.168.0.65 --image-repository registry.aliyuncs.com/google_containers --service-cidr=10.1.0.0/16 
#--kubernetes-version指明要下载的镜像版本
#--pod-network-cidr 指明网络的子网掩码，此处使用flannel模型的子网掩码
#--apiserver-advertise-address 指明与master结点绑定的ip


```



**按照操作步骤**

```
mkdir -p $HOME/.kube
  sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
  sudo chown $(id -u):$(id -g) $HOME/.kube/config
```


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

## 安装pod网络插件

```
kubectl apply -fhttps://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml
```

## 加入集群节点

在节点机器上执行如下命令，加入集群

```
kubeadm join 192.168.0.65:6443 --token c1nse1.o6g9zyxlnn8445od --discovery-token-ca-cert-hash sha256:92b39175b8707c6a86c97db9372f4e125af4e69f9a169a9630f5953381aa9850
```

## 集群测试

```
//创建nginx
kubectl delete service nginx
//暴露端口
kubectl expose deployment nginx --port=80 --type=NodePort
//查看pods 和services
kubectl get pods, svc
```

## 安装dashboard

```
kubectl apply -f https://raw.githubusercontent.com/kubernetes/dashboard/v1.10.1/src/deploy/recommended/kubernetes-dashboard.yaml

kubectl apply -f https://raw.githubusercontent.com/kubernetes/dashboard/master/src/deploy/recommended/kubernetes-dashboard.yaml

```

## 部署这个dashboard太他妈蛋疼了

- 首先，官方给出的dashboard.yaml无法下载。在网上查了下，好像需要梯子。没有办法，找其他资源。

  找到了csdn上一个文章，地址如下：https://blog.csdn.net/qq_40460909/article/details/85123605。下载完作者的kubernetes-dashboard.yaml, 将镜像地址 `k8s.gcr.io``/kubernetes-dashboard-amd64``:v1.8.3`改为了李振良老师的镜像地址 lizhenliang/kubernetes-dashboard-amd64:v1.10.1。

  后按照视频教程去处理

  ```
  kubectl apply -f kubernetes-dashboard.yaml
  ```

  但是教程结果是  需要进行配置kubeconfig和 令牌的页面不同，而我的页面是类似这种界面![](G:\linux\linux学习\20181220153258533.jpg)

无法按照教程去处理了，无奈之下只有删除已经创建的dashboard。

```
kubectl get secret,sa,role,rolebinding,services,deployments --namespace=kube-system | grep dashboard
kubectl delete deployment kubernetes-dashboard --namespace=kube-system 
kubectl delete service kubernetes-dashboard  --namespace=kube-system 
kubectl delete role kubernetes-dashboard-minimal --namespace=kube-system 
kubectl delete rolebinding kubernetes-dashboard-minimal --namespace=kube-system
kubectl delete sa kubernetes-dashboard --namespace=kube-system 
kubectl delete secret kubernetes-dashboard-certs --namespace=kube-system
kubectl delete secret kubernetes-dashboard-key-holder --namespace=kube-system
```

反正最后是把dashboard相关的pod ，serivce， deployment 等全部删除了。期间还执行过secret生成之类的。

https://blog.csdn.net/qq_38900565/article/details/100729686



创建管理员角色

```
创建kubernetes-dashboard管理员角色
[root@k8s-master ~]# vi k8s-admin.yaml
apiVersion: v1
kind: ServiceAccount
metadata:
  name: dashboard-admin
  namespace: kube-system
---
kind: ClusterRoleBinding
apiVersion: rbac.authorization.k8s.io/v1beta1
metadata:
  name: dashboard-admin
subjects:
  - kind: ServiceAccount
    name: dashboard-admin
    namespace: kube-system
roleRef:
  kind: ClusterRole
  name: cluster-admin
  apiGroup: rbac.authorization.k8s.io

[root@k8s-master ~]# kubectl create -f k8s-admin.yaml
```

获取dashboard管理员角色token

```
获取dashboard管理员角色token
[root@k8s-master ~]# kubectl describe secret dashboard-admin-token-7z6zm -n kube-system     
Name:         dashboard-admin-token-7z6zm
Namespace:    kube-system
Labels:       <none>
Annotations:  kubernetes.io/service-account.name: dashboard-admin
              kubernetes.io/service-account.uid: f0d1d33f-d43d-11e9-a75a-fa163e7d0486

Type:  kubernetes.io/service-account-token

Data
====
token:      eyJhbGciOiJSUzI1NiIsImtpZCI6IiJ9.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJrdWJlLXN5c3RlbSIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJkYXNoYm9hcmQtYWRtaW4tdG9rZW4tN3o2em0iLCJrdWJlcm5ldGVzLmlvL3NlcnZpY2VhY2NvdW50L3NlcnZpY2UtYWNjb3VudC5uYW1lIjoiZGFzaGJvYXJkLWFkbWluIiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9zZXJ2aWNlLWFjY291bnQudWlkIjoiZjBkMWQzM2YtZDQzZC0xMWU5LWE3NWEtZmExNjNlN2QwNDg2Iiwic3ViIjoic3lzdGVtOnNlcnZpY2VhY2NvdW50Omt1YmUtc3lzdGVtOmRhc2hib2FyZC1hZG1pbiJ9.Jmws1PEvnjG4fmR2YoZTV909dvPwJdRTi_KSkUnezA1i1GBd7YHpIjw_MmVj8Vx-C4dE83OPPqS2UIdslJQV-KsAYQNOMaPxhxOz4WRgIzEcxpOXMEKny93AGB6PcpQrmtNnmnwGEX8wF-dqogqoyH-8X-iDdpQ75-TbrVmco-NZtb7GMGKiTnBK_cRZ2iGg-Oq4ic7YoJpM0C1a87xNb4kOfUCIShj1JqWJTdoMtvjiCSTvjBVz8mICvQ9qMrJfxCZZJ6BjNNvMDqrd2cWKu14mjDo_hipt6DBcKSZDmp-jBCccx4RG_9CGpp6UyeFWVuEvDxeN8ABkX6RB74s3hw
ca.crt:     1025 bytes
namespace:  11 bytes

```



基本删除干净后，有重新在cnblogs找了dashboard.yaml文件，地址如下 https://www.cnblogs.com/aguncn/p/10904822.html。将该dashboard.yaml下载下来后，只是将镜像名改为李振良老师的镜像地址。然后执行 

```
kubectl apply -f kubernetes-dashboard.yaml
```

然后他妈的居然好了，连教程里的kubeconfig和令牌都不用配了。
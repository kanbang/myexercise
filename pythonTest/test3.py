#/usr/bin/
#-*- coding:utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

#初始化数据
x=0.5/5
A=10*10**-3
TA=100
TB=500
k=1000

#定义系数矩阵
xishu=np.matrix([[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0]])

#定义所求的物理量
result=np.matrix(['T1','T2','T3','T4','T5'])

#所求的数据放在这个数组中
s=np.matrix([0,0,0,0,0]).reshape(-1,1)

def formatStringList(data):
    msg= []
    for a in data:
        msg.append(str(a).ljust(8))
    return ' '.join(msg)
def printTitle(title):
    print formatStringList(title)
def printData(datas):
    for data in datas:
        print formatStringList(data)
#主函数
def test(x,A,TA,TB,k):
    title=['节点    ','aW','aE','Su','SP','aP']
    datas=[]
    for i in range(1,6):
        if i==1:
           datas.append(f1(x,A,TA,TB,k))
        if i> 1 and i< 5:
            datas.append(f(x,A,TA,TB,k,i))
        if i==5:
            datas.append(f5(x,A,TA,TB,k))
    xi=xishu**-1
    r=np.dot(xi,s)

    #如果不需要的话,这些打印语句可以注释掉
    #通常print一般作为调试的时候用
    printTitle(title)
    printData(datas)
    print '\n计算结果为：'
    printTitle(result)
    printData(r.reshape(1,-1))

    return r;

#第一个节点的处理
def f1(x,A,TA,TB,k):
    aW=0;aE=k*A/x
    SP= -2*k*A/x;aP=aW+aE-SP
    Su=2*k*A*TA/x
    xishu[0,0]=aP
    xishu[0,1]= -aE
    s[0]=200*TA
    return [1,aW,aE,Su,SP,aP]

#第五个节点的处理
def f5(x,A,TA,TB,k):
    aW=k*A/x;aE=0
    SP=-2*k*A/x;aP=aW+aE-SP
    Su=2*k*A*TB/x
    xishu[4,4]=aP
    xishu[4,3]= -aW
    s[4]=200*TB
    return [2,aW,aE,Su,SP,aP]

#从2到4节点的处理
def f(x,A,TA,TB,k,i):
    aW=k*A/x;aE=k*A/x;aP=aW+aE
    n=i
    xishu[n-1,n-2]= -aW
    xishu[n-1,n-1]=aP
    xishu[n-1,n]= -aE
    return[n,aW,aE,0,0,aP]

def fig(z):
    x=np.arange(0,z,0.05)
    y1=(800*x+100)
    plt.figure(figsize=(8,4))
    plt.plot(x,y,color='red',linewidth=2)
    #plt.xlable("juli(x/m)")
    #plt.ylable("t")
    plt.title("figure1")
    plt.ylim(0,600)
    X0=[0,0.05,0.15,0.25,0.35,0.45,0.5]
    Y0=[100,140,220,300,380,460,500]
    plot1=plt.plot(X0,Y0,'b*',3)
    plt.legend()
    plt.show()

def fig2(xmin, xmax, step=0.05):
    X=np.arange(xmin,xmax,step)

    Y = []
    #计算X区间中的每一个x值
    for x in X:
        r = test(x, A, TA, TB, k)
        #利用r构造数据，比如Y
        #Y.append(xxxxx)

    plt.figure(figsize=(8,4))
    plt.plot(X,Y,color='red',linewidth=2)
    plt.show()

test(x,A,TA,TB,k)
fig(0.55)
fig2(0,0.55,0.05)


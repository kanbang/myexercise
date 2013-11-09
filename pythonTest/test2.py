#/usr/bin/
#-*- coding:utf-8 -*- 

import numpy as np

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
#主函数

#格式化字符串链表
#将链表中的每个字符串重新格式化为宽度为8(不足部分的用空白替代),左对齐(ljust)
#最后将格式化后的链表分割成字符串
#
#关键词: "python 输出对齐"
#ljust函数示例:
#   "12.3".ljust(4)  ==>  "12.3 "
#   "2.3".ljust(8)   ==>  "12.3    "
#
#关键词: "python list string 转换"
# a=[1,2,3]
# ' '.join(a)  ==> "1 2 3"
# '\t'.join(a)  ==> "1  2   3"
# '**'.join(a)  ==> "1**2**3"
def formatStringList(data):
   msg = []
   for a in data:
       msg.append(str(a).ljust(8))
   return ' '.join(msg)

def printTitle(title):
    print formatStringList(title)

def printData(datas):
    for data in datas:
        print formatStringList(data)

def test(x,A,TA,TB,k):
    #记录标题
    title = ['节点','aW','aE','Su','SP','aP']
    #记录矩阵数据
    #可以用list嵌套list表示矩阵或者使用numpy提供的matrix也行
    datas = []
    #
    #计算并返回相应的数据
    for i in range(1,6):
        if i==1:
            #注:不要在函数中直接打印,这种习惯不好!!!!
            #应该将数据存放在某个数据结构中,最后统一打印!!!
            datas.append(f1(x,A,TA,TB,k))
        if i>1 and i<5:
            datas.append(f(x,A,TA,TB,k,i))
        if i==5:
            datas.append(f5(x,A,TA,TB,k))
    xi=xishu**-1
    r=np.dot(xi,s)

    printTitle(title)
    printData(datas)
    print '\n计算结果:'
    printTitle(result)
    printData(r.reshape(1,-1))

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
    return [5,aW,aE,Su,SP,aP]

#从2到4节点的处理
def f(x,A,TA,TB,k,i):
    aW=k*A/x;aE=k*A/x;aP=aW+aE
    Su=0;SP=0
    n=i
    xishu[n-1,n-2]= -aW
    xishu[n-1,n-1]=aP
    xishu[n-1,n]= -aE
    return [n,aW,aE,Su,SP,aP]

test(x,A,TA,TB,k)


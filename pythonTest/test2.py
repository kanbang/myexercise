#/usr/bin/
#-*- coding:utf-8 -*- 

import numpy as np

#��ʼ������
x=0.5/5
A=10*10**-3
TA=100
TB=500
k=1000
#����ϵ������
xishu=np.matrix([[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0],[0,0,0,0,0]])
#���������������
result=np.matrix(['T1','T2','T3','T4','T5'])
#��������ݷ������������
s=np.matrix([0,0,0,0,0]).reshape(-1,1)
#������

#��ʽ���ַ�������
#�������е�ÿ���ַ������¸�ʽ��Ϊ���Ϊ8(���㲿�ֵ��ÿհ����),�����(ljust)
#��󽫸�ʽ���������ָ���ַ���
#
#�ؼ���: "python �������"
#ljust����ʾ��:
#   "12.3".ljust(4)  ==>  "12.3 "
#   "2.3".ljust(8)   ==>  "12.3    "
#
#�ؼ���: "python list string ת��"
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
    #��¼����
    title = ['�ڵ�','aW','aE','Su','SP','aP']
    #��¼��������
    #������listǶ��list��ʾ�������ʹ��numpy�ṩ��matrixҲ��
    datas = []
    #
    #���㲢������Ӧ������
    for i in range(1,6):
        if i==1:
            #ע:��Ҫ�ں�����ֱ�Ӵ�ӡ,����ϰ�߲���!!!!
            #Ӧ�ý����ݴ����ĳ�����ݽṹ��,���ͳһ��ӡ!!!
            datas.append(f1(x,A,TA,TB,k))
        if i>1 and i<5:
            datas.append(f(x,A,TA,TB,k,i))
        if i==5:
            datas.append(f5(x,A,TA,TB,k))
    xi=xishu**-1
    r=np.dot(xi,s)

    printTitle(title)
    printData(datas)
    print '\n������:'
    printTitle(result)
    printData(r.reshape(1,-1))

#��һ���ڵ�Ĵ���
def f1(x,A,TA,TB,k):
    aW=0;aE=k*A/x
    SP= -2*k*A/x;aP=aW+aE-SP
    Su=2*k*A*TA/x
    xishu[0,0]=aP
    xishu[0,1]= -aE
    s[0]=200*TA
    return [1,aW,aE,Su,SP,aP]

#������ڵ�Ĵ���
def f5(x,A,TA,TB,k):
    aW=k*A/x;aE=0
    SP=-2*k*A/x;aP=aW+aE-SP
    Su=2*k*A*TB/x
    xishu[4,4]=aP
    xishu[4,3]= -aW
    s[4]=200*TB
    return [5,aW,aE,Su,SP,aP]

#��2��4�ڵ�Ĵ���
def f(x,A,TA,TB,k,i):
    aW=k*A/x;aE=k*A/x;aP=aW+aE
    Su=0;SP=0
    n=i
    xishu[n-1,n-2]= -aW
    xishu[n-1,n-1]=aP
    xishu[n-1,n]= -aE
    return [n,aW,aE,Su,SP,aP]

test(x,A,TA,TB,k)


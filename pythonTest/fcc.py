#-*- code:utf-8 -*-

import numpy as np

A=np.array([[100,0,0,64,0],[0,100,0,0,144],[100,-100,4,0,0]])
b=np.array([450,450,0])

A0=np.eye(5)-np.dot(np.linalg.pinv(A),A)
A1=np.eye(5)-np.dot(np.linalg.pinv(A0),A0)
A2=np.eye(5)-np.dot(np.linalg.pinv(A1),A1)
A3=np.eye(5)-np.dot(np.linalg.pinv(A2),A2)

print 'A0 =',A0
print 'A1 =',A1
print 'A2 =',A2
print 'A3 =',A3

H0=np.linalg.pinv(A0)
H1=np.linalg.pinv(A1)
H2=np.linalg.pinv(A2)
H3=np.linalg.pinv(A3)

print np.dot(H1,H0)
print np.dot(H2,np.dot(H1,H0))
print np.dot(H3,np.dot(H2,np.dot(H1,H0)))

r=np.linalg.matrix_rank(A)
u,s,v=np.linalg.svd(A)
AH=np.linalg.pinv(A)

#A    -- m*n
#AH   -- n*m
#AH*A -- n*n
n=A.shape[1]
def get_x(k):
    return np.dot(AH,b)-np.dot(np.eye(n)-np.dot(AH,A),k)

def rand_float(a,b,n=1):
    return a+(b-a)*np.random.rand(n)

#k=(0,1)
x1=get_x(rand_float(0,1,n))
x2=get_x(rand_float(3,9,n))
x3=get_x(rand_float(2,6,n))

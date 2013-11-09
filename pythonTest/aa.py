# coding=utf-8

def f(r,q):
    return r*q

def fp(r,q):
    return r*pow(q,3)/3

def h(q):
    return 1046.5+5*q-0.85*pow(q,2)

def H(q):
    return 1046*q+0.5*5*pow(q,2)-0.85/3*pow(q,3)

r1=0.375
r2=0.15
r3=2.0
r4=4.6875
r5=12.5
r6=1.7361
r7=0.5

q1=20
q2=10
q3=10
q4=8
q5=2
q6=12
q7=20

# P1
P1=fp(r1,q1)+fp(r2,q2)+fp(r3,q3)+fp(r4,q4)+fp(r5,q5)+fp(r6,q6)+fp(r7,q7)
P2=H(20)
P3=h(20)*20

print P1
print P2
print P3

from ASCsoft.bla import *
m =3
n =4
x=Matrix(m,n)
y=Matrix(n,m)
v=Vector(n)
print("shape of x =", x.shape)
print("height of x =", x.height)
print("width of x =", x.width)
for i in range(m):
    for j in range(n):
        x[i,j]=i+j
        y[j,i]=i
        v[j]=j
print("x =",x)
print("y =",y)
print("x-x =", x-x)
print("x*y =", x*y)
print("v =", v)
print("x*v =", x*v)
print("2*x =", 2*x)

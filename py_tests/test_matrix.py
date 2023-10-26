from ASCsoft.bla import Matrix
m =3
n =3
x=Matrix(m,n)
print(x.shape)
print(x.height)
print(x.width)
for i in range(m):
    for j in range(n):
        x[i,j]=i+j
print(x)
print(x+x)

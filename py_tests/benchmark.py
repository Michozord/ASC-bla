import timeit
#primitve benchmark
#to do: implement buffer protocol, think how you could effectively measure the time for C++ code separately
#runtime around 20 sec
#For later comparison: before adding HPC improvements we had runtime of roughly 1sec for n=1000
setup10='''
import random
from ASCsoft.bla import Matrix
def fill(k):
    m=Matrix(k,k)
    for i in range(k):
        for j in range(k):
            m[i,j]=10*random.random()
    return m

x1=fill(10)
x2=fill(10)'''
setup100='''
import random
from ASCsoft.bla import Matrix
def fill(k):
    m=Matrix(k,k)
    for i in range(k):
        for j in range(k):
            m[i,j]=10*random.random()
    return m

y1=fill(100)
y2=fill(100)'''
setup1000='''
import random
from ASCsoft.bla import Matrix
def fill(k):
    m=Matrix(k,k)
    for i in range(k):
        for j in range(k):
            m[i,j]=10*random.random()
    return m

z1=fill(1000)
z2=fill(1000)'''

code10='''
x=x1*x2
'''
code100='''
y=y1*y2
'''
code1000='''
z=z1*z2
'''
time10=0
for k in range(5):
    time10+=timeit.timeit(setup=setup10,stmt=code10,number=3)/3
time100=0
for k in range(5):
    time100+=timeit.timeit(setup=setup100,stmt=code100,number=3)/3
time1000=0
for k in range(5):
    time1000+=timeit.timeit(setup=setup1000,stmt=code1000,number=3)/3

print("the time for n=10 is:", time10/5)
print("the time for n=100 is:", time100/5)
print("the time for n=1000 is:", time1000/5)

npsetup1000='''
import numpy as np
z1=np.random.rand(1000,1000)*10
z2=np.random.rand(1000,1000)*10
'''

npcode1000='''
np.matmul(z1,z2)
'''

nptime1000=0
for k in range(5):
    nptime1000+=timeit.timeit(setup=npsetup1000,stmt=npcode1000,number=3)/3

print("numpy time for n=1000 is:", nptime1000/5)
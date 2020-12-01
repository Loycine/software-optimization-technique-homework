
results = []

with open('plot_block.txt', 'r') as f:
    is_head_line = True
    line = f.readline()
    while line:
        if(is_head_line):
            is_head_line=False
            line = f.readline()
            continue
        results.append(line.split())
        line = f.readline()

results_in_order = sorted(results, key=lambda x : int(x[1]))
print(results_in_order)

x = []
y = []
z = []

N = 2048
total = N*N*N + N*N*(N-1)

for r in results_in_order:
    m = int(r[1])
    t = float(r[3])

    x.append(m)
    y.append(t)
    z.append(70.2498)
    print(f'| {m} | {total/t:.2f} |')




import numpy as np
import matplotlib.pyplot as plt

l1=plt.plot(x,y,'r--',label='N=2048')

plt.plot(x,y,'ro-')
plt.title('The Time and m0')
plt.xlabel('m0')
plt.ylabel('Time')
plt.legend()
plt.savefig('result.png')
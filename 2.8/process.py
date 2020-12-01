
results = []

with open('result.txt', 'r') as f:
    is_head_line = True
    line = f.readline()
    while line:
        if(is_head_line):
            is_head_line=False
            line = f.readline()
            continue
        results.append(line.split())
        line = f.readline()


print(results)

for r in results:
    N = int(r[0])
    
    m0 = int(r[1])
    t1 = float(r[3])

    print(f'| {N} | {t1:.2f} |')
    
for r in results:
    N = int(r[0])
    m0 = int(r[1])
    t1 = float(r[2])
    
    total = N*N*N +N*N*(N-1)
    print(f'| {N} | {total/t1:.2f} |')


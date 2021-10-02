f = open('parabola.txt', 'w')

for i in range(0, 600):
    f.write(f'{i} {i} {i * i}\n')


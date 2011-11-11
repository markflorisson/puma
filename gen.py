L = [50, 100, 200, 400, 800, 1000, 1200, 1400, 1600, 1800, 2000]

for x in L:
    f = open('perf%04dx%04d.dat' % (x, x), 'w')
    f.write('%d %d\n' % (x, x))
    for i in range(x):
        f.write(' '.join('1' for _ in xrange(x)))
        f.write('\n')
    f.close()

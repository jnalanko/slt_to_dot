import numpy as np
import matplotlib.pyplot as plt
import sys
import math

wanted_depth = int(sys.argv[2])
bucket_width = 5
buckets = [0] * 100000
for line in open(sys.argv[1]):
    depth, size = map(int, line.split())
    if depth == wanted_depth:
        buckets[size/bucket_width] += 1

while buckets[-1] == 0: 
    buckets.pop()

# HACK TO MAKE LOGARITHM NOT CRASH
for i in xrange(0,len(buckets)): 
    buckets[i] += 1
# END HACK

print(buckets)

# HACK: xrange(1,len(buckets+1)), starting from 1 instead of 0 to make logarithm not crash
plt.scatter(map(math.log, [bucket_width * x for x in xrange(1,len(buckets)+1)]), map(math.log, buckets))

#plt.scatter([bucket_width * x for x in xrange(0,len(buckets))], buckets)
plt.title("Depth " + str(wanted_depth))
plt.xlabel("Subtree size (log)")
plt.ylabel("Frequency (log)")
plt.show()

import math

out = "../auto-generated/tan_lut.c"

n = 1000
correction = 0.0001
start = 0.0001
interval = math.pi / n - correction
end = math.pi / 2

a = [start]
acc = start
while interval < end:
    a.append()
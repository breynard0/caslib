import math
import os

out = "auto-generated/tan_lut.h"

n = 1000
correction = 0.0001
start = 0.0001
interval = math.pi / (2*n) - correction
end = math.pi / 2

a = []
b = []
acc = start
while acc < end:
    a.append(acc)
    b.append(math.tan(acc))
    acc += interval

text = f"const int LENGTH = {len(a)};\n"
text += f"const float TAN_LUT_A[{len(a)}] = "
text += '{\n'
for val in a:
    text += f"  {val},\n"
text = text[:-2]
text += "\n};\n"
text += f"const float TAN_LUT_B[{len(b)}] = "
text += "{\n"
for val in b:
    text += f"  {val},\n"
text = text[:-2]
text += "\n};"

with open(out, "w") as f:
  f.write(text)
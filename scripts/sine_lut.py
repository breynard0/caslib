exit()

import math

out = "auto-generated/sin_lut.h"

n = 20000
start = 0
interval = math.pi / n
end = math.pi / 2

a = []
b = []
acc = start
while acc <= end:
    a.append(acc)
    b.append(math.sin(acc))
    acc += interval

text = f"const int LENGTH = {len(a)};\n"
text += f"const float SIN_LUT_A[{len(a)}] = "
text += '{\n'
for val in a:
    text += f"  {val},\n"
text = text[:-2]
text += "\n};\n"
text += f"const float SIN_LUT_B[{len(b)}] = "
text += "{\n"
for val in b:
    text += f"  {val},\n"
text = text[:-2]
text += "\n};"

with open(out, "w") as f:
  f.write(text)

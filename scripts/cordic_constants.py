import math
from math import atan

out = "auto-generated/cordic_constants.h"

n = 40;

angles = []
cos_mul = 1.0
i = 0;
while i < n:
    val = atan(1.0/(pow(2.0, i)))
    angles.append(val)
    cos_mul *= math.cos(val)
    i += 1

text = f"const unsigned int CORDIC_COUNT = {n};\n"
text += "const double CORDIC_CONSTANTS["
text += f"{n}] = "
text += "{"
for angle in angles:
    text += f"{angle}, "
text = text[:-2]
text += "};\nconst double COS_MULTIPLIER = "
text += f"{cos_mul};"

with open(out, "w") as f:
  f.write(text)

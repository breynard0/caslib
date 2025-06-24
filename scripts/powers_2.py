out = "auto-generated/powers_two"

n = 64

text = f"double TWO_POWERS[{n}] = "
text += "{"

i = 0;
while i < n:
    text += f"{pow(2, i)}.0, "
    
    i += 1

text = text[:-2]
text += "};"

with open(out + '.c', "w") as f:
  f.write(text)
with open(out + '.h', "w") as f:
    f.write(f"extern double TWO_POWERS[{n}];")

import sys, re

if len(sys.argv) != 4:
    print("Usage: ./{sys.argv[0]} outdir code_num filepath")
    exit(1)

outdir = sys.argv[1]
codenum = int(sys.argv[2])
filepath = sys.argv[3]
outpath = f"{outdir}/code{codenum}.cc"

with open(filepath) as f:
    lines = f.readlines()

# Add namespace after includes
index = 0
for i, line in enumerate(reversed(lines)):
    if re.match(r"^\s*#include", line):
        index = len(lines) - i
        break

lines.insert(index, f"\nnamespace code{codenum} {{\n\n")

# Remove main function
for i, line in enumerate(lines):
    if re.match(r"^\s+(int|void)\s+main\(", line):
        lines = lines[:i]
        break

# Close namespace
lines.append("\n\n}\n")

with open(outpath, "w") as f:
    f.writelines(lines)
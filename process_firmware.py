import sys, os, re

if len(sys.argv) != 4:
    print("Usage: ./{sys.argv[0]} outdir code_num filepath")
    exit(1)

outdir = sys.argv[1]
namespace = sys.argv[2]
filepath = sys.argv[3]
outpath = f"{outdir}/{namespace}.cc"

if not re.match(r"^code\d+$", namespace):
    print(f"Invalid namespace name. Format must be code{{n}}: {namespace}")
    exit(1)

if not os.path.isdir(outdir):
    print(f"Outdir doesn't exist: {outdir}")
    exit(1)

if not os.path.exists(filepath):
    print(f"Filepath doesn't exist: {filepath}")
    exit(1)

with open(filepath) as f:
    lines = f.readlines()

# Add namespace after includes
index = 0
for i, line in enumerate(list(lines)):
    if re.match(r"^\s*#include", line):
        lines[i] = re.sub(r'^\s*#include\s+"', '#include "includes/', line)
        index = i + 1

lines.insert(index, f"\nnamespace {namespace} {{\n\n")

# Remove main function
for i, line in enumerate(lines):
    if re.match(r"^\s+(int|void)\s+main\(", line):
        lines = lines[:i]
        break

# Close namespace
lines.append("\n\n}\n")

with open(outpath, "w") as f:
    f.writelines(lines)

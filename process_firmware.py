import sys, os, re

if len(sys.argv) != 4:
    print("Usage: ./%s [outdir] [mcu_id] [filepath.cc]" % sys.argv[0])
    exit(1)


outdir = sys.argv[1]
namespace = sys.argv[2]
filepath = sys.argv[3]
outpath = "%s/%s.cc.new" % (outdir, namespace)

if not re.match(r"^code\d+$", namespace):
    print("Invalid namespace name. Format must be code{{n}}: %s" % namespace)
    exit(1)

if not os.path.isdir(outdir):
    print("Outdir doesn't exist: %s" % outdir)
    exit(1)

if not os.path.exists(filepath):
    print("Filepath doesn't exist: %s" % filepath)
    exit(1)

with open(filepath) as f:
    lines = f.readlines()

# Add namespace after includes
index = 0
for i, line in enumerate(list(lines)):
    if re.match(r"^\s*#include", line):
        lines[i] = re.sub(r'^\s*#include\s+"', '#include "includes/', line)
        index = i + 1

lines.insert(index, "\nnamespace %s {\n\n" % namespace)

# Remove main function
for i, line in enumerate(lines):
    if re.match(r"^\s+(int|void)\s+main\(", line):
        lines = lines[:i]
        break

# Close namespace
lines.append("\n\n}\n")

with open(outpath, "w") as f:
    f.writelines(lines)

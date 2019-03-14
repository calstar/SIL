import sys
import shutil
import os

if len(sys.argv) > 6:
    print("Too many files")
    exit()

shutil.rmtree('build/obj/firmware')

i = 0
for file in sys.argv[1:]:
    name = "firmware/code" + string(i) + ".cc"
    shutil.move(name, name + ".bak")
    shutil.copy(file, name)
    i += 1

os.system("make")

i = 0
for file in sys.argv[1:]:
    name = "firmware/code" + string(i) + ".cc"
    shutil.rm(name)
    shutil.move(name + ".bak", name)
    shutil.copy(file, name)
    i += 1

print("Now run ./build/SIL <sim_file.json>")

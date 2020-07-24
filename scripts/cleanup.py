import sys
import shutil

dest_dir = str(sys.argv[1])

files_to_move = ["alive.out", "detector.out", "snapshots.out", "run.out"]
files_to_copy = ["run.in", "snapshots.in"]
dir_to_copy   = "../materials"

for f in files_to_move:
    shutil.move(f, dest_dir+"/"+f)

for f in files_to_copy:
    shutil.copy(f, dest_dir+"/"+f)

shutil.copytree(dir_to_copy, dest_dir+"/materials")

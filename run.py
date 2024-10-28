import subprocess
import os
import argparse
from pathlib import Path



parser = argparse.ArgumentParser(description='Run DoctorWho Simulation')


parser.add_argument('--printing', dest='printing', action='store_true', help='Printing command, default')
parser.add_argument('--no-printing', dest='printing', action='store_false', help='Not Printing command')
parser.set_defaults(printing=True)


args = parser.parse_args()


compile_flags = '-std=c++20 -O2 -Wall -Wextra -pedantic '
if not args.printing: compile_flags += '-DNPRINTING '


compile = f"g++ {compile_flags} -c"

source_file_folder = Path("implementation_files")

source_files = [file for file in os.listdir(source_file_folder)]
object_files = [Path(file).stem + '.o' for file in source_files]

for file in source_files:
    subprocess.run(f"{compile} {os.path.join(source_file_folder,file)}", check = True, shell = True)


link_command = f"g++ {' '.join(object_files)} -o main"
subprocess.run(link_command, check = True, shell = True)


for o in object_files:
    os.remove(o)




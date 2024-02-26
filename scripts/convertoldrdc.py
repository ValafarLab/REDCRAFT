#!/usr/local/redcraft/py/bin/python3
import sys
import os
import re
from pathlib import Path

args = sys.argv
if len(args) < 3:
    print(
        "Usage: converttordc.py <DIR PATH> <OLD PREFIX> [NEW PREFIX = nefRDC]")
    quit()
key1 = ['C', 'N', 'C', 'CA', 'HA', 'HA']
key2 = ['N', 'H', 'H', 'HA', 'H', 'H']
DIR_PATH = args[1]
OLD_PREFIX = args[2]
NEW_PREFIX = args[3] if len(args) > 3 else "nefRDC"
path = Path(DIR_PATH)
if not path.exists():
    print("Path does not exist")
    quit()
# Regex pattern finds all old files using the old prefix
pattern = r"{}\.\d*$".format(OLD_PREFIX)
old_files = [x for x in os.listdir(
    str(path.absolute())) if re.match(pattern, x)]
for i, f in enumerate(sorted(old_files)):
    # Read from old file
    with open(str(path / f)) as read:
        # Write to a new file
        new_file = str(path / (NEW_PREFIX + ".{}".format(i+1)))
        with open(new_file, "w") as write_new:
            # Write to a backup of the old file
            old_file = str(path / (OLD_PREFIX + ".{}.old".format(i+1)))
            with open(old_file, "w") as write_old:
                current_res = ""
                # For each line
                for index, line in enumerate(read.readlines()):
                    write_old.write(line)
                    # If the line is not a header, i.e. not ASP ############## ASP
                    if index % 7 != 0:
                        l = line.split()
                        if len(l) > 2:
                            print(
                                "It seems your RDC file is either corrupted or has already been converted. Aborting conversion")
                            os.remove(new_file)
                            os.remove(old_file)
                            quit()
                        j = index//7
                        k = (index-1) % 7
                        new_l = [str(j+(1 if k == 1 or k == 3 or k == 4 else 0)),
                                 current_res, key1[k], str(j+1), current_res, key2[k], l[0], l[1]]
                        write_new.write(
                            "{} {} {} {} {}  {} {} {}\n".format(*new_l))
                    else:
                        write_new.write(line)
                        current_res = line.split()[0]
    os.remove(str(path / f))

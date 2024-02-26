#!/usr/local/redcraft/py/bin/python3
"""
        pdbmine2.py
        by Julian Rachele

        Input: path to tarball from PDBMine

        Output files: x.angles which follow REDCRAFT .angles formats, all with RMSD of 1

"""
import csv
import glob
import argparse
import tarfile
import os

from pathlib import Path

parser = argparse.ArgumentParser(description='Generates REDCRAFT angles files from PDBMine data')
parser.add_argument('tarball', type=str, help='Path to a tarball retrieved from PDBMine')
parser.add_argument('output', type=str, default=".", help='Path to output the .ramangles files')
parser.add_argument('--limit', type=int, default=0, help='Place a limit on the number of angles (default: 0)', dest="limit")
parser.add_argument('--filter', type=str, default="", help='Filter out a specific protein\'s data from PDBMine results. Separate different proteins by a comma', dest="filter")
parser.add_argument('--resolution', type=int, default=10, help='Resolution of NxN subunits from which to reduce the phi/psi values', dest="resolution")

args = parser.parse_args()
limit = args.limit
fil = args.filter
#fil = set([f.strip() for f in fil.split(",")])
fil = set(fil.split())
resolution = args.resolution
tarball_path = args.tarball
output_folder = args.output

# if the folder doesn't already exist, extract the tarball
print(tarball_path)
tarball_name = Path(tarball_path).stem
if tarball_name not in os.listdir():
    print(f"Opening {tarball_name}...")
    tarball = tarfile.open(tarball_path)
    print(tarball_name)
    try:
        print(f"Extracting {tarball_name}...")
        tarball.extractall(tarball_name)
    except tarfile.ReadError:
        print("Warning: Possible corrupted tar file!")
else:
    print(f"Reading already extracted data from {tarball_name}/")

if resolution <= 0: resolution = 1

if 360 % resolution != 0:
    print("Error: Resolution must divide evenly into 360")
    exit(0)

grid_dim = 360 // resolution

print("Generating ramangles with options:")
print(f"Resolution: {resolution}")
print(f"Filtered proteins: {fil}")
print(f"Phi/psi limit: {limit}")

residue_files = sorted(glob.glob(f'{tarball_name}/residue_data/*.csv'))
if len(residue_files) == 0:
    print("Residue folder not found or missing data. Delete folder and rerun.")
    exit(-1)

for i,name in enumerate(residue_files):
    with open(name, newline='') as csvfile:
            output_residue = int(Path(name).name[:-6]) + 1
            outputfile_name = "{}.ramangles".format(output_residue)
            print("Writing {} as {}".format(name, outputfile_name))
            Path(output_folder).mkdir(exist_ok=True)
            with open(os.path.join(output_folder, outputfile_name), 'w') as outputfile:
                reader = csv.reader(csvfile)
                if (resolution < 1):
                    resolution = 1

                cache = {}
                # cache format, (phi_subregion, psi_subregion) -> (sum of phi, sum of psi, count)

                if limit == 0: limit = -1
                row_count = 0
                for row in reader:
                    try:
                        if row[0] not in fil:
                            phi = float(row[-2])
                            psi = float(row[-1])
                            if (phi < -180): phi += 360
                            if (psi < -180): psi += 360
                            if (phi > 180): phi -= 360
                            if (psi > 180): psi -= 360
                            phi_subregion = phi // resolution
                            psi_subregion = psi // resolution
                            key = (phi_subregion, psi_subregion)
                            row_count += 1
                            if key not in cache:
                                cache[key] = (phi, psi, 1)
                            else:
                                (sum_phi, sum_psi, count) = cache[key]
                                cache[key] = (sum_phi + phi, sum_psi + psi, count + 1)

                    except Exception as e:
                        print(e)
                        pass
                for key in cache:
                    (sum_phi, sum_psi, count) = cache[key]
                    avg_phi = sum_phi / count
                    avg_psi = sum_psi / count
                    outputfile.write(f'{avg_phi} {avg_psi} 1\n')

                print("Wrote file {} from PDBMine data {}; {} rows".format(outputfile_name, name, row_count))

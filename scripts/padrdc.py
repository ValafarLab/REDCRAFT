#!/usr/local/redcraft/py/bin/python3 
"""
    padrdc.py
    by Julian Rachele

    Manipulates nefRDC files and allows you to prepend and append amino acids with blank RDCs
    to existing RDC data
"""

import glob
import argparse
import os
import re

from pathlib import Path

parser = argparse.ArgumentParser(description='Prepend and append amino acids to nefRDC data')
parser.add_argument('rdcprefix', type=str, help='RDC Prefix for nefRDC file, ex. {RDC Prefix}.1, {RDC Prefix}.2, for files representing media 1 and 2.')
parser.add_argument('--prepend', type=str, help='Prepend Amino Acids, separated by commas. Can be either single char or three-length formatted.')
parser.add_argument('--append', type=str, help='Append Amino Acids, separated by commas. Can be either single char or three-length formatted.')

args = parser.parse_args()
rdcprefix = args.rdcprefix
prepend = args.prepend
append = args.append

if prepend is None:
    prepend = ""

if append is None:
    append = ""


if len(prepend) == 0 and len(append) == 0:
    print("Nothing to add to RDCs.")
    exit(-1)

prepend = [e for e in prepend.split(",") if e != '']
append = [e for e in append.split(",") if e != '']

conv = {
  "A": "ALA",
  "B": "ASX",
  "C": "CYS",
  "D": "ASP",
  "E": "GLU",
  "F": "PHE",
  "G": "GLY",
  "H": "HIS",
  "I": "ILE",
  "K": "LYS",
  "L": "LEU",
  "M": "MET",
  "N": "ASN",
  "P": "PRO",
  "Q": "GLN",
  "R": "ARG",
  "S": "SER",
  "T": "THR",
  "V": "VAL",
  "W": "TRP",
  "X": "XAA",
  "Y": "TYR",
  "Z": "GLX",
}

prepend = [conv[p] if len(p) == 1 else p for p in prepend]
append = [conv[p] if len(p) == 1 else p for p in append]

rdc_files = glob.glob(f"{rdcprefix}.*")
if len(rdc_files) == 0:
    print(f"Unable to find any RDC files with prefix {rdcprefix}")
    exit(-1)

# pattern to verify that its valid nefRDC
pattern = r"\d+\s+[A-Z]{3}\s+[A-Z]+\s+\d+\s+[A-Z]{3}\s+[A-Z]+\s+-?\d+\.?\d*\s+-?\d+\.?\d*\s+"
# form for prepend and append
form = """{res} 999 {res}
0  {res} C 0 {res} N 999 1
0  {res} N 0 {res} H 999 1
0  {res} C 0 {res} H 999 1
0  {res} CA 0 {res} HA 999 1
0  {res} HA 0 {res} H 999 1
0  {res} HA 0 {res} H 999 1
"""

# validate the files first before touching them
for medium in rdc_files:
    with open(medium) as f:
        if not re.search(pattern, f.read(), re.MULTILINE):
            print(f"Not a valid nefRDC format: {medium}")
            exit(-1)


# write the prepended and appended amino acids
for medium in sorted(rdc_files):
    with open(medium, "r+") as f:
        print(f"Writing to {medium}...")
        content = f.read()
        f.seek(0)
        f.truncate()

        written_content = ""
        if len(prepend) > 0:
            written_content += "".join([form.format(res=res) for res in prepend])
            print(f"Prepended {', '.join(prepend)}")
        written_content += content
        if len(append) > 0:
            written_content += "".join([form.format(res=res) for res in append])
            print(f"Appended {', '.join(append)}")

        f.write(written_content)


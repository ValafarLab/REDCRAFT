#!/usr/local/redcraft/py/bin/python3
import sys
import os
import re
from pathlib import Path

args = sys.argv
if len(args) < 2:
    print(
        "Usage: getsequence.py <RDC PATH>")
    print("Outputs single-character amino acid sequence to stdout")
    quit()
DIR_PATH = args[1]
path = Path(DIR_PATH)
if not path.exists():
    print("Path does not exist")
    exit(1)
# extract amino acid
pattern = r"^([A-Z]{3})\s+\d+\.?\d+\s*" 
conv = {
  "ALA": "A",
  "ASX": "B",
  "CYS": "C",
  "ASP": "D",
  "GLU": "E",
  "PHE": "F",
  "GLY": "G",
  "HIS": "H",
  "ILE": "I",
  "LYS": "K",
  "LEU": "L",
  "MET": "M",
  "ASN": "N",
  "PRO": "P",
  "GLN": "Q",
  "ARG": "R",
  "SER": "S",
  "THR": "T",
  "VAL": "V",
  "TRP": "W",
  "XAA": "X",
  "TYR": "Y",
  "GLX": "Z",
}

with open(path) as f:
    c = f.read()
    r = ""
    matches = re.findall(pattern, c, re.MULTILINE)
    for match in matches:
        if match in conv:
            r += conv[match]
    print(r, end="")

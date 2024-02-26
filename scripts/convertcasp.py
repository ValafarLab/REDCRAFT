#!/usr/local/redcraft/py/bin/python3
"""
    convertcasp.py
    by Julian Rachele

    Converts CASP13 NMR data to REDCRAFT usable RDC files
"""
import glob 
import re
import os

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

# get protein name
re_protein = r"(.+)_.+\.txt$"
protein_name = re.findall(re_protein, "\n".join(os.listdir()))
if len(protein_name) > 0:
    protein_name = protein_name[0]
else:
    print("Unable to determine protein name")
    exit(-1)


# get RDC file
re_rdc = r"_RDC.txt$"
rdc_file = [f for f in os.listdir() if re.search(re_rdc, f)]
if len(rdc_file) > 0:
    rdc_file = rdc_file[0]
else:
    print("No RDC file found at: {}".format(os.curdir))
    exit(-1)

# get seq file
re_seq = r"_seq.txt$"
seq_file = [f for f in os.listdir() if re.search(re_seq, f)]
if len(seq_file) > 0:
    seq_file = seq_file[0]
else:
    print("No seq file found at: {}".format(os.curdir))
    exit(-1)

seq = ""
with open(seq_file) as f:
    try:
        seq = f.readlines()[1].strip()
    except:
        print("Unable to read sequence file")
        exit(-1)

# store rdcs in mapping: residue -> (m1, m2)
# rdcs are only valid for N-H
rdcs = {}
with open(rdc_file) as f:
    re_rdc = r"(\d+)\s*(-?\d+\.?\d+)\s*(-?\d+\.?\d+)$"
    try:
        for (residue, m1, m2) in re.findall(re_rdc, f.read(), re.MULTILINE):
            residue = int(residue)
            m1 = float(m1)
            m2 = float(m2)
            # store in dicitonary for easy lookup later
            rdcs[residue] = (m1, m2)
    except e:
        print("Unable to read RDCS")
        print(e)
        exit(-1)

nef_template = """{res} 999 {res}
{j} {res} C 1 {res} N 999 1
{i} {res} N 1 {res} H {rdc} 1
{j} {res} C 1 {res} H 999 1
{i} {res} CA 1 {res} HA 999 1
{i} {res} HA 1 {res} H 999 1
{j} {res} HA 1 {res} H 999 1
"""


with open("{}.1".format(protein_name), "w") as f1:
    with open("{}.2".format(protein_name), "w") as f2:
        for j,small_res in enumerate(seq):
            # first convert individual character into three character counterpart
            res = conv[small_res]
            i = j + 1
            (m1, m2) = rdcs[i] if i in rdcs else (999, 999)
            f1.write(nef_template.format(
                res=res,
                rdc=m1,
                i=i,
                j=j
            ))
            f2.write(nef_template.format(
                res=res,
                rdc=m2,
                i=i,
                j=j
            ))

print("Wrote {}.2 and {}.2 in {}".format(protein_name, protein_name, os.curdir))
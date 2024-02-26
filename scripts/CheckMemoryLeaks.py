#!/usr/bin/python3
"""
    CheckMemoryLeaks.py (currently only working on Linux)
    Julian Rachele
    10 Sept 2020
    -------------------------------------------------------------
    Check for memory leaks in REDCRAFT with Valgrind, in bulk.
    Creates a folder with respect to each run and the output of the command, to be inspected manually.
    New commands can be created by modifying the template dict provided below
    Test collections can also be loaded using JSON
"""
import json
import os
import shutil
import subprocess
from datetime import datetime
from pathlib import Path

TEMPLATE_DICT = {
        # Specify the REDCRAFT binary 
    "binary": "stage1",
    "params": ["nefRDC", "1", "0", "100"],
    "valgrindopts": ["--leak-check=full"],
}

# This is for running stage 2
TEMPLATE_STAGE2_CONFIG = {
    "Decimation": False,
    "Refinement": False,
    "Run_Type": "new",
    "RDC_File_Prefix": "nefRDC",
    "Start_Residue": 1,
    "Stop_Residue": 10,
    "Media_Count": 2,
    "Default_Search_Depth": 500,
    "Data_Path": ".",
    "Cluster_Sensitivity": "",
    "Score_Threshold": "",
    "Decimation_Ranges": "",
    "Cluster_Count": "",
    "Maximum_Number_Of_Additional_Structures": "",
    "script1": "./minimize.prl"
}

# List of dictionaries that supply the test commands. Each dict will have its own output file
# this is the default test that is ran if no json file is provided
tests = [
    {
        "binary": "stage1",
        "params": ["nefRDC", "1", "0", "10"],
        "valgrindopts": ["--leak-check=full"],
    },
    {
        "binary": "stage1",
        "params": ["nefRDC", "2", "0", "10"],
        "valgrindopts": ["--leak-check=full"],
    },
    {
        "binary": "stage1",
        "params": ["nefRDC", "all", "0", "10"],
        "valgrindopts": ["--leak-check=full"],
    },
    {
        "binary": "stage1",
        "params": ["nefRDC", "all", "0", "10"],
        "valgrindopts": ["--leak-check=full"],
    },
    {
        "binary": "stage2",
        "params": [],
        "config": {
            "Decimation": False,
            "Refinement": False,
            "Run_Type": "new",
            "RDC_File_Prefix": "nefRDC",
            "Start_Residue": 1,
            "Stop_Residue": 10,
            "Media_Count": 2,
            "Default_Search_Depth": 10,
            "Data_Path": ".",
            "Cluster_Sensitivity": "",
            "Score_Threshold": "",
            "Decimation_Ranges": "",
            "Cluster_Count": "",
            "Maximum_Number_of_Additional_Structures": "",
            "script1": "./minimize.prl"
        },
        "valgrindopts": ["--leak-check=full"],
    },
]

def test_to_string(test):
    return "Testing binary {}; parameters: {}; valgrind options: {}".format(test["binary"], ", ".join(test["params"]), ", ".join(test["valgrindopts"]))

def conf_to_string(conf):
    return """ 
        [Run_Settings]
        Run_Type={Run_Type}
        RDC_File_Prefix={RDC_File_Prefix}
        Start_Residue={Start_Residue}
        Stop_Residue={Stop_Residue}
        Media_Count={Media_Count}
        Default_Search_Depth={Default_Search_Depth}
        Data_Path={Data_Path}

        [Decimation_Settings]
        {d}{cs}Cluster_Sensitivity={Cluster_Sensitivity}
        {d}Score_Threshold={Score_Threshold}
        {d}Decimation_Ranges={Decimation_Ranges}
        {d}{cc}Cluster_Count={Cluster_Count}
        {d}Maximum_Number_of_Additional_Structures={Maximum_Number_of_Additional_Structures}

        [Refinement]
        {r}script1={script1}

        [OTEstimation]
            """.format(        
                Run_Type=conf["Run_Type"],
                RDC_File_Prefix=conf["RDC_File_Prefix"],
                Start_Residue=conf["Start_Residue"],
                Stop_Residue=conf["Stop_Residue"],
                Media_Count=conf["Media_Count"],
                Default_Search_Depth=conf["Default_Search_Depth"],
                Data_Path=conf["Data_Path"],
                Cluster_Sensitivity=conf["Cluster_Sensitivity"],
                Score_Threshold=conf["Score_Threshold"],
                Decimation_Ranges=conf["Decimation_Ranges"],
                Cluster_Count=conf["Cluster_Count"],
                Maximum_Number_of_Additional_Structures=conf["Maximum_Number_of_Additional_Structures"],
                script1=conf["script1"],
                # finally, check if decimation and refinement are enabled
                d="" if conf["Decimation"] else "#",
                r="" if conf["Refinement"] else "#",
                # if count is used then sensitivity is disabled and vice versa
                cs="" if conf["Cluster_Count"] == "" else "#",
                cc="" if conf["Cluster_Sensitivity"] == "" else "#",
            )

print("Check for memory leaks in REDCRAFT")
folder = input("Specify path to folder: ")
folder_path = Path(folder).absolute()
print("Folder selected: " + str(folder_path))
if (input("Confirm? (y/n): ").lower() == "y"):
    verbose = input("Verbose? (y/n): ").lower() == "y"
    load_tests = input("Specify JSON tests file? (y/n): ").lower() == "y"
    if (load_tests):
        # Load tests from JSON file
        json_p = input("Specify path to json tests file: ")
        json_path = Path(json_p).absolute()
        print("Running tests from: {}".format(str(json_path)))
        with open(json_path) as json_file:
            data = json.load(json_file)
            tests = data

    # Create output folder
    now = datetime.now()
    dt_string = now.strftime("%d%m%Y_%H%M%S")
    output_folder = "LeakCheck_" + dt_string
    os.mkdir(folder_path / output_folder)
    if (os.path.exists(folder_path / "default.conf")):
        # Make a backup of the previous default.conf
        shutil.copyfile(folder_path / "default.conf", folder_path / "backup_redcraft.conf.old")

    for i,test in enumerate(tests):
        print("Test {}; {}".format(i+1, test_to_string(test)))
        # We must account for the edge case of stage 2 testing, where we will have to define our own default.conf
        if ("config" in test):
            with open(folder_path / "default.conf", 'w') as f:
                f.write("Test config used:")
                f.write(conf_to_string(test["config"]))
        p = subprocess.Popen(["valgrind", *test["valgrindopts"], "/usr/local/redcraft/bin/" + test["binary"], *test["params"]], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, bufsize=1)
        with open(folder_path / output_folder / "test_{}_{}.txt".format(test["binary"], "_".join(test["params"])), 'a') as f:
            for line in iter(p.stdout.readline, b''):
                if (verbose):
                    print("{}> {}".format(i+1, line.decode('utf-8')))
                f.write(line.decode('utf-8'))
            p.stdout.close()
            p.wait()
        print("-------------------------")

    if (os.path.exists(folder_path / "backup_redcraft.conf.old")):
        # Restore the backup of default.conf
        os.rename(folder_path / "backup_redcraft.conf.old", folder_path / "default.conf")
    




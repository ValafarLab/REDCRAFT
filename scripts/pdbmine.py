#!/usr/local/redcraft/py/bin/python3
#imports

import sys          # to get command line parameters
import os           # to perform directory and file operations
import getopt       # to process command line parameters
import yaml         # to read yaml files                    (pip install pyyaml)
import json         # to parse JSON
import requests     # to send REST requests                 (pip install requests)
import time         # to sleep while polling
import logging      # to log messages
# In buildFilesFromKMeans is an import using sklearn        (pip install sklearn)
# In buildFilesFromKMeans is an import using pyclustering   (pip install pyclustering)

#Program Return Codes
SUCCESS = 0
OPT_ERROR = 2
UNKNOWN_OPT = 3
#NO_CONFIG = 3
BAD_YAML = 4
#INVALID_CONFIG = 5
MISSING_CONFIG_KMEANS = 6

#Process the command line options and return the yaml file and output directory
def processCommandLineOptions(argv):
  USAGE = "pdbmine.py -y <yaml file> -i <protein to ignore> -o <output directory> -l <log level>"
  PARSING_ERROR_MSG = "Error parsing options"

  yaml = None
  protein = None
  outputDir = None

  
  #Read and process the command line arguments
  try:
    opts, args = getopt.getopt(argv, "hi:y:o:l:" , ["help", "ignore=", "yaml=", "output=", "logging="])
  except getopt.GetoptError:
    print(PARSING_ERROR_MSG)
    print(USAGE)
    sys.exit(OPT_ERROR)

  for opt, arg in opts:
    if opt in ("-h", "--help"):
      print(USAGE)
      sys.exit(SUCCESS)
    elif opt in ("-y", "--yaml"):
      yaml = arg
    elif opt in ("-o", "--output"):
      outputDir = arg 
    elif opt in ("-i", "--ignore"):
      protein = arg
    elif opt in ("-l", "--logging"):
      numeric_level = getattr(logging, arg.upper(), None)
      if not isinstance(numeric_level, int):
        raise ValueError('Invalid log level: %s' % loglevel)
      logging.basicConfig(level=numeric_level, handlers=[
        logging.FileHandler("pdbmine.log"),
        logging.StreamHandler()
      ])
    else:
      logging.error('Unknown Option %s, %s' % (opt, arg))
      print(USAGE)
      sys.exit(UNKNOWN_OPT)
    
  #If no yaml file was provided, set a default 
  if None != yaml:
    yaml = os.path.abspath(yaml)
    yamlDir = os.path.dirname(yaml)

    if not os.path.isfile(yaml):
      logging.error("%s is not a valid file" % yaml)
      sys.exit(BAD_YAML)

  return yaml, outputDir, protein

#Validate the config values
def validateConfig(config):
  return True

#Build the config based of command line options and the yaml file
def buildConfig(yamlFile, outputDir, ignorePID):
  config = dict({'server': {'uri': None, 'port': None}, 'constraints': {'filter': None}, 'results': {'directory': None}})

  #If there's a yaml config
  if yamlFile != None:
    #Read the expirement configuration from the yaml file
    with open(yamlFile) as file:
      config = yaml.load(file, Loader=yaml.FullLoader)

    #If the config is valid, then set the output directory and PID's to ignore
    if None != ignorePID:
      if None != config['constraints']['filter']:
        config['constraints']['filter'].append(ignorePID)
      else:
        config['constraints']['filter'] = [ignorePID]

    if None != outputDir:
      #Just overwrite the config file if something is given on the command line
      config['results']['directory'] = outputDir
  else:
    #If no config, just set some defaults
    if None != ignorePID:
      config['constraints']['filter'] = [ignorePID]

    if None == outputDir:
      config['results']['directory'] = "."
    else:
      config['results']['directory'] = outputDir

    config['server']['uri'] = "https://ifestos.cse.sc.edu"
    config['server']['port'] = 443

  #Validate the config
  if not validateConfig(config):
    logging.error('Config validation failed')
    sys.exit(INVALID_CONFIG)

  #Convert to full path
  config['results']['directory'] = os.path.abspath(config['results']['directory'])

  #If the output directory doesn't exist, create it
  if not os.path.isdir(config['results']['directory']):
    os.makedirs(config['results']['directory'])

  print(config)

  return config

#Query ifestos PDBMine
def queryServer(config):
  logging.info("Querying PDBMine at %s" % config['server']['uri'])
  query_results = []
  pdbmine_uri = "%s:%d/v1/api/query" % (config['server']['uri'], config['server']['port'])
  pdbmine_headers = {'Content-Type': 'application/json', 'Accept': '*/*'}
  query_data = json.dumps(config['query'])

  logging.debug("URL=%s" % pdbmine_uri)
  logging.debug(query_data)

  # Send PDB Mine Request
  response = requests.post(pdbmine_uri,
                           query_data,
                           headers=pdbmine_headers)

  logging.debug("Status code: %s" % response.status_code)
  pdbmine_response = response.json()

  logging.debug("Printing Post JSON response data")
  logging.debug(": %s" % pdbmine_response["status"])
  logging.info("Query ID: %s" % pdbmine_response["queryID"])

  pdbmine_query_id = pdbmine_response["queryID"]

  if response.status_code == 201: 
    #Loop until the results are ready
    while response.status_code != 200:
      time.sleep(config['server']['poll'])
      logging.info("Waiting on server ...")
      
      pdbmine_get_uri = "%s/%s/" % (pdbmine_uri, pdbmine_query_id)
      logging.debug(pdbmine_get_uri)
      response = requests.get(pdbmine_get_uri)

      logging.debug("Status code: %s" % response.status_code)

  query_results = response.json()
  logging.debug(query_results)
  return query_results

#Formats the results into an array of arrays of tuples
def formatData(config, query_data):
  #initialize the results set
  result_set = []

  #TODO: Test this for 3 code residues
  if 1 == config['query']['codeLength']:
    for i in range(len(config['query']['residueChain'])): #residue code size of 1)
      result_set.append([])
  else:
    for i in range(len(config['query']['residueChain'])/3): #assuming residue code size of 3)
      result_set.append([])   
  position = 0

  #Parse the data so we can create lists
  for frame in query_data['frames']:
    #print(frame)
    for protein in query_data['frames'][frame]:
      #skip the protein if it's in the filter list
      #print("Adding data for %s" % protein)
      if protein[0:4].lower() in config['constraints']['filter'] or protein[0:4].upper() in config['constraints']['filter']:
        logging.debug("Skipping %s" % protein)
        continue
      angles = []
      for model in query_data['frames'][frame][protein]:
        for num, residue in enumerate(model, start=0):
          #print("Adding angles %.2f %.2f to position %d: Residue %s" % (residue['phi'], residue['psi'], position+num, residue['residueName']))
          result_set[position+num].append((residue['phi'], residue['psi']))
          #print(residue['residueName'])
          angles.append(residue['phi'])
          angles.append(residue['psi'])
        # each angle is one dataset for the clustering algorithm
        #print(angles)
        angles = []
    position = position + 1

  return result_set

#Run the K Means algorithm with a set K
def buildFilesFromKMeans(config):
  #Check to make sure we have K Means configs
  if "k-means" in config['method']:
    from sklearn.cluster import KMeans

    #Set a default value of K and warn
    k_value = 3
    if "k" in config['method']['k-means']:
      k_value = config['method']['k-means']['k']
    else:
      logging.warning("Setting default K value to %d since no config found!" % k_value)

    max_iterations = 300
    if "max_iter" in config['method']['k-means']:
      max_iterations = config['method']['k-means']['max_iter']

    runs = 10
    if "n_init" in config['method']['k-means']:
      runs = config['method']['k-means']['n_init']

    random_state = 0

    logging.info("Running K-Means with set k of %d" % k_value)
    logging.info("Running K-Means for %d iterations" % runs)
    logging.info("Running K-Means a max of %d times" % max_iterations)

    #run query
    query_data = queryServer(config)

    #format the results into an array of arrays of tuples
    result_set = formatData(config, query_data)

    for i, result in enumerate(result_set):
      logging.debug(result)
      logging.info("Printing angles for residue %d: %s" % (i+1, config['query']['residueChain'][i]))
      
      #phi_list = [a_tuple[0] for a_tuple in result[i]]
      #psi_list = [a_tuple[1] for a_tuple in result[i]]

      kmeans = KMeans(n_clusters=k_value, init='k-means++', max_iter=max_iterations, n_init=runs, random_state=0)
      pred_y = kmeans.fit_predict(result)

      # Write the files
      filename = os.path.join(config['results']['directory'], str(i+1) + ".ramangles")
      with open(filename, "w") as output_file: 
        for entry in kmeans.cluster_centers_:
          output_file.write("%.1f\t%.1f\t0\n" % (entry[0], entry[1]))
  else:
    #error out
    logging.error("No config set for k-means")
    sys.exit(MISSING_CONFIG_KMEANS)

#Run the K Means algorithm with a set K
def buildFilesFromXMeans(config):
  from pyclustering.cluster.xmeans import xmeans
  from pyclustering.cluster.center_initializer import kmeans_plusplus_initializer

  #Check to make sure we have K Means configs
  if "x-means" in config['method']:
     #Set a default value of K and warn
    initial_centers = 2
    if "init_centers" in config['method']['x-means']:
      initial_centers = config['method']['x-means']['init_centers']
    else:
      logging.warning("Defaulting initial centers to %d since no config found!" % initial_centers)

    max_clusters = 20
    if "max_clusters" in config['method']['x-means']:
      max_clusters = config['method']['x-means']['max_clusters']
    else:
      logging.warning("Defaulting initial centers to %d since no config found!" % max_clusters)

    logging.info("Running X-Means with %d initial clusters and %d max clusters" % (initial_centers, max_clusters))

    #run query
    query_data = queryServer(config)

    #format the results into an array of arrays of tuples
    result_set = formatData(config, query_data)

    for i, result in enumerate(result_set):
      logging.debug(result)
      logging.info("Printing angles for residue %d: %s" % (i+1, config['query']['residueChain'][i]))
      
      #phi_list = [a_tuple[0] for a_tuple in result[i]]
      #psi_list = [a_tuple[1] for a_tuple in result[i]]

      init_clusters = kmeans_plusplus_initializer(result, initial_centers).initialize()

      #result = [list(row) for row in result]
      xmeans_instance = xmeans(result, init_clusters, max_clusters)
      xmeans_instance.process()

      centers = xmeans_instance.get_centers()

      # Write the files
      filename = os.path.join(config['results']['directory'], str(i+1) + ".ramangles")
      logging.info("Centers found: %d" % len(xmeans_instance.get_centers()))
      with open(filename, "w") as output_file: 
        for entry in xmeans_instance.get_centers():
          output_file.write("%.1f\t%.1f\t0\n" % (entry[0], entry[1]))
  else:
    #error out
    logging.error("No config set for k-means")
    sys.exit(MISSING_CONFIG_KMEANS)

#Main program
def main(argv):
  #Get the YAML file from the command line parameters
  yamlFile, outputDir, ignorePID = processCommandLineOptions(argv)

  #Create the config dictionary
  config = buildConfig(yamlFile, outputDir, ignorePID)
  logging.debug("Config: %s" % config)

  #Run Clustering algorithm
  if "k-means" in config['method']:
    buildFilesFromKMeans(config)
  elif "x-means" in config['method']:
    buildFilesFromXMeans(config)
  else:
    logging.warning("No supported clustering algorithm found")

# If the program is run stand alone, call the main function  
if __name__== "__main__":
  main(sys.argv[1:])
  sys.exit(SUCCESS)

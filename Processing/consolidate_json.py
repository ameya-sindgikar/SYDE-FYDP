import os
import fnmatch
import json
import numpy as np
import matplotlib.pyplot as plt

directory = 'Data/Expert'

consolidatedExpData = []

for filename in os.listdir(directory):
    if filename.endswith(".json") and fnmatch.fnmatch(filename, 'TakeOffExp*'):

        with open(os.path.join(directory, filename)) as data_file:
            expCollection = json.load(data_file)
        currentTrialData = []
        for row in range(1, len(expCollection)):
            expDataArray = expCollection[row]['data']
            currentTrialData.append(expDataArray)
        currTrialData = np.array(currentTrialData)
        print('File:', os.path.join(directory, filename), 'Shape:', np.shape(currTrialData))
    consolidatedExpData.append(currTrialData)

#import pymongo
import os
import fnmatch
import json
import numpy as np

#Connect to mongoDB and get data from collection
#client = pymongo.MongoClient()
#db = client.TrainingDataDB

currentExpData = []
extractedExpData = []

directory = 'Data/Expert'

for filename in os.listdir(directory):
    if filename.endswith(".json") and fnmatch.fnmatch(filename, 'TakeOffExp*'):

        with open(os.path.join(directory, filename)) as data_file:
            expCollection = json.load(data_file)
        #expDocuments = expCollection.find({})
        currentTrialData = []
        for row in range(1, len(expCollection)):
            expDataArray = expCollection[row]['data']
            currentExpData.append(expDataArray)
            currentTrialData.append(expDataArray)
        currTrial = np.array(currentTrialData) #make it into numpy array
        #print(np.shape(currTrial))
        #print currTrial.shape
        # get the acceleration in x
        currTrialAccX = np.array(currTrial[:,0])
        currTrialImpX = np.gradient(currTrialAccX)
        # get the acceleration in y
        currTrialAccY = np.array(currTrial[:,1])
        currTrialImpY = np.gradient(currTrialAccY)
        # get the acceleration in z
        currTrialAccZ = np.array(currTrial[:,2])
        currTrialImpZ = np.gradient(currTrialAccZ)
        # get the magnitude of all acceleration
        currTrialMag = np.array(currTrial[:,3])
        currTrialImpMag = np.gradient(currTrialMag)
        # get the gyroscope values in x
        currTrialGyrX = np.array(currTrial[:,4])
        currTrialAngVelX = np.gradient(currTrialGyrX)
        # get the gyroscope values in y
        currTrialGyrY = np.array(currTrial[:,5])
        currTrialAngVelY = np.gradient(currTrialGyrY)
        # get the gyroscope values in z
        currTrialGyrZ = np.array(currTrial[:,6])
        currTrialAngVelZ = np.gradient(currTrialGyrZ)
        # get the pitch
        currTrialPitch = np.array(currTrial[:,7])
        currTrialDerPitchX = np.gradient(currTrialPitch)
        # get the roll
        currTrialRoll = np.array(currTrial[:,8])
        currTrialDerRollX = np.gradient(currTrialRoll)
        # get the altitude
        currTrialAlt = np.array(currTrial[:,9])
        currTrialDerAlt = np.gradient(currTrialAlt)

        currTrialDer = np.array([currTrialImpX,
                            currTrialImpY,
                            currTrialImpZ,
                            currTrialImpMag,
                            currTrialAngVelX,
                            currTrialAngVelY,
                            currTrialAngVelZ,
                            currTrialDerPitchX,
                            currTrialDerRollX,
                            currTrialDerAlt])
        # find the transpose because we didnt store in columns but rows
        currTrialDer = currTrialDer.T
        # -------------- Raw Data ------------
        # get standard deviation
        currTrialStd = np.std(currTrial, axis=0)
        #print "Standard deviation--------------------------------------------------------------------"
        #print currTrialStd
        currTrialMean = np.mean(currTrial, axis=0)
        #print "Mean----------------------------------------------------------------------------------"
        #print currTrialMean
        currTrialVar = np.var(currTrial, axis=0)
        #print "Variance----------------------------------------------------------------------------------"
        currTrialMax = np.max(currTrial, axis=0)
        #print Max
        # ------------- Gradient Data ------------
        # get standard deviation
        currTrialDerStd = np.std(currTrialDer, axis=0)
        #print "Standard deviation--------------------------------------------------------------------"
        #print currTrialStd
        currTrialDerMean = np.mean(currTrialDer, axis=0)
        #print "Mean----------------------------------------------------------------------------------"
        #print currTrialMean
        currTrialDerVar = np.var(currTrialDer, axis=0)
        #print "Variance----------------------------------------------------------------------------------"
        currTrialDerMax = np.max(currTrialDer, axis=0)
        #print Max

        #print currTrialVar
        #currTrialAllExtData = np.array(zip(currTrialStd, currTrialMean))
        currTrialAllExtData = np.concatenate((currTrialStd,
                                            currTrialMean,
                                            currTrialVar,
                                            currTrialMax,
                                            currTrialDerStd,
                                            currTrialDerMean,
                                            currTrialDerVar,
                                            currTrialDerMax), axis=0)
        print(currTrialAllExtData.shape)
        print(currTrialAllExtData)
        #print "Concatenated---------------------------------------------------------------------------"
        #print currTrialAllExtData
        #print "Appended-------------------------------------------------------------------------------"
        #print currTrialAllExtData.shape
        extractedExpData.append(currTrialAllExtData)
        #print extractedExpData
        #print "END-------------------------------------------------------------------------------------"
expertDataArray = np.array(currentExpData)
expertExtractedDataArray = np.array(extractedExpData)
print (expertExtractedDataArray.shape)
#print expertDataArray.shape
#print discExpDataArray

// Read in data row by row


float data1[] = {2, 2, 2};
float data2[] = {4, 4, 4};
float distance = euclideanDistance(data1, data2, 3);


// Get the euclidean distance between two points
float euclideanDistance(float[] instance1,float[] instance2,int length) {
  int distance = 0;
  for (int i = 0; i < length; i++){
    distance += pow((instance1[i] - instance2[i]), 2);
  };
  println(sqrt(distance));
  return sqrt(distance);
}

// Get Nearest Neighbours
float[] getNeighbours(float[] trainingSet,float[] testInstance,int k) {
  // create an array to get distances
  float[] distances = new float[trainingSet.length];
  // get the length of the test instance
  int length = testInstance.length - 1;
  float dist = 0;
  for (int i = 0; i < trainingSet.length; i++) {
    //dist = euclideanDistance(testInstance, trainingSet[i], length);
    distances[i] = dist;
  };
  float[] neighbours = new float[distances.length];
  for (int i = 0;  i < k; i++) {
    neighbours[i] = distances[i];
  }
  
  return neighbours;
}
// Read in data row by row


float data1[] = {2, 2, 2};
float data2[] = {4, 4, 4};
float distance = euclideanDistance(data1, data2, 3);

float trainSet[][] = {{2, 2, 2}, {4, 4, 4}};
float testInstance[] = {5, 5, 5};
int k = 1;
float neighbours = getNeighbours(trainSet[][], testInstance[], 1);
//print(neighbors)

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
  float[][] distances = new float[trainingSet.length][2];
  // get the length of the test instance
  int length = testInstance.length - 1;
  float dist = 0;
  for (int i = 0; i < trainingSet.length; i++) {
    dist = euclideanDistance(testInstance, trainingSet, length);
    distances[i][1] = trainingSet[i];
    distances[i][2] = dist;
    //distances.sort;
    println("hi", distances[i][1], distances[i][2]);
  };
  float[] neighbours = new float[distances.length];
  for (int i = 0;  i < k; i++) {
    //neighbours[i] = distances[i];
  }
  
  return neighbours;
}
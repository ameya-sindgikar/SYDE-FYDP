# open JSON code

import json
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.colors import ListedColormap
from time import time
import numpy as np
from matplotlib import offsetbox
from sklearn import manifold, datasets, decomposition, ensemble,discriminant_analysis, random_projection

cmap_bold = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])
cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA', '#AAAAFF'])

with open('Expert/TakeOffExp1.json') as data_file:
    takeoffexp1 = json.load(data_file)

xlistexp = []
ylistexp = []
zlistexp = []
coordexp = []
    
for x in range(1, len(takeoffexp1)):
    xdir = takeoffexp1[x]['data'][1]
    xlistexp.append(xdir)
    ydir = takeoffexp1[x]['data'][2]
    ylistexp.append(ydir)
    zdir = takeoffexp1[x]['data'][3]
    zlistexp.append(zdir)
    coordexp.append(takeoffexp1[x]['data'][0:9])
    #print ('x is: ', xdir, ' y is: ', ydir, ' z is: ', zdir)

with open('Novice/TakeOffNov1.json') as data_file:
    takeoffnov1 = json.load(data_file)

xlistnov = []
ylistnov = []
zlistnov = []
coordnov = []
    
for y in range(1, len(takeoffnov1)):
    xdir = takeoffnov1[y]['data'][1]
    xlistnov.append(xdir)
    ydir = takeoffnov1[y]['data'][2]
    ylistnov.append(ydir)
    zdir = takeoffnov1[y]['data'][3]
    zlistnov.append(zdir)
    coordnov.append(takeoffnov1[y]['data'][0:9])
    #print ('x is: ', xdir, ' y is: ', ydir, ' z is: ', zdir)
    

# Make a target vector for classification
# digits = datasets.load_digits(n_class=10)
num1 = np.ones((len(takeoffexp1) -1 ,1), dtype=np.int)
num2 = 2 * np.ones((len(takeoffnov1) - 1,1), dtype=np.int)
y = np.concatenate((num1,num2), axis=0)

X = np.concatenate((coordexp, coordnov), axis=0)

#X = digits.data
#y = digits.target

xx=[]
for i in range(0,len(X)):
    xx.append((X[i] - np.min(X)) / (np.max(X) - np.min(X)))
X=xx

pca=decomposition.TruncatedSVD(n_components=2)
X_pca = decomposition.TruncatedSVD(n_components=2).fit_transform(X)
pca2=pca.fit(X)
pca2.components_
pca3=pca2.transform(X)

tsne = manifold.TSNE(n_components=2, init='pca', random_state=0,perplexity=70,verbose=1,n_iter=1500)
X_tsne = tsne.fit_transform(X)

fig = plt.figure(figsize=(10,5))
plt.subplot2grid((1,2), (0,0))
plt.title('PRINCIPAL COMPONENTS ANALYSIS')
plt.scatter(X_pca[:, 0], X_pca[:, 1], c=y, cmap = cmap_bold)
plt.subplot2grid((1,2), (0,1), rowspan=1, colspan=2)
plt.title('t-SNE')
plt.scatter(X_tsne[:, 0], X_tsne[:, 1], c=y, cmap = cmap_bold)
plt.show()

    
    
    
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

ax.scatter(xlistexp, ylistexp, zlistexp, color='r')
ax.scatter(xlistnov, ylistnov, zlistnov, color='b')
plt.show()



# rotate the axes and update
#for angle in range(0, 360):
#    ax.view_init(30, angle)
#    plt.draw()
#    plt.pause(.001)
#pprint(takeoffexp1)
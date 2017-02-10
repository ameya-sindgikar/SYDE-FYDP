# open JSON code

import json
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.colors import ListedColormap
from pprint import pprint


with open('TestData/Expert/TakeOffExp1.json') as data_file:
    takeoffexp1 = json.load(data_file)

xlistexp = []
ylistexp = []
zlistexp = []

for x in range(1, len(takeoffexp1)):
    xdir = takeoffexp1[x]['data'][1]
    xlistexp.append(xdir)
    ydir = takeoffexp1[x]['data'][2]
    ylistexp.append(ydir)
    zdir = takeoffexp1[x]['data'][3]
    zlistexp.append(zdir)
    #print ('x is: ', xdir, ' y is: ', ydir, ' z is: ', zdir)

with open('TestData/Novice/TakeOffNov1.json') as data_file:
    takeoffnov1 = json.load(data_file)

xlistnov = []
ylistnov = []
zlistnov = []

for y in range(1, len(takeoffnov1)):
    xdir = takeoffnov1[y]['data'][1]
    xlistnov.append(xdir)
    ydir = takeoffnov1[y]['data'][2]
    ylistnov.append(ydir)
    zdir = takeoffnov1[y]['data'][3]
    zlistnov.append(zdir)
    print ('x is: ', xdir, ' y is: ', ydir, ' z is: ', zdir)


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
cmap_bold = ListedColormap(['#FF0000', '#00FF00', '#0000FF'])
cmap_light = ListedColormap(['#FFAAAA', '#AAFFAA', '#AAAAFF'])
ax.scatter(xlistexp, ylistexp, zlistexp, color='r')
ax.scatter(xlistnov, ylistnov, zlistnov, color='b')
plt.show()
# rotate the axes and update
#for angle in range(0, 360):
#    ax.view_init(30, angle)
#    plt.draw()
#    plt.pause(.001)
#pprint(takeoffexp1)

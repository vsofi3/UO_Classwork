import sys
file = sys.stdin


def getDag(n):
    vertices = file.readline()              #stores number of vertices in the graph
    edges = file.readline()                 #stores number of edges, also moves to the next line of the file

    short =  [0] * int(vertices)            #an array for the shortest possible paths
    long  =  [0] * int(vertices)
    shortD = [0] * int(vertices)            #an array for the number of shortest paths
    longD =  [0] * int(vertices)

    short[1] = 1            #for each array, set the second index = to 1
    long[1] = 1
    shortD[0] = 1           #for the number of longest/shortest paths, set the first index = 1
    longD[0] = 1

    for line in file:
        line = line.replace("\n", "")           #gets ride of \n characters at the end of string
        line = line.split()                     #splits the string at the space
        i = int(line[0])
        j = int(line[1])
        #followed the logic used on geeksforgeeks where the distances of the adjacent vertex is updated
        #using the distance of the current vertex.

        if short[j - 1] == 0:           #finds the shortest path
            short[j - 1] = short[i - 1] + 1     
        if short[j - 1] > short[i - 1] + 1:
            short[j - 1] = short[i - 1] + 1

        elif short[j - 1] == short[i - 1] + 1:        #finds the number of shortest paths
            shortD[j - 1] += shortD[i - 1]

        if long[j - 1] == 0:                        #finds the longest path
            long[j - 1] = long[i - 1] + 1
        if long[j - 1] < long[i - 1] + 1:
            long[j - 1] = long[i - 1] + 1

        elif long[j-1] == long[i-1] + 1:        #finds the number of longest paths
            longD[j -1] += longD[i-1]

    print("Shortest Path: {}".format(short[-1]))
    print("Number of Short Paths: {}".format(shortD[-1]))
    print("Longest Path: {}".format(long[-1]))
    print("Number of Long Paths: {}".format(longD[-1]))

getDag(3)

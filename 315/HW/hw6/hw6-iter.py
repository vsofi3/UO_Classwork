import sys
INF = sys.maxint
fileName = sys.stdin

Values = [] #array of cost of items - int
Cal = [] #array of calories for each item - int
Items = [] #array of menu item names - string
mCal = [] #mincal you can get for any cost. Cost is index of array

def main():
        n = int(fileName.readline())            #read first line of file, sets n to total number of items
        W = int(fileName.readline())            #read second line, set W to total that must be spent
        someArray = [INF] * (W+1)

        lines = fileName.readlines()            #get list of remaining lines
        for line in lines:
                line = line.strip("\n").split(" ")      #split lines, assign to appropriate list
                Values.append(int(line[0]))
                Cal.append(int(line[1]))
                Items.append(line[2])
        mCal.append(0)                  
        for i in range(1, W + 1):           #set everything in mCal to INF
                mCal.append(INF)
        for i in range(1, W + 1):                #iterate through mCal
                for j in range(0, n):        #iterate through Values & Cal items
                        if Values[j] <= i:
                                if mCal[i] > mCal[i - Values[j]] + Cal[j]:
                                        mCal[i] = mCal[i - Values[j]] + Cal[j]
                                        someArray[i] = j                #records minCal location
        
        if mCal[W] == INF:      
                print "Not possible to spend exactly: {}".format(W)
                return None
        else:
                print "It's possible to spend exactly: {}".format(W)
                print "Minimum Calories: {}".format(mCal[W])

        Orders = [0] * n # will keep track of the number that will be ordered
        while (W):
                Orders[someArray[W]] += 1
                W = W - Values[someArray[W]]


        theOrder = {}
        for i in range(n):
                if Orders[i]:
                        theOrder[Items[i]] = Orders[i]
        for key in theOrder:
                print "{}: {}".format(key, theOrder[key])




main()

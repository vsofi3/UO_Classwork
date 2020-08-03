import sys
INF = sys.maxint
fileName = sys.stdin

def getData(fileName):
        lines = fileName.readlines()
        return lines

def fillLists(lines, Values, Cal, Items):
        global n, W
        n = int(lines.pop(0))           #read first line of file, sets n to total number of items
        W = int(lines.pop(0))           #read second line, set W to total that must be spent

        for line in lines:
                line = line.strip("\n").split(" ")      #split lines, assign to appropriate list
                Values.append(int(line[0]))
                Cal.append(int(line[1]))
                Items.append(line[2])         

def minCal(W, n, Values, Cal, someArray):
        mCal = [INF] * (W)
        mCal[0] = 0
        if not W:
                return False
        if (mCal[W-1] is None):
                for i in range(n):
                        if Values[i] <= W:
                                temp = minCal(W - Values[i], n, Values, Cal, someArray)
                                if temp + Cal[i] < mCal[W]:
                                        mCal[W] = temp + Cal[i]
                                        someArray[W] = i
        return mCal

                
def main():
        lines = getData(fileName)
        Values = [] #array of cost of items - int
        Cal = [] #array of calories for each item - int
        Items = [] #array of menu item names - string
        fillLists(lines, Values, Cal, Items)
        someArray = [INF] * (W+1)
        mCal = minCal(W, n, Values, Cal, someArray)
        print(mCal)

        

'''
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
'''
main()
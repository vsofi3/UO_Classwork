from OwenMcEvoy_MaxHeap import MaxHeap

class pQueue(object):
    def __init__(self,size) :
        # Build the Constructor
        self.__myHeap = MaxHeap(size)

    def insert(self, data):
        self.__myHeap.insert(data)
        
    def maximum(self):
        print self.__myHeap.maximum()
    
    def extractMax(self):
        print self.__myHeap.extractMax()

    def isEmpty(self):
        # Return true when the priority queue is empty
        if self.__myHeap.getLength() == 0:
            print("Empty")
        else:
            print("Not Empty")
    
    def printQueue(self):
        self.__myHeap.buildMax()            #turns heap into a max heap
        lastVal = self.__myHeap.getHeap()
        lastVal = lastVal[-1]
        rVal = "Current Queue: "
        for item in self.__myHeap.getHeap():
            if item == lastVal:
                rVal += str(item)
            else:
                rVal += str(item) + ","
        print(rVal)

'''
queue = pQueue(10)
queue.insert(5)
queue.insert(7)
queue.insert(4)
queue.insert(15)
queue.insert(15)
queue.insert(35)
queue.printQueue()
'''
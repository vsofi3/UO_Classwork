class MaxHeap(object):

    def __init__(self, size):
        # finish the constructor, given the following class data fields:
        self.__maxSize = size
        self.__length = 0
        self.__myArray = []     #i will be using the append feature, the list size is exactly how many items there are this way


    ''' free helper functions '''
    def getHeap(self):
        return self.__myArray

    def getMaxSize(self):
        return self.__maxSize
    
    def setMaxSize(self, ms):
        self.__maxSize = ms

    def getLength(self):
        return self.__length
    
    def setLength(self, l):
        self.__length = l
    
    ''' Required Methods '''
    def insert(self, data):
    	# Insert an element into your heap.
    	
    	# When adding a node to your heap, remember that for every node n, 
    	# the value in n is greater than or equal to the values of its children, 
    	# but your heap must also maintain the correct shape.
        size = self.getMaxSize()
        if self.__length == self.__maxSize:
            self.setMaxSize(size + 1)
        self.__myArray.append(data)
        self.__length += 1
        self.__heapify()


    
    def maximum(self):
        # return the maximum value in the heap
        maximum = self.__myArray[0]
        return maximum

    def extractMax(self):
        index = 0           #in a max heap, largest value is at the 0 index
        rMax = self.__myArray.pop(index)
        self.__length -= 1
        self.__heapify()
        return rMax


    def __heapify(self, node = None):
    # helper function for reshaping the array
        if node is None:
            node = self.__length // 2   #starting index for heapify process

        lChild = (2 * node) + 1         #location of the left child in the array
        rChild = (2 * node) + 2        #location of the right child in the array

        while rChild > (self.__length - 1):     #error checking technique, ensures that list index is not out of range
            node = node - 1                     #rChild > lChild, so that is what we must be checking
            lChild = (2 * node) + 1
            if lChild == (self.__length - 1):       #checks to see if left child is the last item in the heap
                swapVal = self.__myArray[lChild]     #this is only the case when the node has a left child but no right child
                if swapVal > self.__myArray[node]:      #so the index of the left child is the last one in the array
                    self.swap(lChild, node)
                nNode = node - 1
                if nNode >= 0:                      #we want to be able to access array at index[0], index[-1] would
                    self.__heapify(nNode)           #return last item in the array
                else:
                    pass
            rChild = (2 * node) + 2

        if self.__myArray[lChild] and self.__myArray[rChild]:   #case where there are two children
            if self.__myArray[lChild] > self.__myArray[rChild]:
                swapVal = lChild
            else:                                           #determines the larger of the 2 children to swap with
                swapVal = rChild
            if self.__myArray[swapVal] > self.__myArray[node]:              #if the child is greater than the parent, swap
                self.swap(swapVal, node)
            nNode = node - 1
            if nNode >= 0:                #makes sure node index is not 0
                self.__heapify(nNode)  #recursively calls heapify using decremented index
            else:
                pass

    ''' Optional Private Methods can go after this line
        # If you see yourself using the same ~4 lines of code more than once...
        # You probably want to turn them into a method.'''
    def swap(self, x, y):
        temp = self.__myArray[y]                    #to save a couple of lines of code in heapify i made this swap function
        self.__myArray[y] = self.__myArray[x]       #swaps values at indexes x&y in the array
        self.__myArray[x] = temp

    def buildMax(self):
        self.__heapify()                            #calling this will turn the unsorted array into a maxheap

'''
heap = MaxHeap(10)
heap.insert(5)
heap.insert(12)
heap.insert(64)
heap.insert(1)
heap.insert(37)
heap.insert(90)
heap.insert(91)
heap.insert(97)
print(heap.getHeap())
print(heap.maximum())
print(heap.extractMax())
print(heap.getHeap())
'''
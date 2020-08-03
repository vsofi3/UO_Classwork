class Node(object):
    # Node Class for Binary Search Tree.
    # A node has a parent, left child, and right child.
    # A node also has data.
    # data should only be touched by accessor methods defined here
    #    i.e., set* and get*

    def __init__(self, data):
        self.__data = data
        self.__parent = None
        self.__leftChild = None
        self.__rightChild = None

    def setData(self, data):
        self.__data = data

    def setLeftChild(self, lc):
        self.__leftChild = lc

    def setRightChild(self, rc):
        self.__rightChild = rc

    def setParent(self, p):
        self.__parent = p

    def getData(self):
        return self.__data
    
    def getLeftChild(self):
        return self.__leftChild
    
    def getRightChild(self):
        return self.__rightChild
    
    def getParent(self):
        return self.__parent

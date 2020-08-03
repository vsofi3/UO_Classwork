#Owen McEvoy    Red Black Tree - Lab 4 -    CIS 313 Winter 2019     - Last Modified: March 15th, 2019

# BinarySearchTree is a class for a binary search tree (BST)
# when called, a BST is initialized with no root and size 0.
# size keeps track of the number of nodes in the tree
from Node import RB_Node

class RedBlackTree:
    # initialize root and size
    def __init__(self):
        self.root = None
        self.size = 0
        self.sentinel = RB_Node(None,None, color = 'black')
        self.sentinel.parent = self.sentinel
        self.sentinel.leftChild = self.sentinel
        self.sentinel.rightChild = self.sentinel

    '''
    Free Methods
    '''

    def sentinel(self):     
        return self.sentinel

    def root(self):
        return self.root

    def __iter__(self):
        # in-order iterator for your tree
        return self.root.__iter__()

    def getKey(self, key):
        # expects a key
        # returns the key if the node is found, or else raises a KeyError

        if self.root:
            # use helper function _get to find the node with the key
            res = self._get(key, self.root)
            if res: # if res is found return the key
                return res.key
            else:
                raise KeyError('Error, key not found')
        else:
            raise KeyError('Error, tree has no root')

    
    def getNode(self, key):
        # expects a key
        # returns the RB_Node object for the given key
        
        if self.root:
            res = self._get(key, self.root)
            if res:
                return res
            else:
                raise KeyError('Error, key not found')
        else:
            raise KeyError('Error, tree has no root')

    # helper function _get receives a key and a node. Returns the node with
    # the given key
    def _get(self, key, currentNode):
        if currentNode is self.sentinel: # if currentNode does not exist return None
            print("couldnt find key: {}".format(key))
            return None
        elif currentNode.key == key:
            return currentNode
        elif key < currentNode.key:
            # recursively call _get with key and currentNode's leftChild
            return self._get( key, currentNode.leftChild )
        else: # key is greater than currentNode.key
            # recursively call _get with key and currentNode's rightChild
            return self._get( key, currentNode.rightChild )

    
    def __contains__(self, key):
        # overloads the 'in' operator, allowing commands like
        # if 22 in rb_tree:
        # ... print('22 found')

        if self._get(key, self.root):
            return True
        else:
            return False

    def delete(self, key):
        # Same as binary tree delete, except we call rb_delete fixup at the end.

        z = self.getNode(key)
        if z.leftChild is self.sentinel or z.rightChild is self.sentinel:
            y = z
        else:
            y = z.findSuccessor()

        if y.leftChild is not self.sentinel:
            x = y.leftChild
        else:
            x = y.rightChild

        if x is not self.sentinel:
            x.parent = y.parent

        if y.parent is self.sentinel:
            self.root = x

        else:
            if y == y.parent.leftChild:
                y.parent.leftChild = x
            else:
                y.parent.rightChild = x

        if y is not z:
            z.key = y.key

        if y.color == 'black':
            if x is self.sentinel:
                self._rb_Delete_Fixup(y)
            else:
                self._rb_Delete_Fixup(x)

    def traverse(self, order, top = -1):
        # Same a BST traverse
        if top is -1:
            top = self.root
            last_call = True
        
        last_call = False

        if top is not self.sentinel :
            if order == "in-order":
                self.traverse(order, top = top.leftChild)
                print(top.key),
                self.traverse(order, top = top.rightChild)

            if order == "pre-order":
                print(top.key),
                self.traverse(order, top = top.leftChild)
                self.traverse(order, top = top.rightChild)

            if order == "post-order":
                self.traverse(order, top = top.leftChild)
                self.traverse(order, top = top.rightChild)
                print(top.key),

        if last_call:
            print

    '''
    Required Methods Begin Here
    '''

    def insert(self, key):
        #add a key to the tree. Don't forget to fix up the tree and balance the nodes.
        #is there a root? if not, what do you have to do?
        #if there is, how does that change things?
        if self.root is None:               #checks to see if the root is none, if not the new node will become the root
            node = RB_Node(key)
            self.root = node
            node.parent = self.sentinel        #sets parents and children to sentinel nodes
            node.leftChild = self.sentinel
            node.rightChild = self.sentinel
            node.color = "black"                #root node is always black
        else:
            node = RB_Node(key)
            y = self.sentinel
            x = self.root
            while x != self.sentinel:           #x is the root, will go down the tree until x reaches a sentinel node
                y = x                           #keeps track of the parent node
                if node.key < x.key:            #determining which path x will take
                    x = x.leftChild
                else:
                    x = x.rightChild
            node.parent = y                     #sets the parent to y
            if y == self.sentinel:              #if y was a sentinel, the node must become the root since only
                self.root = node                #the root has a sentinel parent
            elif node.key < y.key:
                y.leftChild = node              #determining if it's a left or right child
            else:
                y.rightChild = node
            node.leftChild = self.sentinel          #sets the children to sentinels
            node.rightChild = self.sentinel
            node.color = "red"                      #newly inserted nodes are red
            self._rbInsertFixup(node)

    def _rbInsertFixup(self, z):
        #write a function to balance your tree after inserting. Function is based off
        #pseudocode in the book and information from geeksforgeeks
        while z.parent.color == "red":
            if z.parent == z.parent.parent.leftChild:
                y = z.parent.parent.rightChild
                if y.color == "red":                #case 1: z is red and uncle is red
                    z.parent.color = "black"        #color of parent and uncle become black
                    y.color = "black"
                    z.parent.parent.color = "red"   #grandparent becomes red
                    z = z.parent.parent             #sets z as grandparent
                else:
                    if z.isRightChild():    #case 2: z is red, z is a right child, and z is black
                        z = z.parent
                        self.leftRotate(z)

                    z.parent.color = "black"     #set z's parent color to black
                    z.parent.parent.color = "red"   #grandparents color changes to red
                    self.rightRotate(z.parent.parent)   #rotate right using grandparent
            else:
                if z.parent == z.parent.parent.rightChild:
                    y = z.parent.parent.leftChild
                    if y.color == "red":                        #checks if the uncle, y, is red
                        z.parent.color = "black"
                        y.color = "black"                   #color of parent/uncle becomes black
                        z.parent.parent.color = "red"       #grandparent becomes red
                        z = z.parent.parent
                    else:
                        if z.isLeftChild():             #case 3: z is red, a left child, and the uncle is black
                            z = z.parent                #set z to its parent
                            self.rightRotate(z)         #rotate right using z

                        z.parent.color = "black"            #set parent color to black
                        z.parent.parent.color = "red"       #set grandparent color to red
                        self.leftRotate(z.parent.parent)    #rotate left using grandparent
        self.root.color = "black"           #the root must always be black

    def _rb_Delete_Fixup(self, x):
        # receives a node, x, and fixes up the tree, balancing from x. used pseudocode from the book
        while x != self.root and x.color == "black":        #balances tree until x is root or black
            if x.isLeftChild():     #checks if x is a left child //
                w = x.parent.rightChild     #sets w to x's sibling
                if w.color == "red":        #case 1 w & x are red (w&x are siblings)
                    w.color = "black"   #sets w to be black
                    x.parent.color = "red"  #makes parent red
                    self.leftRotate(x.parent)   #rotates left using the parent
                    w = x.parent.rightChild
                if w.leftChild.color == "black" and w.rightChild.color == "black": #case 2: w is black and both children are black
                    w.color = "red"   #sets parent color to red
                    x = x.parent       # sets x = to its parent
                elif w.rightChild.color == "black":         #case 3: w is black, w's left child is red, and w's right child is black
                    w.leftChild.color = "black"             #sets both children to black
                    w.color = "red"                         #w turns red
                    self.rightRotate(w)        #rotates right using w
                    w = x.parent.rightChild
                else:                                       #case 4 w is black and its right child is red
                    w.color = x.parent.color
                    x.parent.color = "black"
                    w.rightChild.color = "black"
                    self.leftRotate(x.parent)
                    x = self.root
            else:                                   #the code below is the same except that
                w = x.parent.leftChild              #left and right have been swapped
                if w.color == "red":
                    w.color = "black"
                    x.parent.color = "red"
                    self.rightRotate(x.parent)
                    w = x.parent.leftChild
                if w.rightChild.color == "black" and w.rightChild.color == "black":
                    w.color = "red"
                    x = x.parent
                elif w.leftChild.color == "black":
                    w.rightChild.color = "black"
                    w.color = "red"
                    self.leftRotate(w)
                    w = x.parent.leftChild
                else:
                    w.color = x.parent.color
                    x.parent.color = "black"
                    w.leftChild.color = "black"
                    self.rightRotate(x.parent)
                    x = self.root
        x.color = "black"




    def leftRotate(self, currentNode):
        # perform a left rotation from a given node. used pseudocode from book
        y = currentNode.rightChild  #set y
        currentNode.rightChild = y.leftChild    #turn y's left subtree into current trees's right subtree
        if y.leftChild != self.sentinel:
            y.leftChild.parent = currentNode
        y.parent = currentNode.parent
        if currentNode.parent == self.sentinel:         #only a root would have a sentinel parent
            self.root = y
        elif currentNode == currentNode.parent.leftChild:
            currentNode.parent.leftChild = y
        else:
            currentNode.parent.rightChild = y
        y.leftChild = currentNode     #put x on y's left
        currentNode.parent = y        #set x's parent as y

    def rightRotate(self, currentNode):
        # perform a right rotation from a given node. used pseudocode from the book
        x = currentNode.leftChild       #set x
        currentNode.leftChild = x.rightChild    #turn x's right subtree to
        if x.rightChild != self.sentinel:
            x.rightChild.parent = currentNode
        x.parent = currentNode.parent
        if currentNode.parent == self.sentinel:
            self.root = x                   #only a root can have a sentinel parent
        elif currentNode == currentNode.parent.rightChild:
            currentNode.parent.rightChild = x
        else:
            currentNode.parent.leftChild = x
        x.rightChild = currentNode                  #sets up child/parent relationship
        currentNode.parent = x


    '''
    Optional handy methods that you can imagine can start here
    '''
    def search(self, key):
    #simple search function. uses the __contains__ method
    #only need to return the key and print found if so, if not print not found
        if self.__contains__(key):      #checks to see if the key is in the tree
            print("{} Found".format(key))
            return key
        else:
            return 0                #if it is not found, contains will print couldn't find key
                                    # so i chose to not print it in my function




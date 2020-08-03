#In this project I have created a binary search tree with the methods: find, insert, and delete

from Node import Node

class BST(object):
    def __init__(self):
        self.__root = None

    def getRoot(self):
        # Private Method, can only be used inside of BST.
        return self.__root

    def __findNode(self, data, aNode = None):
        # Private Method, can only be used inside of BST.
        # Search tree for a node whose data field is equal to data.
        # Return the Node object
            if not self.__root:
                return None

            if aNode is None:
                aNode = self.__root

            if aNode.getData() == data:
                return aNode

            elif aNode.getData() > data:
                if not aNode.getLeftChild():        #ensures that there is a left child to go to
                    return None
                nNode = aNode.getLeftChild()
                return self.__findNode(data, nNode)

            elif aNode.getData() < data:
                if not aNode.getRightChild():       #ensures there is a right child to go to
                    return None
                nNode = aNode.getRightChild()
                return self.__findNode(data, nNode)

            else:
                return None


    def exists(self, data):
        # return True of node containing data is present in the tree.
        # otherwise, return False.
        if self.__findNode(data, aNode = None):
            return True
        else:
            return False

    def insert(self, data, node = None):
        # Find the right spot in the tree for the new node
        # Make sure to check if anything is in the tree
        # Hint: if a node n is null, calling n.getData() will cause an error
        if self.__root is None:
            nNode = Node(data)
            self.__root = nNode
            return
        if node is None:
            node = self.__root

        if node.getData() > data:
            if node.getLeftChild():             #checks if there is a left child, if so, go to that left child
                nNode = node.getLeftChild()
                self.insert(data, nNode)
            else:
                aNode = Node(data)              #if no left child, create a node, store the data, and set the node as a left child
                node.setLeftChild(aNode)
                aNode.setParent(node)

        else:
            if node.getRightChild():           #checks if there is a right child, if so, go to that right child
                nNode = node.getRightChild()
                self.insert(data, nNode)
            else:
                aNode = Node(data)              #if no right child, create node, store data, set node as right child
                node.setRightChild(aNode)
                aNode.setParent(node)

    
    def delete(self, data, aNode = None):
        # If the value specified by delete does not exist in the tree, then don't change the tree.
        # If you find the node and ...
        #  a) The node has no children, just set it's parent's pointer to Null.
        #  b) The node has one child, make the nodes parent point to its child.
        #  c) The node has two children, replace it with its successor, and remove 
        #       successor from its previous location.

        node = self.__findNode(data, aNode)                         #finds the node to delete
        if node:
            if not (node.getLeftChild() and node.getRightChild()):      #case a, node has no children, want to set parents pointer to null
                if node.getParent():
                    parent = node.getParent()                                  #finds parent node
                    if parent.getData() > node.getData():                      #determines if node is a R/L child
                        parent.setLeftChild(None)
                    else:
                        parent.setRightChild(None)
                else:
                    self.__root = None                  #if the node we want to delete is the root, makes sure to set root to none

            if bool(node.getLeftChild()) + bool(node.getRightChild()) == 1: #case b, node has 1 child
                if node.getParent():
                    parent = node.getParent()
                    if node.getLeftChild():                             #finds the child which will replace the node
                        nNode = node.getLeftChild()
                    else:
                        nNode = node.getRightChild()

                    if parent.getData() > node.getData():               #determines if the node is a R/L child
                        parent.setLeftChild(nNode)
                        nNode.setParent(parent)                         #the node's child replaces the node
                    else:
                        parent.setRightChild(nNode)
                        nNode.setParent(parent)
                else:
                    if node.getLeftChild():
                        lC = node.getLeftChild()
                        node.setData(lC.getData())                  #sets data to data of left child
                        self.delete(lC.getData(), lC)
                    else:
                        rC = node.getRightChild()
                        node.setData(rC.getData())                  #sets data to that of right child
                        self.delete(rC.getData(), rC)

            if node.getLeftChild() and node.getRightChild():        #case c, node has 2 children
                if node.getParent():
                    parent = node.getParent()
                    successor = self.__findSuccessor(node)
                    if parent.getData() > node.getData():
                        node.setData(successor.getData())          #determines if node is a left/right child, then sets
                                                                   #node data to that of its child
                    else:
                        node.setData(successor.getData())

                    self.delete(successor.getData(), successor)    #delete the node you used to replace the original node with
                else:
                    successor = self.__findSuccessor(node)
                    node.setData(successor.getData())

                    self.delete(successor.getData(), successor)

        return None

    def __findSuccessor(self, aNode):

        if aNode.getRightChild():                           #case where the node has a R child
            successor = aNode.getRightChild()               #set successor value to anode's R child
            while successor.getLeftChild():
                successor = successor.getLeftChild()        #stage where you "go left until end"
            return successor

        if not aNode.getRightChild():                       #case where the node does not have a R child
                                                            #2 cases, node is L or R child
            successor = aNode.getParent()
            if successor.getLeftChild() == aNode:           #case 1, aNode is the L child of it's parent
                return successor                            #case 1 implies the successor is the parent

            while successor.getParent():                    #case 2, aNode is the right child of it's parent.
                successor = successor.getParent()
                if successor.getData() > aNode.getData():   #due to the ordering of a bst, the first "right turn"
                                                            #will be when the parent is greater than the node
                    return successor


    def traverse(self, order, top):
        # traverse the tree by printing out the node data for all node in a specified order
        if top is not None:
            if order == "preorder":
                print(top.getData()),
                self.traverse(order, top.getLeftChild())
                self.traverse(order, top.getRightChild())
                
            
            elif order == "inorder":
                # your code here, remove pass
                self.traverse(order, top.getLeftChild())
                print(top.getData()),
                self.traverse(order, top.getRightChild())

            elif order == "postorder":
                # your code here, remove pass
                self.traverse(order, top.getLeftChild())
                self.traverse(order, top.getRightChild())
                print(top.getData()),
            
            else:
                print("Error, order {} undefined".format(order))



'''
tree = BST()
tree.insert(12)
tree.insert(17)
tree.insert(10)
tree.insert(5)
tree.insert(11)
tree.insert(13)
tree.insert(20)
tree.insert(15)
tree.insert(18)
tree.insert(23)
tree.insert(3)
tree.insert(8)
tree.insert(1)
tree.insert(4)
tree.insert(7)
tree.insert(9)
tree.traverse('postorder', tree.getRoot())
tree.delete(12)
print("")
tree.traverse('postorder', tree.getRoot())
'''
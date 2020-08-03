#Owen McEvoy CIS 313 Lab1                                               Last Modified: 2/21/2019
from sys import argv

class Node(object):
    def __init__(self, data = None, next = None):
        self.__data = data
        self.__next = next
            
    def setData(self, data):
        # Set the "data" data field to the corresponding input
        self.__data = data

    
    def setNext(self, next):
        # Set the "next" data field to the corresponding input
        self.__next = next 

    def getData(self):
        # Return the "data" data
        return self.__data
            
    def getNext(self):
        # return the "next" data field
        return self.__next

class Queue(object):
    def __init__(self):
        self.__head = None
        self.__tail = None
        self.__size = 0 #keeps track of number of elements in queue
    
    def enqueue(self, newData):
        # Create a new node whose data is newData and whose next node is null
        # Update head and tail
        # Hint: Think about what's different for the first node added to the Queue
        newNode = Node(newData, None)
        if self.isEmpty():
            self.__head = newNode
            self.__tail = newNode
            self.__size = self.__size + 1
        else:
            self.__tail.setNext(newNode) #creates the next node
            self.__tail = newNode
            self.__size = self.__size + 1

    def dequeue(self):
        #  Return the head of the Queue
        #  Update head
        #  Hint: The order you implement the above 2 tasks matters, so use a temporary node
        #          to hold important information
        #  Hint: Return null on a empty Queue
        if self.isEmpty():
            return None
        
        data = self.__head
        self.__head = data.getNext()      #gets data and moves the head to next in line

        self.__size = self.__size -1 
        return data.getData()
    
    def isEmpty(self):
        # Check if the Queue is empty
        if self.__size == 0:
            return True
        else:
        	return False
    
    def printQueue(self):
        # Loop through your queue and print each Node's data
        data = self.__head
        while data != None:
            print(data.getData())
            data = data.getNext()

class Stack(object):
    def __init__(self):
        # We want to initialize our Stack to be empty
        # (ie) Set top as null
        self.__top = None
        self.__size = 0             #keeps track of the number of elements of stack
    
    def push(self, newData):
        # We want to create a node whose data is newData and next node is top
        # Push this new node onto the stack
        # Update top
        newNode = Node(newData)
        newNode.setNext(self.__top)         #sets up next node
        self.__top = newNode
        self.__size = self.__size + 1

    def pop(self):
        # Return the Node that currently represents the top of the stack
        # Update top
        # Hint: The order you implement the above 2 tasks matters, so use a temporary node
        #         to hold important information
        # Hint: Return null on a empty stack
        if self.isEmpty():
            return None
        else:
            data = self.__top
            self.__top = data.getNext()            #changes top to the next node on the stack
            self.__size = self.__size - 1
            return data.getData()
        
    def isEmpty(self):
        # Check if the Stack is empty
        if self.__size == 0:
            return True
        else:
            return False
        
    def printStack(self):
        # Loop through stack and print each Node's data
        data = self.__top
        while data != None:
            print(data.getData())
            data = data.getNext()

def main(argv):
    # reads the file line by line and checks if each line is a palindrome
    input_file = argv[1]
    with open(input_file, 'r') as file_ob:
        file_ob.next()  #this skips the first line of the file, which tells us the number of lines
        for line in file_ob:        #loop through the file
            if isPalindrome(line):
                print("This is a Palindrome")
            else:
                print("Not a Palindrome")

def isPalindrome(s):
    '''
        The purpose of this function is to determine if a line from a file is a palindrome or not. 
        The input for this function in this case is the line of a file, although it should work for any string of characters 
                    EX: isPalindrome(12321) 
                    Return: True
    '''
    s = s.strip()               #strips whitespace
    s = s.lower()               #ensures that every character is lowercase 
    myStack = Stack() 
    myQueue = Queue()
    
    for i in range(len(s)):        #put every character onto the stack/into the queue
        myStack.push(s[i])
        myQueue.enqueue(s[i])

    a1 = ""
    a2 = ""
    for i in range(len(s) // 2):        #stack is last in first out, queue is first in first out
        a1 = a1 + myStack.pop()         #so you only have to pop/dequeue half of the elements
        a2 = a2 + myQueue.dequeue()     #to determine if s is a palindrome

    if a1 == a2:

        return True
    else:

        return False
        
def isPalindromeEC(s):
    # Implement if you wish to do the extra credit.
    return

if __name__ == "__main__":
    main(argv)




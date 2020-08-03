from sys import argv
from RBT import RedBlackTree


def main(argv):
    fileName = argv[1]
    T = RedBlackTree()
    with open(fileName, 'r') as fob:
        for line in fob:            #reads file line by line
            l = line.strip().split()    #splits spaces and strips whitespace
            if len(l) == 2:
                command = l[0]
                data = int(l[1].strip())
                if command == 'insert':
                    T.insert(data)

                if command == 'delete':
                    T.delete(data)

                if command == 'search':         #search function i've implemented into RBT.py
                    T.search(data)

            if len(l) == 1:
                print(l[0])
                T.traverse(l[0])
                print('')
    fob.close()             #close the file

if __name__ == "__main__":
    main(argv)
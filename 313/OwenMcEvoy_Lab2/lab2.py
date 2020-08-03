#Owen McEvoy CIS 313


from sys import argv
from BST import BST




def main(argv):
    # Loop over input file (filename passed via argv).
    # Split each line into a task and number (if one exists) 
    #   hint: use the split method for strings https://docs.python.org/2/library/string.html#string.split
    # Perform the function corresponding to the specified task
    # i.e., insert, delete, inorder, preorder, postorder
    # Close the file when you're done.
    file = argv[1]
    tasklist = []
    tree = BST()
    with open(file, 'r' ) as file_ob:
        for line in file_ob:
            tasks = line.strip()            #strips whitespace
            calls = tasks.split(" ")    #separates commands (some require 2 arguments)
            tasklist.append(calls)

        for task in tasklist:
                     #these types of commands are the traversals inorder, preorder, postorder
            if task[0] == "inorder":
                tree.traverse("inorder", tree.getRoot())
                print("")  # just to make my output a little more readable
            elif task[0] == "preorder":
                tree.traverse("preorder", tree.getRoot())
                print("")  # just to make my output a little more readable
            elif task[0] == "postorder":
                tree.traverse("postorder", tree.getRoot())
                print("")  # just to make my output a little more readable
            elif len(task) == 2:
                command = task[0]      #here are the insert/delete commands
                value = int(task[1])
                if command == "insert":
                    tree.insert(value)
                elif command == "delete":
                    tree.delete(value)
                else:
                    print("this is not an appropriate BST method")


if __name__ == "__main__":
    main(argv)


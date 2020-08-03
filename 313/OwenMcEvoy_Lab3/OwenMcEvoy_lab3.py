from sys import argv
from OwenMcEvoy_pQueue import pQueue

def main(argv):
    # Loop over input file. 
    # Split each line into the task and the corresponding number (if one exists)
    # Depending on what the input task was, preform the corresponding function
    # Finally, close your file.

    inputFile = argv[1]
    tasklist = []
    queue = pQueue(0)
    with open(inputFile, 'r') as file_ob:
        for line in file_ob:
            tasks = line.strip()  # strips whitespace
            calls = tasks.split(" ")  # separates commands (some require 2 arguments)
            tasklist.append(calls)
        for task in tasklist:
            if task[0] == "extractMax":
                queue.extractMax()
            elif task[0] == "maximum":
                queue.maximum()
            elif task[0] == "isEmpty":
                queue.isEmpty()
            elif task[0] == "print":
                queue.printQueue()
            elif task[0] == "insert":
                queue.insert(int(task[1]))
            else:
                print("this is an invalid command")


if __name__ == "__main__":
    main(argv)
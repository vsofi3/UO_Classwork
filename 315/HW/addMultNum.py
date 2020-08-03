import sys
fileName = sys.stdin
def main():
    fileName.readline()
    for line in fileName:            #reads file line by line
        l = line.strip().split()    #splits spaces and strips whitespace
        sum  = int(l[0]) + int(l[1])
        mult = int(l[0]) * int(l[1])
        print("{} {}".format(sum, mult))
main()
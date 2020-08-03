#NAME: OWEN MCEVOY
#LAST MODIFIED: January 14th

from sys import argv

def main(argv):
    file_name = argv[1]
    
    with open(file_name, 'r') as file_object:
      numProblems = file_object.readline()

      for line in file_object:
        a, b = line.strip().split(' ')
        the_gcd = gcd(a, b)
        the_lcm = lcm(a, b)
        print("{} {}".format(the_gcd, the_lcm))

def gcd(a, b):
    '''
       Inputs (a,b) - 2 strings that are obtained by reading the input file
       Description - converts strings to integer and then uses Euclid algo. to determine the GCD
       Returns: an integer (the gcd of a & b)
    '''
  # Find the greatest common divisor of a and b
  # Hint: Use Euclid's Algorithm
  # https://en.wikipedia.org/wiki/Euclidean_algorithm#Procedure
    a = int(a)
    b = int(b)
    if a > b:
        larger = a
        smaller = b
    elif b > a:
        larger = b
        smaller = a
    else:
        return a

    while smaller != 0:
        temp = smaller
        smaller = larger % smaller
        larger = temp
    return larger
        


def lcm(a, b):
    '''
       Inputs (a,b) - 2 strings that are obtained by reading the input file
       Description- converts strings to integer and then determines the LCM by using the GCD function
       Returns: an integer (the lcm of a & b)
    '''
  # Find the least common multiple of a and b
  # Hint: Use the gcd of a and b
    a = int(a)
    b = int(b)
    if a % b == 0:
        lcm = a
    elif b % a == 0:
        lcm = b
    else:
        lcm = (a * b) / gcd(a, b)
    return lcm

if __name__ == "__main__":
  main(argv)

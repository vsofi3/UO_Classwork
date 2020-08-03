# Run with /Applications/VisIt.app/Contents/Resources/bin/visit -cli -s originScript.py 
import sys

def setUp():
    RestoreSession("flat.session", 0)

def animateInTime():
    i = 2400
    while(i < 3239):
        TimeSliderSetState(i)
        i += 12

def main():
    setUp()
    animateInTime()

main()
sys.exit()

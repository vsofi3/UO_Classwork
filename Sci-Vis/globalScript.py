# Run with /Applications/VisIt.app/Contents/Resources/bin/visit -cli -s globalScript.py 
import math
import threading
import sys

ma1 = -0.04412552310988097
ma2 = 0.3013169242725946
ma3 = -0.9498057735707826

def set_up():
    RestoreSession("global.session", 0)

def moveCamera():
    global ma1
    global ma2
    global ma3
    v = GetView3D()    
    v.viewNormal = (ma1, ma2, ma3)
    v.focus = (0, 0, 0)
    #v.viewUp = (-0.08888412682497127, 0.9522521207028126, 0.3042970240556992)
    v.viewUp = (0, 1, 0)
    v.viewAngle = 30
    v.parallelScale = 12762.1
    v.nearPlane = -25524.1
    v.farPlane = 25524.1
    v.imagePan = (0, 0)
    v.imageZoom = 1
    v.perspective = 1
    v.eyeAngle = 2
    v.centerOfRotationSet = 0
    v.centerOfRotation = (0, 0, 0)
    v.axis3DScaleFlag = 0
    v.axis3DScales = (1, 1, 1)
    v.shear = (0, 0, 1)
    v.windowValid = 1
    for i in range(360):
        angle = math.pi * 2 * i/359.0
        ma1 = math.cos(angle)
        ma3 = math.sin(angle)
        v.viewNormal = (ma1, ma2, ma3)
        SetView3D(v)

def animateInTime():
    i = 2400 
    while(i < 3239):
        TimeSliderSetState(i)
        i += 12
        if i % 15 == 0:
            moveCamera()
def main():
    set_up()
    t1 = threading.Thread(target = animateInTime)
    t1.start()
     
main()
sys.exit()

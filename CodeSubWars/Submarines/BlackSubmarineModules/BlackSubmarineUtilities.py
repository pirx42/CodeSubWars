# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#this is utility stuff for black submarine

import math


#calculate an approximate hit position if an object would attacked at the given current position.
#returns a tuple: 
#   first:    true if hit position could be determined otherwise false
#   second:   the approximated hit position
def calculateHitPosition(submarine, currentGlobalTargetPosition):
    targetInMap = submarine.getControlCenter().getMap().findNearestElement(currentGlobalTargetPosition, 20, 0xffffffff, 0)
    if not targetInMap.isValid():
        return (0, currentGlobalTargetPosition)

    #solve the problem on numerical way using iterative method. just calculate 5 iterations.
    times = determineTotalTime(submarine, currentGlobalTargetPosition)
    approximatedGlobalHitPosition = currentGlobalTargetPosition + targetInMap.vecWorldTVelocity*(times[0] + times[1])

    times = determineTotalTime(submarine, approximatedGlobalHitPosition)
    approximatedGlobalHitPosition = currentGlobalTargetPosition + targetInMap.vecWorldTVelocity*(times[0] + times[1])

    times = determineTotalTime(submarine, approximatedGlobalHitPosition)
    approximatedGlobalHitPosition = currentGlobalTargetPosition + targetInMap.vecWorldTVelocity*(times[0] + times[1])

    times4 = determineTotalTime(submarine, approximatedGlobalHitPosition)
    approximatedGlobalHitPosition = currentGlobalTargetPosition + targetInMap.vecWorldTVelocity*(times4[0] + times4[1])

    times5 = determineTotalTime(submarine, approximatedGlobalHitPosition)
    approximatedGlobalHitPosition = currentGlobalTargetPosition + targetInMap.vecWorldTVelocity*(times5[0] + times5[1])

    #the determination failed if the iteration is flip floping or the torpedo moving time is longer than its live time or
    #the torpedo will not be armed within the moving time
    return (abs(times4[1] - times5[1]) < 2 and times5[0] < 65 and times5[0] > 5, 
            approximatedGlobalHitPosition)


#determines the time within a torpedo could reach the given global target position.
#returns a tuple: 
#   first:    the moving time of the torpedo
#   second:   the rotation time of the submarine
def determineTotalTime(submarine, globalTargetPosition):
    vecToCurrentTarget = globalTargetPosition - submarine.getControlCenter().getGPS().getPosition()
    lengthToCurrentTarget = vecToCurrentTarget.getLength()

    #simplification: assume that the torpedos is uniform motion (t = s/v)
    velocityTorpedo = 35.6 #meter per second (measured)
    timeTorpedo = lengthToCurrentTarget/velocityTorpedo

    #calculate the time to rotate by given angle
    angleToRotate = submarine.getControlCenter().getGyroCompass().getForwardDirection().getAngleBetween(vecToCurrentTarget)
    timeRotation = determineRotationTime(angleToRotate)
          
    return (timeTorpedo, timeRotation)


#determine the time to rotate by given angle
def determineRotationTime(angleToRotate):
    #simplification: assume the rotation consists of:
    #                1. accelerate to maximum rotation velocity: uniform accelerated motion (t = sqrt(2*s/a)
    #                2. rotate at maximum rotation velocity: uniform motion (t = s/v)
    #                3. brake to zero rotation velocity: uniform accelerated motion
    
    velocityRotation = 0.7 #degree per second (measured)
    
    timeToFullSpeed = 4 #seconds (measured)
    accGoOn = velocityRotation/timeToFullSpeed #degree per second^2
    angleToFullSpeed = accGoOn * timeToFullSpeed*timeToFullSpeed / 2   #degree
    
    timeToStop = 12 #seconds (measured)
    accBrake = velocityRotation/timeToStop #degree per second^2
    angleToStop = accBrake * timeToStop*timeToStop / 2  #degree
    
    timeRotation = 0
    if angleToRotate >= angleToFullSpeed + angleToStop:
        timeRotation = timeToFullSpeed + (angleToRotate - angleToFullSpeed - angleToStop)/velocityRotation + timeToStop
    else:
        angleAccelerated = angleToRotate*accBrake/(accBrake + accGoOn)
        timeWentOn = math.sqrt(2*angleAccelerated/accGoOn)
        timeBraked = math.sqrt(2*(angleToRotate - angleAccelerated)/accBrake)
        timeRotation = timeWentOn + timeBraked

    return timeRotation

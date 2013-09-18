LeapRTC
=======

This is a program for LeapMotion RT-component.

## What is RT-component? 
See http://openrtm.org

## What is LeapMotion?
See http://www.leapmotion.com

Use this program with LeapRTCTest which is distributed in below:

https://github.com/sugarsweetrobotic/LeapTest_py.git


# Author
Yuki Suga (Sugar Sweet Robotics CO. LTD.)

E-mail: ysuga@sugarsweetrobotics.com

URL: http://sugarsweetrobotics.com

# License
* GPL v3.0

If you need another license, please contact author.


# Requirement 
* OpenRTM-aist 1.1 C++ version
* LeapSDK 1.0.8

# HOW TO USE

Set Environmental Variable LEAP_SDK_PATH to your LeapDevelopmentKit 
 as you can find Leap.h by ( $LEAP_SDK_PATH/include/Leap.h )
Then, cmake and make.

    $ cd $path_to_leaprtc
    $ mkdir build
    $ cd build
    $ cmake ../
    $ make 

Then you can find your LeapRTC in $path_to_leaprtc/build/src directory.
You must put libleap.dylib in the same directory of your RTC. 
If you use LeapRTCComp, you must put the libleap.dylib with it.
If you use LeapRTC.dylib as a loadable module for rtcd, you must put the libleap.dylib with the LeapRTC.dylib.

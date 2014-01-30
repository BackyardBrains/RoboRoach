RoboRoach
=========

![The RoboRoach](http://backyardbrains.com/img/photos/roboRoach.jpg)

The RoboRoach is the world's first commercially available cyborg! Created by Backyard Brains.  This repository contains the software, hardware, and firmware for the complete cyborg system. The hardware currently relies on the CC2540 and Blue Tooth Low Energy, so Apple MFI contracts to not apply.  To program, you will need a CC Debugger by TI <http://www.ti.com/tool/cc-debugger>.  The PCB is currently configured to use a TAG Connect port <http://www.tag-connect.com/catalog>. 

Overview
--------
Have you ever wanted to walk down the hall of your school or department with your own remote controlled cockroach? We are now excited to announce the world's first commercially available cyborg! With our RoboRoach you can briefly wirelessly control the left/right movement of a cockroach by microstimulation of the antenna nerves. The RoboRoach is a great way to learn about neurotechnology, learning, and electronics!

Hardware
--------
The PCB version 1.1 of the PCB Backpack contains 2 high-current digital outs for the two antennea.  We placed the backpack in it's own directory in case we decide to make a PCB for a bluetooth remote control that does not require a mobile device.

Firmware
--------
We tried 2 versions.  The first was using the BlueRadio AT.e firmware.  This allowed for UART debugging, but unforunately the API was too buggy.  We ended up swiching back to the TI BLE Stack, which allowed for lower level control of the timers and interrupts.  

Software
--------
Our release supports iOS and Android devices.  Native support is only for Apple mobile devices released after October 2011 (iPhone 4s and later) and Android devices running Android 4.3 or later (October 2013). 

![iOS Interface](http://backyardbrains.com/products/img/roboRoach.iOS.jpg)

We do not support Windows Phone at the moment. We also need to add support for native iPad interface, and add an OSX app interface too (the BLE code compiles on MacOSX, but we have no interface). 
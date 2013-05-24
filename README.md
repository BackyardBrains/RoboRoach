RoboRoach
=========

![The RoboRoach](http://backyardbrains.com/img/photos/roboRoach.jpg)

The RoboRoach is the world's first commercially available cyborg! Created by Backyard Brains.  This repository contains the software, hardware, and firmware for the complete cyborg system. The hardware currently relies on the CC2540 and Blue Tooth Low Energy, so Apple MFI contracts to not apply.  To program, you will need a CC Debugger by TI <http://www.ti.com/tool/cc-debugger>.  The PCB is currently configured to use a TAG Connect port <http://www.tag-connect.com/catalog>. 

Overview
--------
Have you ever wanted to walk down the hall of your school or department with your own remote controlled cockroach? We are now excited to announce the world's first commercially available cyborg! With our RoboRoach you can briefly wirelessly control the left/right movement of a cockroach by microstimulation of the antenna nerves. The RoboRoach is a great way to learn about neurotechnology, learning, and electronics!

PCB
---
(Coming soon: Eagle CAD Layout + Libraries for RoboRoach hardware version 1.0g)

Firmware
--------
We tried 2 versions.  The first was using the BlueRadio AT.e firmware.  This allowed for UART debugging, but unforunately the API was too buggy.  We ended up swiching back to the TI BLE Stack.  

Software
--------
Our current release only supports iOS devices.  Native support only for Apple mobile devices released after October  2011 (iPhone 4s and later).  Due to the lack of BLE support on Android at the moment, we will only support the Motorola Razor (coming soon). We also need to add support for native iPad interface.
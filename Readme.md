The purpose of Bluemaster is to verify the security of Bluetooth-based Proximity
Authentication. Since Bluetooth-based Proximity Authentication is based on a bluetooth
connection, you cannot guarantee security unless you use the features of bluetooth well.
The properties of Bluetooth can be verified using bluez's tools, the Bluetooth Stack used
in Linux. In other words, the security of Bluetooth-based Proximity Authentication can
also be verified using the tools provided by bluez.
This tool provides guidelines for using bluez for the purpose of verifying the security of
Bluetooth-based Proximity Authentication.
Proceed with the flow according to the description provided by this tool, you can verify
the security of Bluetooth-based Proximity Authentication.

## 1. Prerequisite
This tool should use a platform using bluez as a Bluetooth stack. However, bluez installed
on the platform does not include the tool, so additional installation is required.
Bluez can be downloaded from the official website.

[Bluez](http://www.bluez.org/download/)
 

To install bluez, install the following packages,

> $ sudo apt install libglib2.0-dev libdbus-1-dev libudev-dev libical-dev libreadline-dev 

and install it with the following command.

> $ ./configure
> 
> $ ./make


## 2. install
If bluez is installed properly, bluez has a "tools" directory.
You can run install.sh with that directory path as argument.

> $ ./install.sh [the path of "tools" directory]

This will create a new "out" directory and run "bluemaster" on it.

#!/bin/bash

sudo bdaddr -i hci0 $1

hciconfig hci0 class 0x0c010c

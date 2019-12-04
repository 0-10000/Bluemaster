#!/bin/bash

while true
    do
        sleep 0.1
        sdptool browse --tree --l2cap $1
    done



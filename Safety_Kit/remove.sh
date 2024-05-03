#!/bin/bash
if [ "$1" == "-z" ]; 
then
    rm -f .zone_config.json
    echo "Zone makers removed"
fi

if [ "$1" == "-s" ]; 
then
    rm -f .camera_status_config.json
    echo "Status makers removed"
fi

if [ "$1" == "-a" ]; 
then
    rm -f .zone_config.json
    rm -f .camera_status_config.json
    echo "Zone and status makers removed"    
fi

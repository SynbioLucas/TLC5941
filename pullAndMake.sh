#!/bin/bash
# Pulls most recent files from Github repo and runs make
# Reset the internet connection to make sure it's connected
echo "Resetting the internet connection"
ifup wlan0
# Pull most recent code
echo "Pulling most recent code"
git pull origin master
# Build the code
echo "Building..."
make

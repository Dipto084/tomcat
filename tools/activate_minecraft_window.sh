#/usr/bin/bash

# This script activates the Minecraft window and makes it full-screen.

wmctrl -a 'Minecraft 1.11.2'
wmctrl -b toggle,fullscreen -r 'Minecraft 1.11.2'

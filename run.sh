#!/bin/bash
# Start a local server and open the game in browser

python3 -m http.server &

sleep 2

# For macOS
open http://localhost:8000

# Uncomment this line for Linux:

xdg-open http://localhost:8000
#!/usr/bin/env bash
xterm -hold -fs 24 -fa "Monospace" -e  echo "Hand Safety Service is Loading...!!!"&
cd /home/mano/Safety_Kit
./run.sh
kill $(jobs -pr)

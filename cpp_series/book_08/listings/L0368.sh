#!/bin/sh
set -eu
g++ -std=c++20 -O2 server.cpp -o server
python3 lab.py

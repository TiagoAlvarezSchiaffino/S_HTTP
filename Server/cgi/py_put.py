#!/usr/bin/env python3

import requests
import sys

if len(sys.argv) != 2:
    print("Usage: py_delete.py <url>")
    sys.exit(1)

try:
	response = requests.put(sys.argv[1], data = "A put request sent by py_put.py!")
except:
	print("Error: Could not connect to server")
	sys.exit(1)

print("Status code received: ", response.status_code)
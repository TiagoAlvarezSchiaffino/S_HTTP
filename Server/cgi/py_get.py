#!/usr/bin/env python3

import requests
import sys

if len(sys.argv) != 2:
    print("Usage: py_get.py <url>")
    sys.exit(1)

for i in range(1, 100000):
	try:
		response = requests.get(sys.argv[1])
	except:
		print("Error: Could not connect to server")
		sys.exit(1)

	print(f"{i}: Status code received: {response.status_code}")

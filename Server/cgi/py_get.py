#!/usr/bin/env python3

import requests
import sys

if len(sys.argv) < 2 or len(sys.argv) > 3:
    print("Usage: py_get.py <url> [count]")
    sys.exit(1)

count = 1
if (len(sys.argv) == 3):
	count = int(sys.argv[2])

for i in range(0, count):
	try:
		# f = open("../unchunk.txt", "r")
		# print(len(f.read()))
		# response = requests.get(sys.argv[1], data=f.read())
		response = requests.get(sys.argv[1])
	except:
		print("Error: Could not connect to server")
		sys.exit(1)

	print(f"{i + 1}: Status code received: {response.status_code}")

# jng 2000		18.47 real         4.90 user         0.97 sys
# hyap 2000	    17.57 real         4.83 user         0.94 sys
# mine 2000		14.95 real         4.56 user         0.86 sys

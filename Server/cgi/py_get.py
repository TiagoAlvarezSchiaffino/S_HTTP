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
		f = open("../unchunk.txt", "r")
		# print(len(f.read()))
		response = requests.get(sys.argv[1], data=f.read())
		# response = requests.get(sys.argv[1])
	except:
		print("Error: Could not connect to server")
		sys.exit(1)

	print(f"{i + 1}: Status code received: {response.status_code}")
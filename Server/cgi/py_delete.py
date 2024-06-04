#!/usr/bin/env python3

import requests
import sys

if len(sys.argv) != 2:
    print("Usage: py_delete.py <port>")
    sys.exit(1)

try:
	url = "http://localhost:" + str(sys.argv[1]) + "/cgi/delete/deleteMe"
	response = requests.delete(url)
except:
	print("Error: Could not connect to server")
	sys.exit(1)

print("Status code received: ", response.status_code)
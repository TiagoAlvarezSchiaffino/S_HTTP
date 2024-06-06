#!/usr/bin/env python3

import requests
import sys

if len(sys.argv) != 2:
    print("Usage: py_delete.py <url>")
    sys.exit(1)

try:
	if sys.argv[1].split('/')[-1] != 'toDelete':
		print("File to delete is not toDelete. This is a security measure to prevent accidental deletion of files.")
	else:
		response = requests.delete(sys.argv[1])
		print("Status code received: ", response.status_code)
except:
	print("Error: Could not connect to server")
	sys.exit(1)
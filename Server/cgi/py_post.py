#!/usr/bin/env python3

import requests
import sys

if len(sys.argv) != 2:
    print("Usage: py_post.py <url>")
    sys.exit(1)

try:
	headers = {
		"Transfer-Encoding": "chunked",
		"Content-Type": "application/octet-stream"
	}
	response = requests.post(sys.argv[1], headers=headers, data="14\r\n12345678901234567890\r\n8\r\n, world!\r\n0\r\n\r\n")
except:
	print("Error: Could not connect to server")
	sys.exit(1)

print("Status code received: ", response.status_code)
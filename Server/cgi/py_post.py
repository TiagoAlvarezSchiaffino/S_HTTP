#!/usr/bin/env python3

import requests
import sys

if len(sys.argv) != 2:
    print("Usage: py_post.py <url>")
    sys.exit(1)

def data_generator():
    f = open("../test.txt", "rb")
    data = f.read()
    # data = b"12345678901234567890\n Hello World!"
    chunk_size = 30000
    for i in range(0, len(data), chunk_size):
        yield data[i:i+chunk_size]

try:
    headers = {
        "Content-Type": "application/octet-stream",
        "X-Secret-Header-For-Test": "1",
    }
    # response = requests.post(sys.argv[1], headers=headers, data="1234567890", stream=True)
    response = requests.post(sys.argv[1], headers=headers, data=data_generator(), stream=True)
except:
	print("Error: Could not connect to server")
	sys.exit(1)

print("Status code received: ", response.status_code)
# print(response.headers)
# print(response.text, end="")

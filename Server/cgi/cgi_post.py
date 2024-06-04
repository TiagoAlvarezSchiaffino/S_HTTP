#!/usr/bin/env python3
import cgi
import urllib.request
import urllib.parse

# Define the URL to send the POST request to
url = 'https://example.com/path/to/endpoint'

# Define the POST data to send
post_data = {'param1': 'value1', 'param2': 'value2'}

# Encode the POST data
post_data_encoded = urllib.parse.urlencode(post_data).encode('utf-8')

# Send the POST request
with urllib.request.urlopen(url, data=post_data_encoded) as response:
    # Get the response from the server
    response_text = response.read().decode('utf-8')

# Set the response headers
print('Content-Type: text/html')
print()

# Print the response from the server
print(response_text)
#!/usr/bin/env php
<?php
header("Transfer-Encoding: chunked");
echo "HTTP/1.1 200 OK\r\n";
echo "Content-type:text/html\r\n\r\n";
echo "Hello from PHP (Chunked)!";
?>
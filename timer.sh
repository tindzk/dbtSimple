#!/bin/sh

echo "GET /timer/wait/0 HTTP/1.1
Host: localhost

GET /timer/wait/3 HTTP/1.1
Host: localhost

GET /timer/wait/2 HTTP/1.1
Host: localhost

GET /timer/wait/1 HTTP/1.1
Host: localhost
Connection: close

" | nc 127.0.0.1 8080

echo "GET /timer/interval/3 HTTP/1.1
Host: localhost
Connection: close

" | nc 127.0.0.1 8080

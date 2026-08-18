# http1.1 server in C

## What is this project about?
HTTP server that listens and accepts 1.1 version messages, parses them into request struct, gives it to router, finds match for provided URI and method in routes, calls it's handler function and gets response struct. When there is no match, returns 404. It can work asynchronously for multiple clients thanks to threads.

## Why i made it?
I wanted to learn what is the happening under the hood of frameworks we use everyday. I was shocked that http relies on string parsing actually (well, at least for http1.1 version). I also wanted to improve my c skills. I split the project into 7 parts that goes from basic tcp connection in c and implementing arraylist to multiple threads and (completely optional) query string parsing. I progressively developed it step by step by searching each concepts. I do not use AI to write code when i am learning something new except for asking purposes, so all of it hand made. Here is the project.

## Caveats?
May v2 come to:
- support keep-alive connections.
- serve static files directly with MIME table.
- handle partial read/write.
- return more error types.
- use select or thread pool for optimizing.
- support path parameters.
- add logging.
- add tests.
- make more secure and bug free.
- do refactor.
- use makefile.

## Usage
Make sure you have compiler installed to compile and link the codes.
```
git clone https://github.com/turkerkiv/http-server-from-scratch
cd http-server-from-scratch
gcc router.c response.c request.c main.c arraylist.c -o server
```
Once you compile then you can run the server:
```
./server
```
Server now runs. Use `Ctrl + C` in terminal to shut down the server. You can send requests from wherever you want. I mostly used curl but there is client.c in the files to use if you want. Here are examples:
- curl:
```
curl -X POST "http://localhost:8080/api/hello?param1=value1&param2=value2" \
     -H "Content-Type: application/json" \
     -d '{"key": "value"}'
```
- client.c:
```
gcc client.c -o client
./client
```
Output to server terminal looks like this when you send a request:
```
Socket opened with a descriptor: 3
Socket bind successfull
Socket listening up to 2 client
Socket accepted client with a new descriptor: 4
METHOD: POST
URI: /api/hello?param1=value1&param2=value2
PROTOCOL VERSION: HTTP/1.1
QUERY STRING KEY0: param1
QUERY STRING VALUE0: value1
QUERY STRING KEY1: param2
QUERY STRING VALUE1: value2
URI WITHOUT QUERY STRINGS: /api/hello
HEADER KEY0: Host
HEADER VALUE0: localhost:8080
HEADER KEY1: Content-Type
HEADER VALUE1: application/json
HEADER KEY2: Content-Length
HEADER VALUE2: 16
HEADER KEY3: Connection
HEADER VALUE3: close
BODY: {"key": "value"}
```
And output to terminal that we used curl looks like:
```
{"message": "Hello from API!"}
```

To add route as you wish, go to `router.c`, add routes there and also prepare function to handle that route. I added 4 routes to test:
- / GET
- /user GET
- /users GET
- /api/hello POST

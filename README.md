# c-client-server-socket
Implement a very simple client-server model in C

These codes is made by Robert Ingalls. I just added some lines to make them can be compiled at my macbook.

The full socket tutorial is [here](https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html). Please check it out. It's pretty clear.

### instructions
1. compile programs
```
gcc -o server.c
gcc -o client.c
```
2. open two terminal tabs for each one
```
// at server tab
// run the server program with a port number
./server 9999
```

```
// at client tab
// run the client program with the host name and port number
// for the local test, we can just pass 'localhost' 
./client localhost 9999
```

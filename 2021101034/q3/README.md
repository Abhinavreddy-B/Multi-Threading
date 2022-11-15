# Compile & Run:
* Server Terminal:
    ```
    $ g++ server.cpp -o server
    $ ./server
    ```
* Client Terminal:
    ```
    $ g++ client.cpp -o client
    $ ./client
    ```

# Algorithm:
## Client:
* Establish Connection
* Take Input From User.
* Ping The Server on port number of server 0
* wait for response 
* print responce if input is `pt`
* Loop again

## Server:
1. Take Input , and precompute the shortest path to each node from node 0.
1. create N Nodes, at each Node:
1. Start a server at its assigned Port Number.
1. start listening to Client.
1. Wait till some Client is connected
1. Wait till Clients send a message.
1. check for special case `pt` -> return data to Client accordingly.
1. otherwise , depending on destination , find the next Server to shich the message is to be passed.
1. Create a connection with next server.
1. Ping the nextServer with the message and destination.
1. loop the steps `4` -> `11`

# Implementation:
## Client:
* Connect to Server 0 , Get Server Socket FD from the server 0 port using the given `get_socket_fd()`
* Take Imput from user.
* Encode the input as "`dest|message`" ( In case of pt, Encode as "`-1|pt`")
* send the encoded string to Server using the given `send_string_on_socket()` , Wait for response using `read_string_from_socket()`.
* Print response if the command is `pt`.

## Server:
* Each Server Node is simulated using a single Thread.
In each thread, create Welcome socket using `socket()` , bind welcome socket to assigned port number using `bind()`.
* Listen to client using `listen()`
* accept client using `accept()`
* Once a client is connected , read message using given `read_string_from_socket()`
* decode the received message , to find its destination and content, log the output.
* find the nextNode to which the message is to be forwarded, and connect to the next server using `get_socket_fd()`
* asyncronously call `send_string_on_socket()` ( asyncronous so that we can put a sleep inside `send_string_on_socket()` , to simulate network delays , without blocking the Parent Node).

# Follow Up Question:
## How to handle server failure.
* Whenever a server gets failed , we can can remove the edges that connect to the server , 
* Then run Dijkstra Algorithm Once more , from the node where a packet is waiting , Find the shortest path to destination
* Re route the message through new path.
* if we are not able to reach destination without that Node, we can simply throw away the Package , and report the problem to the Client. Or store the message in a Queue, and then try to resend it , after the server is Up.
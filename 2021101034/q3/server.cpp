#include <iostream>
#include <bits/stdc++.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <assert.h>
#include <tuple>

using namespace std;

typedef long long ll;
typedef vector<int> vi;

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#define PORT_ARG 8001
#define MAX_CLIENTS 4
#define MAX_LENGTH 100

// Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define ANSI_RESET "\x1b[0m"

typedef long long LL;

#define pb push_back
#define debug(x) cout << #x << " : " << x << endl
#define part cout << "-----------------------------------" << endl;

const int initial_msg_len = 256;
const ll buff_sz = 1048576;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int n, m;
int **nextNode;
int *distances;
int *Visited;
int *parent;
int *portNum;
int **delays;
pthread_t *threads;

sem_t Creation;

pair<string, int> read_string_from_socket(const int &fd, int bytes)
{
    std::string output;
    output.resize(bytes);

    int bytes_received = read(fd, &output[0], bytes - 1);
    // debug(bytes_received);
    if (bytes_received <= 0)
    {
        cerr << "Failed to read data from socket. \n";
    }

    output[bytes_received] = 0;
    output.resize(bytes_received);
    // debug(output);
    return {output, bytes_received};
}

int send_string_on_socket(int fd, const string &s,int del)
{
    if(del != 0){
        sleep(del);
    }
    // debug(s.length());
    int bytes_sent = write(fd, s.c_str(), s.length());
    if (bytes_sent < 0)
    {
        cerr << "Failed to SEND DATA via socket.\n";
    }

    return bytes_sent;
}

pair<int,string> handle_client_connection(int client_socket_fd)
{
    // int client_socket_fd = *((int *)client_socket_fd_ptr);
    //####################################################

    int received_num, sent_num;

    /* read message from client */
    int ret_val = 1;

    while (true)
    {
        string cmd;
        tie(cmd, received_num) = read_string_from_socket(client_socket_fd, buff_sz);
        ret_val = received_num;
        // debug(ret_val);
        // printf("Read something\n");
        if (ret_val <= 0)
        {
            // perror("Error read()");
            printf("Server could not read msg sent from client\n");
            goto close_client_socket_ceremony;
        }
        
        string msg_to_send_back = "Ack: " + cmd;

        int dest;
        char mesg[MAX_LENGTH];
        sscanf(cmd.c_str(),"%d|%[^\n]s",&dest,mesg);
        string s=mesg;

        ////////////////////////////////////////
        // "If the server write a message on the socket and then close it before the client's read. Will the client be able to read the message?"
        // Yes. The client will get the data that was sent before the FIN packet that closes the socket.

        if(dest == -1){
            msg_to_send_back = "";
            msg_to_send_back += "dest\tforw\tdelay\n";
            for(int i=1;i<n;i++){
                msg_to_send_back += to_string(i) + "\t" + to_string(nextNode[0][i]) + "\t" + to_string(distances[i]) + "\n";
            }
        }
        int sent_to_client = send_string_on_socket(client_socket_fd, msg_to_send_back,0);
        // debug(sent_to_client);
        if (sent_to_client == -1)
        {
            perror("Error while writing to client. Seems socket has been closed");
            goto close_client_socket_ceremony;
        }

        
        return  make_pair(dest,s);
    }
close_client_socket_ceremony:
    close(client_socket_fd);
    printf(BRED "Disconnected from client" ANSI_RESET "\n");
    return make_pair(0,string(NULL));
}

int get_socket_fd(struct sockaddr_in *ptr, int port)
{
    struct sockaddr_in server_obj = *ptr;

    // socket() creates an endpoint for communication and returns a file
    //        descriptor that refers to that endpoint.  The file descriptor
    //        returned by a successful call will be the lowest-numbered file
    //        descriptor not currently open for the process.
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error in socket creation for CLIENT");
        exit(-1);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    int port_num = port;

    memset(&server_obj, 0, sizeof(server_obj)); // Zero out structure
    server_obj.sin_family = AF_INET;
    server_obj.sin_port = htons(port_num); // convert to big-endian order

    // Converts an IP address in numbers-and-dots notation into either a
    // struct in_addr or a struct in6_addr depending on whether you specify AF_INET or AF_INET6.
    // https://stackoverflow.com/a/20778887/6427607

    /////////////////////////////////////////////////////////////////////////////////////////
    /* connect to server */

    if (connect(socket_fd, (struct sockaddr *)&server_obj, sizeof(server_obj)) < 0)
    {
        cout << "Problem in connecting to the server\n";
        exit(-1);
    }

    // part;
    //  printf(BGRN "Connected to server\n" ANSI_RESET);
    //  part;
    return socket_fd;
}

void *server_main(void *arg)
{
    int i, j, k, t, n;
    int SNo = *((int *)arg);

    int wel_socket_fd, client_socket_fd, port_number;
    socklen_t clilen;

    struct sockaddr_in serv_addr_obj, client_addr_obj;
    /////////////////////////////////////////////////////////////////////////
    /* create socket */
    /*
    The server program must have a special door—more precisely,
    a special socket—that welcomes some initial contact
    from a client process running on an arbitrary host
    */
    // get welcoming socket
    // get ip,port
    /////////////////////////
    wel_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (wel_socket_fd < 0)
    {
        perror("ERROR creating welcoming socket");
        exit(-1);
    }

    //////////////////////////////////////////////////////////////////////
    /* IP address can be anything (INADDR_ANY) */
    bzero((char *)&serv_addr_obj, sizeof(serv_addr_obj));
    port_number = portNum[SNo];
    serv_addr_obj.sin_family = AF_INET;
    // On the server side I understand that INADDR_ANY will bind the port to all available interfaces,
    serv_addr_obj.sin_addr.s_addr = INADDR_ANY;
    serv_addr_obj.sin_port = htons(port_number); // process specifies port

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* bind socket to this port number on this machine */
    /*When a socket is created with socket(2), it exists in a name space
       (address family) but has no address assigned to it.  bind() assigns
       the address specified by addr to the socket referred to by the file
       descriptor wel_sock_fd.  addrlen specifies the size, in bytes, of the
       address structure pointed to by addr.  */

    // CHECK WHY THE CASTING IS REQUIRED
    if (bind(wel_socket_fd, (struct sockaddr *)&serv_addr_obj, sizeof(serv_addr_obj)) < 0)
    {
        perror("Error on bind on welcome socket: ");
        exit(-1);
    }
    //////////////////////////////////////////////////////////////////////////////////////

    /* listen for incoming connection requests */

    while (1)
    {
        /* accept a new request, create a client_socket_fd */
        /*
        During the three-way handshake, the client process knocks on the welcoming door
of the server process. When the server “hears” the knocking, it creates a new door—
more precisely, a new socket that is dedicated to that particular client.
        */
        // accept is a blocking call
        listen(wel_socket_fd, MAX_CLIENTS);
        // cout << "Server " << SNo << " has started listening on the LISTEN PORT" << endl;
        clilen = sizeof(client_addr_obj);

        int dest;
        sem_post(&Creation);

        // printf("Waiting for a new client to request for a connection - %d\n", SNo);
        client_socket_fd = accept(wel_socket_fd, (struct sockaddr *)&client_addr_obj, &clilen);
        if (client_socket_fd < 0)
        {
            perror("ERROR while accept() system call occurred in SERVER");
            exit(-1);
        }

        // printf(BGRN "New client connected from port number %d and IP %s \n" ANSI_RESET, ntohs(client_addr_obj.sin_port), inet_ntoa(client_addr_obj.sin_addr));

        string mesg;
        tie(dest,mesg) = handle_client_connection(client_socket_fd);

        if(dest == -1){
            continue;
        }

        if (SNo == dest)
        {
            cout << "Data received at node:" << SNo << "; Source : 0; Destination :" << dest << ";  Message : \"" << mesg << "\";\n";
            continue;
        }

        cout << "Data received at node:" << SNo << "; Source : 0; Destination :" << dest << "; Fowarded_Destination : " << nextNode[SNo][dest] << "; Message : \"" << mesg << "\";\n";
        struct sockaddr_in server_obj;
        int socket_fd = get_socket_fd(&server_obj, portNum[nextNode[SNo][dest]]);

        // cout << "Connection to server successful" << endl;

        future<int> fut = async(send_string_on_socket, socket_fd,to_string(dest) + "|" + mesg,delays[SNo][nextNode[SNo][dest]]);
        // send_string_on_socket(socket_fd, to_string(dest) + "|" + mesg,del);
        // cout << "====" << endl;
    }

    close(wel_socket_fd);

    return 0;
}

void dijkstra(vector<pair<int, int>> adj[])
{
    int cnt = 0;
    while (cnt != n)
    {
        int minnode = -1;
        for (int i = 0; i < n; i++)
        {
            if (Visited[i] == false && (minnode == -1 || distances[i] < distances[minnode]))
            {
                minnode = i;
            }
        }
        Visited[minnode] = true;
        for (auto j : adj[minnode])
        {
            if (distances[minnode] + j.second < distances[j.first])
            {
                parent[j.first] = minnode;
                distances[j.first] = distances[minnode] + j.second;
            }
        }
        cnt++;
    }
}

int main()
{
    cin >> n >> m;
    vector<pair<int, int>> adj[n];
    vector<bool> vst;
    nextNode = (int **)malloc(sizeof(int *) * n);
    delays = (int **)malloc(sizeof(int *) * n);
    distances = (int *)malloc(sizeof(int) * n);
    portNum = (int *)malloc(sizeof(int) * n);
    Visited = (int *)malloc(sizeof(int) * n);
    parent = (int *)malloc(sizeof(int) * n);
    threads = (pthread_t *)malloc(sizeof(pthread_t) * n);

    for (int i = 0; i < n; i++)
    {
        nextNode[i] = (int *)malloc(sizeof(int) * n);
        delays[i] = (int *)malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++)
        {
            nextNode[i][j] = -1;
        }
        distances[i] = INT_MAX;
        Visited[i] = false;
    }
    for (int i = 0; i < m; i++)
    {
        int a, b, d;
        cin >> a >> b >> d;
        adj[a].push_back(make_pair(b, d));
        adj[b].push_back(make_pair(a, d));
        delays[a][b] = d;
        delays[b][a] = d;
    }
    
    distances[0] = 0;
    dijkstra(adj);
    for (int i = 0; i < n; i++)
    {
        cout << parent[i] << '\n';
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j != 0;)
        {
            nextNode[parent[j]][i] = j;
            j = parent[j];
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << nextNode[i][j] << ' ';
        }
        cout << '\n';
    }
    for (int i = 0; i < n; i++)
    {
        portNum[i] = PORT_ARG + i;
    }
    sem_init(&Creation, 0, 0);
    for (int i = 0; i < n; i++)
    {
        pthread_create(&threads[i], NULL, server_main, &i);
        sem_wait(&Creation);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }
}
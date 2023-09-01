#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
    int client;
    int port_number = 61000;
    int buf_size = 1024;
    char buffer[buf_size];
    const char* ip = "127.0.0.1"; //localhost

    struct sockaddr_in server_addr;

    /* ---------- ESTABLISHING SOCKET CONNECTION ---------- */

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cerr << "Error establishing socket..." << endl;
        exit(1);
    }

    cout << "Socket client has been created..." << endl;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        cerr << "Invalid address or address not supported." << endl;
        exit(1);
    }

    /* ------------- SEND CALL REQUEST ------------- */

    if (connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        cerr << "Connection failed." << endl;
        exit(1);
    }

    cout << "Connection to the server port number: " << port_number << endl;
    
    cout << "Awaiting confirmation from the server..." << endl;
    ssize_t bytesReceived = recv(client, buffer, buf_size - 1, 0);
    if (bytesReceived <= 0)
    {
        cerr << "Error receiving confirmation from the server." << endl;
        close(client);
        exit(1);
    }
    buffer[bytesReceived] = '\0'; 
    cout << "Connection confirmed, you are good to go..." << endl;

    cout << "\n\nEnter # to end the connection" << endl;

    while (true) {
        /* Sending data to server */
        cout << "Client: ";
        cin.getline(buffer, buf_size);
        send(client, buffer, buf_size, 0);
        if (strcmp(buffer, "#") == 0) {
            cout << "\nConnection terminated.\nGoodbye..." << endl;
            break;
        }

        /* Receiving data from server */
        cout << "Server: ";
        bytesReceived = recv(client, buffer, buf_size - 1, 0);
        if (bytesReceived <= 0) {
            cerr << "Error receiving data from the server." << endl;
            close(client);
            exit(1);
        }
        buffer[bytesReceived] = '\0'; 
        cout << buffer << endl;   
    }

    /* ---------------- CLOSE CALL ------------- */

    close(client);
    return 0;
}

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main() {
    int server; 
    int port_number = 61000;
    int buf_size = 1024;
    char buffer[buf_size];

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size = sizeof(client_addr);

    /* ---------- ESTABLISHING SOCKET CONNECTION ---------- */

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0) 
    {
        cerr << "Error establishing socket..." << endl;
        exit(1);
    }

    cout << "Socket server has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_number);

    /* ---------- BINDING THE SOCKET ---------- */

    if (bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        cerr << "Error binding connection." << endl;
        exit(1);
    }

    /* ------------- LISTENING CALL ------------- */

    listen(server, 1);
    cout << "Listening for clients..." << endl;

    /* ------------- ACCEPTING CLIENTS  ------------- */

    int clientCount = 1;
    int client = accept(server, (struct sockaddr*)&client_addr, &client_size);

    if (client < 0) 
    {
        cerr << "Error on accepting client." << endl;
        exit(1);
    }

    cout << "Connected with client #" << clientCount << ". You are good to go..." << endl;

    const char* confirmationMsg = "=> Server connected...\n";
    send(client, confirmationMsg, strlen(confirmationMsg), 0);

    while (true) 
    {
        /* Receiving data from client */
        memset(buffer, 0, buf_size);
        int bytesReceived = recv(client, buffer, buf_size, 0);

        if (bytesReceived <= 0)
        {
            cerr << "Connection closed or error receiving data from client." << endl;
            break;
        }

        cout << "Client: " << buffer << endl;

        /* Sending data to client */
        cout << "Server: ";
        cin.getline(buffer, buf_size);
        send(client, buffer, buf_size, 0);

        if (strcmp(buffer, "#") == 0) 
        {
            cout << "Connection terminated by server." << endl;
            break;
        }
    }

    /* ---------------- CLOSE CALL ------------- */

    close(client);
    close(server);

    return 0;
}

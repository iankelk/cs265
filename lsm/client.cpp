#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    // Connect to server
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating client socket" << std::endl;
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1234);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    if (connect(client_socket, (sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(client_socket);
        return 1;
    }

    // Send commands to server
    std::stringstream ss;
    ss << "p 10 7\n";
    ss << "p 63 222\n";
    ss << "p 15 5\n";
    ss << "s\n";
    std::string command_str = ss.str();
    send(client_socket, command_str.c_str(), command_str.size(), 0);

    // Receive responses from server
    char buffer[1024];
    std::stringstream response_ss;
    ssize_t n_read;
    while ((n_read = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        // Append received data to response stringstream
        response_ss << std::string(buffer, n_read);
        
        // Check if the response is complete
        if (response_ss.str().back() == '\n') {
            // Parse response and clear buffer
            std::string response_str = response_ss.str();
            std::istringstream iss(response_str);
            std::string token;
            while (std::getline(iss, token)) {
                std::cout << token << std::endl;
            }
            response_ss.str("");
            memset(buffer, 0, sizeof(buffer));
        }
    }

    // Clean up resources
    close(client_socket);
    return 0;
}

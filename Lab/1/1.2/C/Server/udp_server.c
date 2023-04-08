#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 524288
#define LONGEST_NAME_SIZE 128

int create_socket()
{
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("failed opening datagram socket");
        exit(1);
    }

    return server_socket;
}

void bind_server(int server_socket, struct sockaddr_in *server_address)
{
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = INADDR_ANY;
    server_address->sin_port = 0;
    
    if (bind(server_socket, (struct sockaddr *) server_address, sizeof *server_address)
        == -1) {
        perror("failed binding stream socket");
        exit(1);
    }
}

void query_server_address(int server_socket, struct sockaddr_in *address_buffer)
{
    int length = sizeof(*address_buffer);
    if (getsockname(server_socket, (struct sockaddr *) address_buffer, &length)
         == -1) {
        perror("failed getting socket name");
        exit(1);
    }
}

void log_assigned_server_port(struct sockaddr_in *valid_address_buffer)
{
        printf("Server port #%d\n", ntohs(valid_address_buffer->sin_port));
}

bool is_work()
{
    return true;
}

int get_end_of_message_index(int message_size, int message_buffer_size)
{
    return message_size > message_buffer_size ? message_buffer_size - 1 : message_size;
}

void get_host_name(struct sockaddr_in *address, char *name_buffer, int name_buffer_length)
{
    gethostbyaddr((const void*) &address->sin_addr, sizeof(address->sin_addr), AF_INET);
    struct hostent *host_info = gethostbyaddr((const void*) &address->sin_addr, sizeof(address->sin_addr), AF_INET);
    if (host_info == (struct hostent *) 0)
    {
        const char* unknown_name = "Unknown host";
        memcpy(name_buffer, (const void*) unknown_name, strlen(unknown_name) + 1);
    }

    // avoid buffer overflow
    if (strlen(host_info->h_name) + 1 > name_buffer_length)
    {
        perror("Client's name is too long");
        memcpy(name_buffer, (const void*) host_info->h_name, name_buffer_length);
        name_buffer[name_buffer_length - 1] = '\0';
        name_buffer[name_buffer_length - 2] = '.';
        name_buffer[name_buffer_length - 3] = '.';
        name_buffer[name_buffer_length - 4] = '.'; 
    }

    memcpy(name_buffer, (const void*) host_info->h_name, strlen(host_info->h_name) + 1);    
}

int main(int argc, char **argv)
{
    int server_socket;
    struct sockaddr_in server_address, client_address;
    char message_buffer[BUFFER_SIZE];

    server_socket = create_socket();
    bind_server(server_socket, &server_address);
    query_server_address(server_socket, &server_address);
    log_assigned_server_port(&server_address);

    int client_address_length = sizeof(client_address);
    int bytes_received_count;

    char name_buffer[LONGEST_NAME_SIZE];

    do {
        bytes_received_count = recvfrom(server_socket, message_buffer, BUFFER_SIZE, 0,
                                ( struct sockaddr *) &client_address,
			                    &client_address_length);
        get_host_name(&client_address, name_buffer, LONGEST_NAME_SIZE);
        // careful of buffer overflow
        message_buffer[get_end_of_message_index(bytes_received_count, BUFFER_SIZE)] = '\0';
        printf("%s@%s says: %s (%d bytes)\n", name_buffer, inet_ntoa(client_address.sin_addr), message_buffer, bytes_received_count);
    } while (is_work());

     exit(0);
}
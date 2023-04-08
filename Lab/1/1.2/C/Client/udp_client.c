#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#define DATA "So much depends upon a red wheelbarrow glazed with rain water beside the white chickens."
#define BUFFER_SIZE 524288

int create_socket()
{
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("failed opening datagram socket");
        exit(1);
    }

    return client_socket;
}

void fill_server_address(const char *server_name, struct sockaddr_in *server_address)
{
    struct hostent *host_info = gethostbyname(server_name);
    if (host_info == (struct hostent * ) 0)
    {
        fprintf(stderr, "%s: unknown server\n", server_name);
        exit(2);
    }
    
    struct in_addr **addr_list = (struct in_addr**) host_info->h_addr_list;
    printf("Name: '%s' is mapped to addresses: ", server_name);
    for (int i = 0; addr_list[i] != (struct in_addr*) 0; i++)
    {
      printf("%s ", inet_ntoa(*addr_list[i]));
    }

    printf("\n");
    memcpy((void*) &(server_address->sin_addr), (const void*) (host_info->h_addr_list[0]), host_info->h_length);
}

void initialize_buffer(char *buffer, int buffer_size, char content_character)
{
  for (int i = 0; i < buffer_size; i++)
  {
    buffer[i] = content_character;
  }
}

int main(int argc, char * argv[]) {
  int client_socket = create_socket();
  struct sockaddr_in server_address;
  char message_buffer[BUFFER_SIZE];
  initialize_buffer(message_buffer, BUFFER_SIZE, 'a');
  
  char *server_name = argv[1];
  fill_server_address(server_name, &server_address);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(argv[2]));
  char *server_address_text = inet_ntoa(server_address.sin_addr);
  
  int current_message_length;

  while (current_message_length < BUFFER_SIZE && errno != EMSGSIZE)
  {
    if (sendto(client_socket, message_buffer, current_message_length, 0,
      (struct sockaddr * ) &server_address, sizeof server_address) == -1)
    {
        perror("sending datagram message");
    }
    printf("Sending message #%d of length %d bytes to server %s@%s\n", current_message_length, current_message_length, server_name, server_address_text);
    current_message_length++;
  }

  printf("=======\n=======\nMaximum size of a datagram sent atomically: %d\n", current_message_length - 2); // 2 is divided cause after the last one UNsuccesfuly sent, the counter is incremented by 1 
  close(client_socket);
  exit(0);
}
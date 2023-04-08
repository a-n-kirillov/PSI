```
cd [source_code]/1/1.1/C/Server
docker build -t z13_1.1_c_server .
cd [source_code]/1/1.1/C/Client
docker build -t z13_1.1_c_client .
docker run -it --rm --network-alias z13_1.1_c_server --network z13_network --name z13_1.1_c_server z13_1.1_c_server
```
Server port is printed on the console, now we can run the client using the obtained port:
```
docker run -it --rm --network-alias z13_1.1_c_client --network z13_network --name z13_1.1_c_client z13_1.1_c_client z13_1.1_c_server [obtained_server_port] [initial_message_size] [packets_to_send_count] [succesive_size_multiplier]
```

First we send a packet of length `initial_message_size`.  
We send exactly `packets_to_send_count` packets. Each packet being send gets its size calculated as: previous_packet_size * `succesive_size_multiplier`, with the first packet being exactly `initial_message_size` bytes long.  

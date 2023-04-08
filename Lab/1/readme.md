# Creating docker images
```
docker build -t z13_[task_number]_[implementation_language]_[client | server]:latest [dockerfile_path]
```

Example usage:
```
cd [lab_code]/1/1.1/Client
docker build -t z13_1.1_c_client:latest .
```
# Running server
Having followed the format defined earlier, enter command:
```
docker run -it --rm --network-alias z13_1.1_c_server --network z13_network --name z13_1.1_c_server z13_1.1_c_server:latest
```

Assigned port is logged to the console.
# Running Client
Call arguments explanation:
```
docker run -it --rm --network-alias z13_1.1_c_client --network z13_network --name z13_1.1_c_client z13_1.1_c_client:latest [server_name] [server_port] [initial_message_size] [datagrams_to_send_count] [multiplication_factor]
```

Each datagram being sent gets its size by multiplying the size of previously sent datagram by ```multiplication_factor```.

**REMINDER** Server port is logged to the console when the server container starts.
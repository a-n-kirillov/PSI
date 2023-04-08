```
cd [source_code]/1/1.2/C/Server
docker build -t z13_1.2_c_server .
cd [source_code]/1/1.2/C/Client
docker build -t z13_1.2_c_client .
docker run -it --rm --network-alias z13_1.2_c_server --network z13_network --name z13_1.2_c_server z13_1.2_c_server
```
Server port is printed on the console, now we can run the client using the obtained port:
```
docker run -it --rm --network-alias z13_1.2_c_client --network z13_network --name z13_1.2_c_client z13_1.2_c_client z13_1.2_c_server [obtained_server_port]
```

As an output, we receive the maximum datagram length that we are looking for.
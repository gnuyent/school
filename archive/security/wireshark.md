# Wireshark

## Background

Wireshark is likely the most popular network protocol and traffic analyzer available today.

It is useful for:

* Network Security
* Network Engineering
* Software Development

**Command and Control (C2)** - How something is controlled post-exploitation.

**Beaconing** - Calling back to ensure the attacker that the infected system is still accessible. Typically some type of connection (HTTPS, etc) that will call back over a given interval.

## Key Starting Points

### Conversations

**Conversations** contain a list of network connections between two endpoints or hosts.

* Useful for investigating suspicious hosts, malware infection, or understanding a communication path between devices.

### Endpoints

**Endpoints** contain a list of hosts to or from an address.

* Useful for investigating endpoints with many packets.

### Follow Protocol Streams

**Protocol Streams** filter and follow a particular network session (e.g. TCP, UDP, TLS, HTTP).

Select packet -> right click -> Follow -> Select desired protocol stream.

## Important Features

### Name Resolution

This option converts some numerical address values into a human-readable format.

### GeoIP Resolution

GeoIP Resolution allows you to see physical IP address location and provider information. This requires the MaxMind database file.

Filter example:

`ip and not ip.geoip.country == "United States"`

### TLS SSL Decrypt

A web browser Key log file or RSA private key can be used to apply decryption of TLS packet capture sessions inside Wireshark.

<https://gitlab.com/wireshark/wireshark/-/wikis/TLS#tls-decryption>
<https://www.comparitech.com/net-admin/decrypt-ssl-with-wireshark/>

## Filter Language & Expressions

Wireshark allows filters to be used against packets. These filters are used by a series of expressions.

Expressions check for a protocol, field, the value found in a field, or even compare two fields.

* Expressions can be combined.

Comparisons can be combined with logical operators like "and" and "or", and parentheses into complex expressions.

Boolean can be 1 for true or 0 for false.

Functions convert fields.

### Filter Primer

Both source or destination IP

`ip.addr == <ip>`

Source or Destination IP

`ip.src == <ip>`

`ip.dst == <ip>`

IP subnet

`ip.addr == <ip>/24`

Port Number

`port <num`

Port Number Range

`tcp portrange <start>-<end>`

### Encrypted Traffic Insights

**DNS Queries for host name discovery**

Filter on available queries

`dns.qry.name`

Filter out for host name in a DNS query

`dns.qry.name == "<hostname>"`

**TLS Server Name Indication**

Search encrypted traffic for certain domain

`tls.handshake.extensions_server_name contains "<hostname>"`

check if the SNI field exists

`ssl.handshake.extension.type == 0`


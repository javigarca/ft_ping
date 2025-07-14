# ft_ping

A reimplementation of the `ping` command in C, based on the behavior of `inetutils 2.0`.

## 📦 Description

`ft_ping` is a network utility that sends ICMP Echo Request packets to a given host and measures the round-trip time (RTT) for each reply. This project replicates the basic functionality of the Unix `ping` command using raw sockets and manual packet construction.

Developed in C as part of the 42 school curriculum.

## ✅ Features

- Manual construction of ICMP Echo Request packets
- IPv4 support
- Raw socket usage (`SOCK_RAW`)
- Verbose mode with `-v`
- Help output with `-?` or `--help`
- Statistics report on exit (`SIGINT`)
- RTT calculation per packet
- Signal handling (`SIGINT`)

## ⚠️ Permissions

`ft_ping` requires root privileges or appropriate capabilities to open a raw socket.

### Option 1: Run with `sudo`
sudo ./ft_ping 8.8.8.8

### Option 2 :Grant raw socket capability (recommended)
sudo setcap cap_net_raw+ep ./ft_ping

Build: 
make
 -or-
make setcap 
  (to grant raw socket capability on build)

The executable will be located at ./bin/ft_ping.
Usage:
./ft_ping [options] <destination>

Example:
./ft_ping -v google.com

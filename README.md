# 42 IRC Server Project

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
6. [Commands](#commands)

## Introduction

Welcome to the 42 IRC Server Project! This project implements an Internet Relay Chat (IRC) server using C++98. The server is designed to handle multiple clients simultaneously, conforming to standard IRC protocols. This implementation focuses on core IRC functionalities without server-to-server communication and adheres to the specified requirements.

## Features

- **Multi-client Support**: Handles multiple client connections simultaneously.
- **Non-blocking I/O**: Ensures the server remains responsive by using non-blocking I/O operations.
- **Poll-based Event Handling**: Uses `poll()` for efficient event-driven programming.
- **Authentication**: Clients must authenticate using a password upon connection.
- **Channel Management**: Supports joining channels, sending/receiving messages, and channel operator commands.
- **Operator Commands**: Implemented commands for channel operators such as KICK, INVITE, TOPIC, and MODE.

## Installation

### Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/abel-hid/42-IRC.git
   cd 42-IRC

2. **Build the Project**
   ```bash
    make

3. **Run the Server**
    ```bash
    ./ircserv <port> <password>
    ```
    Replace `<port>` with the desired port number and `<password>` with the server password.
4. **Example:**
    ```bash
    ./ircserv 6667 password
    ```
    The server will start listening on the specified port.
5. **Connect to the Server**
    ```bash
    nc localhost <port>
    ```
    Replace `<port>` with the port number specified in step 3.
    **Example:**
    ```bash
    nc localhost 6667
    ```
    You can now interact with the server using the supported IRC commands.

## Commands

The server supports the following IRC commands:
 **User Commands**
- **PASS**: Authenticate with the server.
- **NICK**: Set the user's nickname.
- **USER**: Set the user's username and realname.
- **JOIN**: Join a channel.
- **PART**: Leave a channel.
- **PRIVMSG**: Send a message to a user or channel.
- **QUIT**: Disconnect from the server.
- **TOPIC**: Set the channel topic.
- **KICK**: Kick a user from a channel.
- **INVITE**: Invite a user to a channel.
- **MODE**: Set channel modes.
- **mode flags**
- **o**: Give/take channel operator status.
- **i**: Set/unset invite-only mode.
- **k**: Set/unset channel key (password).
- **l**: Set the user limit for the channel.
- **t**: Set/unset topic protection.

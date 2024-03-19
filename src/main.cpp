/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:22:39 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/19 02:33:44 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#define Welcome "\033[33;1m\n\
░▒▓████████▓▒░▒▓████████▓▒          ░▒▓█▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░  \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        \n\
░▒▓██████▓▒░    ░▒▓█▓▒░    ▒▓███▓▒  ░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░        \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░  \033[0m\n\n\
\t\033[36;1mMade By: \033[0m\033[31mylamsiah \033[0m\033[32m| \033[0m\033[35mabel-hid \033[0m\033[32m|\033[0m\033[34m araiteb \033[0m \n"

int parssing_port(std::string port)
{
	int i = 0;
	if(std::atoi(port.c_str()) > 65535 || std::atoi(port.c_str()) < 1024)
		return (0);
	while (port[i])
	{
		if (!isdigit(port[i]))
			return (0);
		i++;
	}
	return (1);
}

void handl_signal(int signum)
{
	if (signum == SIGINT)
		std::cout << "\n\033[31;1mServer is shutting down...\033[0m" << std::endl;
		exit(0);
}

bool Server::initServer()
{
    // create socket file descriptor
    this->setServer_fd(socket(AF_INET, SOCK_STREAM, 0)); // 0 for TCP and 1 for UDP
    // check if socket is created
    if (this->getServer_fd() < 0)
    {
        std::cout << "Socket failed" << std::endl;
        return (0);
    }
    //scokaddr_in is a structure containing an internet address
    int opt = 1;
    sockaddr_in address;
    // set address family
    address.sin_family = AF_INET; // AF_INET is the address family for IPv4
    address.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY means any address for binding
    address.sin_port = htons(this->getPort()); // htons() converts port number to network byte order
    this->setAddress(address);
    // bind socket to address
    // non blocking
    if(setsockopt(this->getServer_fd(), SOL_SOCKET,SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        std::cout << "Setsockopt failed" << std::endl;
        return (0);
    }
    if (fcntl(this->getServer_fd(), F_SETFL, O_NONBLOCK) < 0)
    {
        std::cout << "Fcntl failed" << std::endl;
        return (0);
    }
    
    if (bind(this->getServer_fd(), (struct sockaddr *)&address, sizeof(this->getAddress())) < 0)
    {
        std::cout << "Bind failed" << std::endl;
        return (0);
    }
    // listen for connections on a socket
    if (listen(this->getServer_fd(), 7000) < 0) // 3 is the maximum size of queue connections
    {
        std::cout << "Listen failed" << std::endl;
        return (0);
    }
    return 1;
}

int Server::accept_client()
{
    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_fd = accept(this->getServer_fd(), (struct sockaddr *)&client_address, &client_address_size);
    if (client_fd < 0) 
    {
        std::cout << "Accept failed" << std::endl;
        return -1;
    }
    this->addClient(client_fd, new Client());
    std::string add = inet_ntoa(client_address.sin_addr);
    this->clients[client_fd]->set_ip_address(add);
    this->pport[client_fd] = ntohs(client_address.sin_port);
    this->setPport(client_fd, ntohs(client_address.sin_port));
    std::cout << "\n[\033[32;1mINFO\033[0m] \033[33;1mNew client connected with ip address\033[0m [\033[32;1m" << add << "\033[0m] \033[33;1mFrom port\033[0m [\033[32;1m" \
    << ntohs(client_address.sin_port) << "\033[0m]" << std::endl;
    return client_fd;
}
std::string update_str(std::string str)
{
    if(str[0] == ':')
    {
        size_t space_pos = str.find(" ");
        if (space_pos != std::string::npos) 
		{
            str = str.substr(space_pos + 1);
        }
		else
		{
			str = "";
		}
    }
    return str;
}

void Server::execute_command(std::string str, int fd)
{
    std::string buffer = str;
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
    if(buffer.empty())
        return ;
    std::vector<std::string> words;
    std::string word;
    std::istringstream iss(buffer);
    while (iss >> word)
    {
        words.push_back(word);
    }
    if(words.size() == 0)
        return ;
	this->commandsIrc(words, str, fd);
}
int Server::receve_msg(int fd)
{
    char buffer[1024] = {0};
    ssize_t bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) 
    {
        // Client disconnected or CTRL+C was pressed
        std::cout << "[\033[31;1mINFO\033[0m] \033[31;1mClient Disconnected\033[0m" << std::endl;
		send(fd, "\033[31;1mYou are Disconnected 😞.\r\n\033[0m", 40, 0);
        this->remove_client_from_channels(fd);
        this->removeClient(fd);
        delete this->getClients()[fd];
        this->clients.erase(fd);
        close(fd);
        return -1;
    } 
    else 
    {
        buffer[bytesRead] = '\0';
        std::string str = buffer;

        size_t newline_pos = str.find_first_of("\r\n");
        if (newline_pos == std::string::npos)
        {
            this->clients[fd]->setStr(str);
            return 1;
        }
        else
        {
            std::string tmp = "";
            if (!this->clients[fd]->getStr().empty())
            {
            for (size_t j = 0; j < this->clients[fd]->getStr().size(); j++)
                tmp += this->clients[fd]->getStr()[j];
                this->clients[fd]->clearStr();
            }
            str = tmp + str;
            str  = update_str(str);
            std::string tmp1 = str;
            std::string up = tmp1.substr(0, tmp1.find_first_of(" "));
            for (int i = 0 ; up[i] ; i++)
                up[i] = toupper(up[i]);
            up.erase(std::remove(up.begin(), up.end(), '\n'), up.end());
            up.erase(std::remove(up.begin(), up.end(), '\r'), up.end());
            std::cout << "Received : <<" << tmp1.erase(tmp1.find_first_of("\r\n")) << ">>" << std::endl;
            std::cout << "[\033[33;1mCMD\033[0m] \033[32;1m" << up << "\033[0m \033[33;1mFrom\033[0m [\033[32;1m" << this->get_ip_address(fd) \
            << "\033[0m]\033[0m \033[33;1mFrom port\033[0m [\033[32;1m" << this->getPport(fd) << "\033[0m]" << std::endl;
            if (str.find_first_of("\r\n") != std::string::npos && str != "\n")
            {
                size_t pos = str.find_last_of("\r\n");
                str = str.substr(0, pos);
                execute_command(str, fd);
                return 1;
            }
        }
    }
    return 1;

}

int main(int ac, char **av)
{
    if (ac != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]" << std::endl;
		return 0;
	}
	if (!parssing_port(av[1]))
	{
		std::cout << "Invalid port" << std::endl;
		return 0;
	}
	signal(SIGINT, handl_signal);
    Server server;
    server.setPort(atoi(av[1]));
    std::vector<pollfd> fds;
    
    if (!server.initServer())
        return (0);
    
    fds.push_back(pollfd());
    fds.back().fd = server.getServer_fd();
    fds.back().events = POLLIN;
    while (true) 
    {
        int ready = poll(&fds[0], fds.size(), 0);
        if (ready == -1) 
        {
            std::cout << "Poll failed" << std::endl;
            break;
        }
        for (size_t i = 0; i < fds.size(); ++i) 
        {
            if (fds[i].revents & POLLIN) 
            {
                if (fds[i].fd == server.getServer_fd())
                {
                    // New connection
                    int client_fd = server.accept_client();
                    if (client_fd > 0) 
                    {
        std::cout << "Waiting for incoming connections..." << std::endl;
                        fds.push_back(pollfd());
                        fds.back().fd = client_fd;
                        fds.back().events = POLLIN;
                    }
                    else
                        continue;
                } 
                else 
                {
                    // Incoming data
                    if (server.receve_msg(fds[i].fd) == -1)
                    {
                        fds.erase(fds.begin() + i);
                        i--;
                    }
                }
            }
        }
    }
    return (0);
}

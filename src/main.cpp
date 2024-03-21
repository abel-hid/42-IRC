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

bool Server::initServer() 
{
    // Create socket file descriptor
    this->setServer_fd(socket(AF_INET, SOCK_STREAM, 0));
    // Check if socket is created
    if (this->getServer_fd() == -1)
    {
        std::cerr << "Socket failed" << std::endl;
        return false;
    }
    int opt = 1;
    struct sockaddr_in address;
    // Set address family
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->getPort());
    this->setAddress(address);
    // Set socket options
    if (setsockopt(this->getServer_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        std::cerr << "Setsockopt failed" << std::endl;
        close(this->getServer_fd());
        return false;
    }
    // Set non-blocking mode
    if (fcntl(this->getServer_fd(), F_SETFD, O_NONBLOCK) < 0)
    {
        std::cerr << "Fcntl failed" << std::endl;
        close(this->getServer_fd());
        return false;
    }
    // Bind socket to address
    if (bind(this->getServer_fd(), reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(this->getServer_fd());
        return false;
    }
    // Listen for connections on a socket
    if (listen(this->getServer_fd(), 5000) < 0) 
    {
        std::cerr << "Listen failed" << std::endl;
        close(this->getServer_fd());
        return false;
    }
    return true;
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
    fcntl(client_fd, F_SETFD, O_NONBLOCK);
    this->addClient(client_fd, new Client(client_fd));
    std::string add = inet_ntoa(client_address.sin_addr);
    this->clients[client_fd]->set_ip_address(add);
    this->pport[client_fd] = ntohs(client_address.sin_port);
    this->setPport(client_fd, ntohs(client_address.sin_port));
    std::cout << "\n[\033[32;1mINFO\033[0m] \033[33;1mNew client connected with ip address\033[0m [\033[32;1m" << add << "\033[0m] \033[33;1mFrom port\033[0m [\033[32;1m" \
    << ntohs(client_address.sin_port) << "\033[0m]" << std::endl;
    return client_fd;
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
    words.clear();
    buffer.clear();
    word.clear();
}

int Server::receve_msg(int fd)
{
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(fd, buffer, sizeof(buffer) -1, 0);
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
        std::cout << "Received: " << buffer << std::endl;
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
            if (str.empty())
                return 1;
            std::string tmp1 = str;
            std::string up = tmp1.substr(0, tmp1.find_first_of(" "));
            for (int i = 0 ; up[i] ; i++)
                up[i] = toupper(up[i]);
            up.erase(std::remove(up.begin(), up.end(), '\n'), up.end());
            up.erase(std::remove(up.begin(), up.end(), '\r'), up.end());
            std::cout << "[\033[33;1mCMD\033[0m] \033[32;1m" << up << "\033[0m \033[33;1mFrom\033[0m [\033[32;1m" << this->get_ip_address(fd) \
            << "\033[0m]\033[0m \033[33;1mFrom port\033[0m [\033[32;1m" << this->getPport(fd) << "\033[0m]" << std::endl;
            if (str.find_first_of("\r\n") != std::string::npos)
            {
                size_t pos = str.find_last_of("\r\n");
                str = str.substr(0, pos);
                this->execute_command(str, fd);
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
		std::cout << "\033[31;1mUsage: ./ircserv [port] [password]\033[0m 🤬." << std::endl;
		return 0;
	}
	if (!parssing_port(av[1]))
	{
		std::cout << "\033[31;1mInvalid port\033[0m 🤬." << std::endl;
		return 0;
	}
    if (!isValidPassword(av[2]))
    {
        std::cout << "\033[31;1mInvalid password\033[0m 🤬." << std::endl;
        return 0;
    }
	signal(SIGINT, handl_signal);
    signal(SIGPIPE, SIG_IGN);
    std::cout << Welcome << std::endl;
	std::cout<< "\t[\033[32;1mINFO\033[0m] \033[32;1mServer is running on port \033[0m[\033[32;1m" << av[1] << "\033[0m]\n" << std::endl;
    Server server;
    server.setPort(atoi(av[1]));
    server.setServerPassword(av[2]);
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

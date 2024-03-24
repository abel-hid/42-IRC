/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Creat_Server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:56:43 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/24 23:01:32 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool Server::initServer() 
{
    this->setServer_fd(socket(AF_INET, SOCK_STREAM, 0));
    if (this->getServer_fd() == -1)
    {
        std::cerr << "Socket failed" << std::endl;
        return false;
    }
    int opt = 1;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->getPort());
    this->setAddress(address);
    if (setsockopt(this->getServer_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        std::cerr << "Setsockopt failed" << std::endl;
        close(this->getServer_fd());
        return false;
    }
    if (fcntl(this->getServer_fd(), F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "Fcntl failed" << std::endl;
        close(this->getServer_fd());
        return false;
    }
    if (bind(this->getServer_fd(), reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(this->getServer_fd());
        return false;
    }
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
    if(fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "Fcntl failed" << std::endl;
        close(client_fd);
        return -1;
    }
    this->addClient(client_fd, new Client(client_fd));
    std::string add = inet_ntoa(client_address.sin_addr);
    this->clients[client_fd]->set_ip_address(add);
    this->setPport(client_fd, ntohs(client_address.sin_port));
    std::cout << "\n[\033[32;1mINFO\033[0m] \033[33;1mNew client connected with ip address\033[0m [\033[32;1m" << add << \
    "\033[0m] \033[33;1mFrom port\033[0m [\033[32;1m" << ntohs(client_address.sin_port) << "\033[0m]" << std::endl;
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
        std::cout << "[\033[31;1mINFO\033[0m] \033[31;1mClient Disconnected\033[0m" << std::endl;
		send(fd, "\033[31;1mYou are Disconnected 😞.\r\n\033[0m", 40, 0);
        this->remove_client_from_channels(fd);
        delete this->clients[fd];
        this->removeClient(fd);
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

void Server::close_all()
{
    std::map<int, Client *>::iterator it = this->clients.begin();
    while (it != this->clients.end())
    {
        close(it->first);
        it++;
    }
    close(this->getServer_fd());
}

void Server::runServer(bool &close_ser)
{
    std::vector<pollfd> fds;

    fds.push_back(pollfd());
    fds.back().fd = this->getServer_fd();
    fds.back().events = POLLIN;
    while (!close_ser) 
    {
        int ready = poll(&fds[0], fds.size(), 0);
        if (ready == -1) 
        {
            this->close_all();
            break;
        }
        for (size_t i = 0; i < fds.size(); ++i) 
        {
            if (fds[i].revents & POLLIN) 
            {
                if (fds[i].fd == this->getServer_fd())
                {
                    int client_fd = this->accept_client();
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
                    if (this->receve_msg(fds[i].fd) == -1)
                    {
                        fds.erase(fds.begin() + i);
                        i--;
                    }
                }
            }
        }
    }
    close(this->getServer_fd());
}

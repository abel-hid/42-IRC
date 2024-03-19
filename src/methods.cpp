/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 12:39:59 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/19 01:49:33 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::string Server::to_string(int number)
{
    std::stringstream ss;
    ss << number;
    return (ss.str());
}
int Server::is_nickname_exist_and_registered(int fd, std::string nickname)
{
    std::map<int, Client *>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->second->getNickname() == nickname && it->second->is_Registered() && it->first != fd)
            return (1);
    }
    return (0);
}

std::string Server::get_hostnames()
{
    char buffer[1024] = {0};
    std::string hostnames = "";
    FILE *stream = popen("hostname", "r");
    if (stream) 
    {
        if (fgets(buffer, sizeof(buffer), stream) != NULL) 
        {
            hostnames = buffer;
        }
        pclose(stream);
    }
    hostnames.erase(std::remove(hostnames.begin(), hostnames.end(), '\n'), hostnames.end());
    return (hostnames);
}

std::string Server::get_nickname(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getNickname());
    return ("");
}

void Server::set_nickname(int fd, std::string nickname)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setNickname(nickname);
}

std::map<std::string, Channel *> &Server::getChannels()
{
    return (this->channels);
}

long Server::get_limit(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        return (channels[channel]->getLimit());
    return (-1);
}

int Server::get_fd_users(const std::string& nickname) const
{
    std::map<int, Client *>::const_iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return (it->first);
    }
    return (-1);
}
std::string Server::get_hostname(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getHostname());
    return ("");
} 
int Server::is_nickname_exist_and_registered(std::string nickname)
{
    std::map<int, Client *>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->second->getNickname() == nickname && it->second->is_Registered())
            return (1);
    }
    return (0);
}

void Server::set_topic(std::string channel, std::string topic)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        channels[channel]->set_topic(topic);
}

std::string Server::get_topic(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        return (channels[channel]->get_topic());
    return ("No topic is set");
}

void Server::priny_users(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        channels[channel]->print_users();
}


bool Server::getInviteToChannel(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getInviteToChannel());
    return (false);
}

std::string Server::get_username(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getUsername());
    return ("");
}
std::string Server::get_realname(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getRealname());
    return ("");
}


std::string Server::get_servername(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getServername());
    return ("");
}

std::string Server::get_current_time()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return (buf);
}

int Server::is_nickname_exist(std::string nickname)
{
    std::map<int, Client *>::iterator it;
    for (it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return (1);
    }
    return (0);
}

void Server::set_is_registered(int fd, int is_registered)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        it->second->set_is_Registered(is_registered);
}
int Server::is_registered(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (it->second->is_Registered());
    return (0);
}

void Server::addChannel(std::string name, Channel *channel)
{
    this->channels[name] = channel;
}
void Server::set_creator(std::string channel, bool creater)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        channels[channel]->set_creater(creater);
}

std::string Server::get_creator_name(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
        return (channels[channel]->get_creator_name());
    return ("");
}

void Server::set_fd(int fd, int new_fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setFd(new_fd);
}
void Server::set_password(int fd, std::string password)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        it->second->setPassword(password);
}
void Server::set_client(int fd, Client *client)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd] = client;
}
void Server::set_clients(std::map<int, Client *> clients)
{
    this->clients = clients;
}
void Server::remove_client_from_channels(int fd)
{
    std::string str =  ":" + this->get_nickname(fd) + "!" + this->get_username(fd) + "@" + this->get_ip_address(fd) + " QUIT : QUIT Leaving...\r\n";
    this->send_to_all(str, fd);
    std::map<std::string, Channel *>::iterator it;
    for (it = this->channels.begin(); it != this->channels.end(); it++)
    {
        it->second->quit_channel(this->get_nickname(fd));
    }
    it = this->channels.begin();
    while(it != this->channels.end())
    {
        if (it->second->getUsers().size() == 0)
        {
            delete it->second;
            this->channels.erase(it++);
        }
        else
        {
            it++;
        }
    }
}

void Server::set_username(int fd, std::string username)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setUsername(username);
}
void Server::set_realname(int fd, std::string realname)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setRealname(realname);
}
void Server::set_hostname(int fd, std::string hostname)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setHostname(hostname);
}
void Server::set_servername(int fd, std::string servername)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        clients[fd]->setServername(servername);
}
std::string Server::get_password(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (clients[fd]->getPassword());
    return ("");
}
std::map<int, Client *> &Server::getClients()
{
    return (this->clients);
}
void Server::addClient(int fd, Client *client)
{
    this->clients[fd] = client;
}
void Server::removeClient(int fd)
{
    this->clients.erase(fd);
}

void Server::setClients(std::map<int, Client *> clients)
{
    this->clients = clients;
}
std::string Server::get_users(std::string channel)
{
    std::map<std::string, Channel *>::iterator it;
    it = this->channels.find(channel);
    if(it != this->channels.end())
    {
        std::set<std::string>::iterator it = channels[channel]->getUsers().begin();
        std::string users;
        while(it != channels[channel]->getUsers().end())
        {
            users += *it + " ";
            it++;
        }
        return (users);
    }
    return ("");
}

bool	Server::IsAuthorized(Client& client) 
{
	if (client.getNickname().empty() || client.getPassword().empty() || client.getUsername().empty())
		return 0;
	return 1;
}
bool Server::is_authenticated(Client &client)
{
    if (client.getNickname().empty() || client.getPassword().empty() || client.getUsername().empty())
        return 0;
    return 1;
}

Client*	Server::getClientByFd(int fdUser)
{
	Client *c;
	if (this->clients.find(fdUser) != this->clients.end())
	{
		c = this->clients.find(fdUser)->second;
		return c;
	}
	return NULL;
}

Client*	Server::getClientByNickname(std::string nickname)
{
	std::map<int , Client *>::iterator it;

	for(it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
	}
	return NULL;
}

std::string Server::get_ip_address(int fd)
{
    std::map<int, Client *>::iterator it;
    it = this->clients.find(fd);
    if(it != this->clients.end())
        return (this->clients[fd]->getipaddress());
    return ("");
}
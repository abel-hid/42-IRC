/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 00:33:02 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/19 00:46:26 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
    this->fd = 0;
    this->password = "";
    this->nickname = "";
    this->username = "";
    this->realname = "";
    this->hostname = "";
    this->servername = "";
    this->InviteToChannel = false;
    this->ip_address = "";
    this->is_registered = 0;
}

Client::~Client()
{
}

std::string Client::getPassword()
{
    return (this->password);
}

std::string Client::getNickname()
{
    return (this->nickname);
}

std::string Client::getUsername()
{
    return (this->username);
}

std::string Client::getRealname()
{
    return (this->realname);
}

int Client::getFd()
{
    return (this->fd);
}

std::string Client::getHostname()
{
    return (this->hostname);
}

std::string Client::getServername()
{
    return (this->servername);
}

std::string Client::getipaddress()
{
    return (this->ip_address);
}

void Client::set_ip_address(std::string ip)
{
    this->ip_address = ip;
}

void Client::clearStr()
{
    this->str.clear();
}

void Client::setStr(std::string str)
{
    this->str.push_back(str);
}

std::vector<std::string> Client::getStr()
{
    return (this->str);
}

void Client::setNickname(std::string nickname)
{
    this->nickname = nickname;
}

void Client::setServername(std::string servername)
{
    this->servername = servername;
}

void Client::setRealname(std::string realname)
{
    this->realname = realname;
}

void Client::setUsername(std::string username)
{
    this->username = username;
}

void Client::setPassword(std::string password)
{
    this->password = password;
}

void Client::setFd(int fd)
{
    this->fd = fd;
}

void Client::setHostname(std::string hostname)
{
    this->hostname = hostname;
}

int Client::is_Registered()
{
    return (this->is_registered);
}

void Client::set_is_Registered(int is_registered)
{
    this->is_registered = is_registered;
}

void Client::setInviteToChannel(bool InviteToChannel)
{
    this->InviteToChannel = InviteToChannel;
}

bool Client::getInviteToChannel()
{
    return (this->InviteToChannel);
}
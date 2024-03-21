/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 00:33:02 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/21 02:35:13 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

// CONSTRUCTOR & DESTRUCTOR

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

Client::Client(int fd)
{
    this->fd = fd;
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

Client::~Client() {}

// GETTERS FUNCTIONS

int Client::getFd() { return (this->fd); }

std::string Client::getHostname() { return (this->hostname); }

std::string Client::getPassword() { return (this->password); }

std::string Client::getNickname() { return (this->nickname); }

std::string Client::getUsername() { return (this->username); }

std::string Client::getRealname() { return (this->realname); }

std::vector<std::string> Client::getStr() { return (this->str); }

std::string Client::getipaddress() { return (this->ip_address); }

std::string Client::getServername() { return (this->servername); }

bool Client::getInviteToChannel() { return (this->InviteToChannel); }

// SETTERS FUNCTIONS

void Client::setFd(int fd) { this->fd = fd; }

void Client::setStr(std::string str) { this->str.push_back(str); }

void Client::set_ip_address(std::string ip) { this->ip_address = ip; }

void Client::setNickname(std::string nickname) { this->nickname = nickname; }

void Client::setRealname(std::string realname) { this->realname = realname; }

void Client::setUsername(std::string username) { this->username = username; }

void Client::setPassword(std::string password) { this->password = password; }

void Client::setHostname(std::string hostname) { this->hostname = hostname; }

void Client::setServername(std::string servername) { this->servername = servername; }

void Client::set_is_Registered(int is_registered) { this->is_registered = is_registered; }

void Client::setInviteToChannel(bool InviteToChannel) { this->InviteToChannel = InviteToChannel; }

// UTILS FUNCTIONS

void Client::clearStr() { this->str.clear(); }

int Client::is_Registered() { return (this->is_registered); }

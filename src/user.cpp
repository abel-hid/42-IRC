/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:01:59 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/19 01:45:55 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::cmduser(Client *c, std::vector<std::string> &words , std::string str)
{
    
	if (words.size() < 5)
	{
        std::string userMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " USER :Not enough parameters\r\n";
        send(c->getFd(), userMsg.c_str(), userMsg.length(), 0);
        return ;
    }
    else if (words.size() > 5 && words[4].at(0) != ':')
    {
        std::string userMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " USER :Not enough parameters\r\n";
        send(c->getFd(), userMsg.c_str(), userMsg.length(), 0);
        return ;
    }
    c->setUsername(words[1]);
    c->setHostname(words[2]);
    c->setServername(words[3]);
    std::string realname;
    if(words.size() > 5 && words[4].at(0) == ':')
    {
        str = str.erase(0, str.find(words[0]) + words[0].length() + 1);
        str = str.erase(0, str.find(words[1]) + words[1].length() + 1);
        str = str.erase(0, str.find(words[2]) + words[2].length() + 1);
        str = str.erase(0, str.find(words[3]) + words[3].length() + 1);
        realname = str.erase(0,1);
    }
    else
        realname = words[4];
    c->setRealname(realname);
    if (this->is_nickname_exist_and_registered(c->getFd() , c->getNickname()))
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NICKNAMEINUSE) + " " + c->getNickname() + " NICK :Nickname is already in use\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        c->setNickname("");
        return ;
    }
	if (this->IsAuthorized(*c) && c->getPassword() == this->server_password)
    {
        std::string str;
        c->set_is_Registered(1);
        str = ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n";
        send(c->getFd(), str.c_str(), str.length(), 0);
        str = ":" + this->get_hostnames() +  " 001 " + this->get_nickname(c->getFd()) + " :Welcome to the Internet Relay Network " + this->get_nickname(c->getFd())+ "!" + this->get_username(c->getFd()) + "@" + this->get_hostnames() + "\r\n";
        send(c->getFd(), str.c_str(), str.length(), 0);            
        str = ":" + this->get_hostnames() +  " 003 " + this->get_nickname(c->getFd()) + " :This server was created " +  this->get_current_time() + "\r\n";
        send(c->getFd(), str.c_str(), str.length(), 0);
    }
    else if(c->getPassword() != this->server_password && this->IsAuthorized(*c))
    {
        std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " " + "PASS" + " :Password incorrect\r\n";
        send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
        c->setPassword("");
        return ;
    }
}

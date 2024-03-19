/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:01:29 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/19 01:47:31 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::cmdpass(std::vector<std::string>& words, Client *c , std::string str)
{
	if (words.size() < 2 || (words.size() > 2 && words[1].at(0) != ':'))
	{
        std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " PASS :Not enough parameters\r\n";
        send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
        return ;
    }
	else 
    {
        std::string pass;
        if (c->getNickname() == "Bot" && words[2].compare(this->server_password))
        {
            std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " PASS :Password incorrect\r\n";
            send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
            return ;
        }
        if(words.size() > 2 && words[1].at(0) == ':')
        {
            pass = str.substr(str.find(":") + 1 , str.length());
        }
        else
            pass = words[1];
		c->setPassword(pass);
        if(c->getPassword() != this->server_password)
        {
            std::string passMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_PASSWDMISMATCH) + " PASS :Password incorrect\r\n";
            send(c->getFd(), passMsg.c_str(), passMsg.length(), 0);
            c->setPassword("");
            return ;
        }
	}
}

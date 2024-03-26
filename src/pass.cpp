/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:01:29 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/26 00:09:42 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::cmdpass(std::vector<std::string>& words, Client *c , std::string str)
{
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
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
        if(words.size() >= 2 && words[1].at(0) == ':' && (this->server_password.find(":") == std::string::npos))
        {
            str = str.erase(0, str.find(words[0]) + words[0].length() + 1);
            str.erase(0 ,str.find_first_of(":") + 1);
            pass = str;
        }
        else if(words.size() > 2 && words[1].at(0) == ':' )
        {
            str = str.erase(0, str.find(words[0]) + words[0].length() + 1);
            str.erase(0 ,str.find_first_of(":") + 1);
            pass = str;
        }
        else if(words.size() >= 2 && words[1].at(0) == ':' && (this->server_password.find(":") != std::string::npos))
        {
            pass = words[1];
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

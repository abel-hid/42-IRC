/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:00:56 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/25 00:23:52 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::send_nick(Client *c, std::string nickMsg)
{
    std::map<std::string, Channel*>::iterator it = this->getChannels().begin();
    std::set<int> fds; 
    int client_fd = c->getFd();
    for (; it != this->getChannels().end(); ++it)
    {
        std::set<std::string>& users = it->second->getUsers();
        for (std::set<std::string>::iterator it2 = users.begin(); it2 != users.end(); ++it2)
        {
            int user = this->get_fd_users(*it2);
            if (user != client_fd && fds.find(user) == fds.end())
            {
                send(user, nickMsg.c_str(), nickMsg.length(), 0);
                fds.insert(user);
            }
        }
    }
}

void Server::users_update(std::string old_nick, std::string new_nick, std::map<std::string, Channel*> channels)
{
    std::map<std::string, Channel*>::iterator it = channels.begin();
    for (; it != channels.end(); ++it)
    {
        std::set<std::string>& users = it->second->getUsers();
        if (users.find(old_nick) != users.end())
        {
            users.erase(old_nick);
            users.insert(new_nick);
        }
        std::set<std::string>& ops = it->second->getOperators();
        if (ops.find("@" + old_nick) != ops.end())
        {
            ops.erase("@" + old_nick);
            ops.insert("@" + new_nick);
        }
    }
}

void Server::nickCmd1(std::string msg, Client *c)
{
    std::string buffer = msg;
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
    for (int i = 0 ; words[0][i] ; i++)
		words[0][i] = toupper(words[0][i]);
    if (!words[0].compare("NICK") && words.size() < 2)
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " NICK :No nickname given\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    else if (!words[0].compare("NICK") && words.size() == 2 && words[1] == ":")
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " NICK :Erroneous nickname\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    else if (!words[0].compare("NICK") && words.size() > 1 && c->getNickname() != words[1])
    {
        std::size_t found = words[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_[]\\`^{}");
        if (found != std::string::npos)
        {
            std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " " + words[0] + " :Erroneous nickname\r\n";
            send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
            return ;
        }
        std::string nickMsg;
        Client *tmpClient = this->getClientByNickname(words[1]);
        if (tmpClient)
            nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NICKNAMEINUSE) + " " + c->getNickname() + " " + words[1] + " :Nickname is already in use\r\n";
        else
        {
            nickMsg = ":" + c->getNickname() + "!" + c->getUsername() + "@" + c->getHostname() + " NICK " + words[1] + "\r\n";
            this->users_update(c->getNickname(), words[1], this->getChannels());
            c->setNickname(words[1]);
        }
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        this->send_nick(c, nickMsg);
        return ;
    }
    else if (words.size() > 2)
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " NICK :Not enough parameters\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
}

void	Server::cmdknick(std::vector<std::string> &words, Client *c)
{
    Client *tmpClient;
    if (words.size() != 2)
    {
        std::string nickMsg;
        if (words.size() < 2)
            nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NONICKNAMEGIVEN) + " " + words[0] +  " :No nickname given\r\n";
        else if (words.size() > 2)
            nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " " + words[0] + " :Erroneous nickname\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    std::size_t found = words[1].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_[]\\`^{}");
    if (found != std::string::npos)
    {
        std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ERRONEUSNICKNAME) + " " + words[0] + " :Erroneous nickname\r\n";
        send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
        return ;
    }
    if (words.size() == 2)
    {
        tmpClient = this->getClientByNickname(words[1]);
        if ((tmpClient && tmpClient->getFd() != c->getFd() && this->IsAuthorized(*tmpClient)) || (tmpClient && !words[1].compare("Bot")))
        {
            std::string nickMsg;
            if (!words[1].compare("Bot"))
                nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NICKNAMEINUSE) + " " + "Bot" + " " + words[0] + " :Nickname is already in use\r\n";
            else
                nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NICKNAMEINUSE) + " " + c->getNickname() + " " + words[0] + " :Nickname is already in use\r\n";
            send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
            return ;
        }
        
        c->setNickname(words[1]);
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
    }
}

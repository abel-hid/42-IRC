/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 03:00:11 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/21 02:55:49 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::join_topic_part_kick_privmsg(int fd , std::string str)
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
    for (int i = 0 ; words[0][i] ; i++)
		words[0][i] = toupper(words[0][i]);
    if(words[0] == "JOIN")
        join_command(words, fd , buffer);
    else if(words[0] == "PRIVMSG")
        privmsg_command(words, fd, buffer);
    else if(words[0] == "TOPIC")
        topic_command(words, fd , buffer);
    else if(words[0] == "PART")
        part_command(words, fd, buffer);
    else if(words[0] == "KICK")
        kick_command(words, fd, buffer);
    iss.clear();
    words.clear();
    buffer.clear();
}

void	Server::commandsIrc(std::vector <std::string> &words, std::string str, int fd)
{
    Client *c ;
    c = getClientByFd(fd);
    if (!c)
        return ;
    for (int i = 0 ; words[0][i] ; i++)
		words[0][i] = toupper(words[0][i]);
    
    if (!this->IsAuthorized(*c))
    {
        if (words[0] == "PASS")
            cmdpass(words, c ,str);
        else if (words[0] == "NICK" && c->getPassword() ==  this->server_password)
            cmdknick(words, c );
        else if (words[0] == "USER" && c->getPassword() ==  this->server_password)
            cmduser(c, words , str);
        else 
        {
            std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOTREGISTERED) + " " + words[0] + " :You have not registered\r\n";
            send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
            return ;
        }
    }
    else if (this->IsAuthorized(*c))
    {
        if (!words[0].compare("LEVEL"))
        {
            send_to_user("Bot", str, c->getFd());
        }
        else if (!words[0].compare("QUIT"))
        {
            this->quit_command(c->getFd());
            return ;
        }
        else if (!words[0].compare("PASS") || !words[0].compare("USER"))
        {
            std::string nickMsg = ":" + this->get_hostnames() + " " + this->to_string(ERR_ALREADYREGISTRED) + " " + c->getNickname() + " :You may not reregister\r\n";
            send(c->getFd(), nickMsg.c_str(), nickMsg.length(), 0);
            return ;
        }
        else if (!words[0].compare("NICK"))
        {
            nickCmd1(str, c);
            return ;
        }
        else if (!words[0].compare("PONG"))
        {
            return ;
        }
        else if (!words[0].compare("TIME"))
            send_to_user("Bot", str, c->getFd());
        else if (!words[0].compare("HELP"))
            send_to_user("Bot", str, c->getFd());
        else if (!words[0].compare("🤖:"))
        {
            comdBotBot(str);
        }
        else if(!words[0].compare("JOIN") || !words[0].compare("TOPIC") || !words[0].compare("PART") 
            || !words[0].compare("KICK") || !words[0].compare("PRIVMSG"))
        {
            join_topic_part_kick_privmsg(c->getFd(), str);
        }
        else if (!words[0].compare("INVITE"))
            this->invitecmd(str, c->getFd());
        else if (!words[0].compare("MODE"))
            this->modecmd(str, c->getFd());
    }
}



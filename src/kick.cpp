/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 00:25:47 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/25 22:08:12 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::kick_command(std::vector<std::string > words  , int fd , std::string str)
{

    if(words.size() == 1)
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return ;
    }
    else
    {
        this->KickChannel(words, this->getChannels(), fd, this->get_nickname(fd), str);
    }
}

void Server::KickChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname , std::string str)
{
    // Check if the command has enough parameters
    if (strs.size() < 3)
    {
        std::string error_message = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + this->get_nickname(fd) + " KICK :Not enough parameters\r\n";
        send(fd, error_message.c_str(), error_message.length(), 0);
        return;
    }
    else if(strs.size() > 4 && strs[3].at(0) != ':')
    {
        std::string error_message = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + this->get_nickname(fd) + " KICK :Not enough parameters\r\n";
        send(fd, error_message.c_str(), error_message.length(), 0);
        return;
    }


    std::vector<std::string> kick_channel;
    std::vector<std::string> users;
    std::string reason;
    std::string message;
    
    if(strs[1].find(',') != std::string::npos) 
    {
        std::stringstream ss(strs[1]);
        std::string token;
        while (std::getline(ss, token, ',')) 
        {
            kick_channel.push_back(token);
        }
        token.clear();
        ss.clear();
    } 
    else 
    {
        kick_channel.push_back(strs[1]);
    }
    
    if(strs[2].find(',') != std::string::npos) 
    {
        std::stringstream ss(strs[2]);
        std::string token;
        while (std::getline(ss, token, ',')) 
        {
            users.push_back(token);
        }
        token.clear();
        ss.clear();
    } 
    else 
    {
        users.push_back(strs[2]);
    }
    reason.clear();
    reason = "";
    if(strs.size() > 3)
    {
        if(strs.size() > 3 && strs[3].at(0) == ':')
        {
            std::cout << "---------------------1----------------\n";
            str = str.erase(0, str.find(strs[0]) + strs[0].length() + 1);
            str = str.erase(0, str.find(strs[1]) + strs[1].length() + 1);
            str = str.erase(0, str.find(strs[2]) + strs[2].length() + 1);
            reason = str.erase(0,1);
        }
        else
        {
            reason = strs[3];
        }
    }
    // Check if the channel exists
    for(std::vector<std::string>::iterator it = kick_channel.begin(); it != kick_channel.end(); it++)
    {
        if (channels.find(*it) == channels.end())
        {
            std::string error_message = ":" + this->get_hostnames() + " 403 " + nickname + " " + *it + " :No such channel\r\n";
            send(fd, error_message.c_str(), error_message.length(), 0);
            continue;
        }
        // Check if the user is in the channel
        if(this->isClientInChannel(nickname, *it, channels) == false)
        {
            std::string error_message = ":" + this->get_hostnames() + " 442 " + nickname + " " + *it + " :You're not on that channel\r\n";
            send(fd, error_message.c_str(), error_message.length(), 0);
            continue;
        }
        
        for(std::vector<std::string>::iterator it1 = users.begin(); it1 != users.end(); it1++)
        {
            // Check if the user is an operator and in the channel
            if (channels[*it]->getUsers().find(*it1) == channels[*it]->getUsers().end() && this->isClientExist(*it1))
            {
                std::string error_message = ":" + this->get_hostnames() + " 442 "  + *it + " :User " + *it1 + " is not on that channel\r\n";
                send(fd, error_message.c_str(), error_message.length(), 0);
                continue;
            }
            else if (!this->isClientExist(*it1))
            {
                std::string error_message = ":" + this->get_hostnames() + " 401 " + nickname + " " + *it1 + " :No such nick\r\n";
                send(fd, error_message.c_str(), error_message.length(), 0);
                continue;
            }
            // Check if the command issuer is an operator
            if (!channels[*it]->isOperator("@" + this->get_nickname(fd)))
            {
                std::string error_message = ":" + this->get_hostnames() + " 482 " + nickname + " " + *it + " :You're not a channel operator\r\n";
                send(fd, error_message.c_str(), error_message.length(), 0);
                continue;
            }
            if(reason.empty())
                message = ":" + nickname + "!" + this->get_username(fd) + "@" + this->get_ip_address(fd) + " KICK " + *it + " " + *it1 + "\r\n";
            else
                message = ":" + nickname + "!" + this->get_username(fd) + "@" + this->get_ip_address(fd) + " KICK " + *it + " " + *it1 + " :" + reason + "\r\n";

            // Send the message to the user
            std::set<std::string>::iterator it2 = channels[*it]->getUsers().begin();
            while (it2 != channels[*it]->getUsers().end())
            {
                send(this->get_fd_users(*it2), message.c_str(), message.length(), 0);
                it2++;
            }
            // Remove the user from the channel
            channels[*it]->removeUser(*it1);
            if(channels[*it]->get_creater() == true)
                channels[*it]->set_creater(false);
            // if operator remove him from the operators
            if (channels[*it]->isOperator("@" + *it1))
                channels[*it]->removeOperator("@" + *it1);
            // if channel is empty remove it
            if (channels[*it]->getUsers().empty())
            {
                delete channels[*it];
                channels.erase(*it);
            }
        }
    }
    
    kick_channel.clear();
    users.clear();
    message.clear();
    reason.clear();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 01:40:02 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/25 22:42:30 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::join_command(std::vector<std::string > words  , int fd , std::string str)
{
    if(words.size() == 1)
    {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + words[0] + " :Not enough parameters\r\n";
            send(fd, str.c_str(), str.length(), 0);
            return ;
    }
    else
    {
        if(this->JoinChannel(words, this->get_nickname(fd), fd , str) == -1)
        {
            std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NOSUCHCHANNEL) + " " + this->get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, str.c_str(), str.length(), 0);
        }
    }
}

void Server::join_broadcast_msg(std::map<std::string, Channel*>& channels , std::string msg , std::string channelName)
{
    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it != channels.end()) 
    {
        std::set<std::string>::iterator it2 = it->second->getUsers().begin();
        while (it2 != it->second->getUsers().end()) 
        {
            send(this->get_fd_users(*it2), msg.c_str(), msg.length(), 0);
            it2++;
        }
    }
}

int Server::public_channel(std::string channel_name , std::string key , int fd)
{
    std::map<std::string, Channel*>::iterator it2 = channels.find(channel_name);
    if (it2 != channels.end() && it2->second->isUser(this->get_nickname(fd))) 
    {
        // User is already in the channel
        std::string msg = ":" + this->get_hostnames() + " 443 " + this->get_nickname(fd) + " " + channel_name + " :is already on channel\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return (1);
    }
    // Check if the user is invited to the channel and the channel is invite only
    if(it2 != channels.end() && it2->second->getInviteOnly() == true && !it2->second->is_invited_user(this->get_nickname(fd)))
    {
        std::string msg = ":" + this->get_hostnames() + " 473 " + this->get_nickname(fd) + " " + channel_name + " :Cannot join channel (+i)\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return (1);
    }
    // limit the number of channels a user can join
    if(this->get_limit(channel_name) != -1 && (size_t)this->get_limit(channel_name) <= channels[channel_name]->getUsers().size())
    {
        std::string msg = ":" + this->get_hostnames() + " 471 " + this->get_nickname(fd) + " " + channel_name + " :Cannot join channel (+l)\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        return (1);
    }
    if (it2 == channels.end()) 
    {
        // Channel doesn't exist, create it
        Channel* newChannel = new Channel(channel_name); // Create a new channel
        newChannel->addUser(this->get_nickname(fd)); // Add the user to the channel
        newChannel->addOperator("@" + this->get_nickname(fd)); // Make the user an operator with '@' prefix
        newChannel->set_creater(true);
        if(!key.empty())
            newChannel->setChannelKey(key); // Set the channel key
        channels[channel_name] = newChannel; // Add the channel to the map
        // reply to the user
        std::string msg = ":" + this->get_nickname(fd) + "!" + this->get_username(fd) + "@" + this->get_ip_address(fd) + " JOIN " + channel_name + "\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        msg = ":" + this->get_hostnames() + " 313 " + this->get_nickname(fd) + " " + channel_name + " :" + this->get_topic(channel_name) + "\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        
        msg = ":" + this->get_hostnames() + " " + this->to_string(RPL_NAMREPLY) + " " + this->get_nickname(fd) + " = " + channel_name + " :@" + this->get_nickname(fd) + "\r\n"; // Prefix '@' to operator's name
        send(fd, msg.c_str(), msg.length(), 0);
        msg = ":" + this->get_hostnames() + " " + this->to_string(RPL_ENDOFNAMES) + " " + this->get_nickname(fd) + " " + channel_name + " :End of /NAMES list\r\n";
        send(fd, msg.c_str(), msg.length(), 0);
        msg.clear();
    }
    else
    {
        // Channel exists, add the user to the channel
        if (key == channels[channel_name]->getChannelKey()) 
        {
            if(it2->second->is_invited_user(this->get_nickname(fd)))
                it2->second->remove_invited_user(this->get_nickname(fd));
            // broadcast to all users in the channel
            std::string msg = ":" + this->get_nickname(fd) + "!" + this->get_username(fd) + "@" + this->get_ip_address(fd) + " JOIN " + channel_name + "\r\n";
            join_broadcast_msg(channels, msg, channel_name);
            // add user to the channel
            it2->second->addUser(this->get_nickname(fd));
            send(fd, msg.c_str(), msg.length(), 0);
            // reply to the user
            std::string str;
            if (this->get_topic(channel_name) == "No topic is set")
            {
                str = ":" + this->get_hostnames() + " 313 " + this->get_nickname(fd) + " " + channel_name + " :No topic is set\r\n";
                send(fd, str.c_str(), str.length(), 0);
            }
            else
            {
                str = ":" + this->get_hostnames() + " " + this->to_string(RPL_TOPIC) + " " + this->get_nickname(fd) + " " + channel_name + " :" + this->get_topic(channel_name) + "\r\n";
                send(fd, str.c_str(), str.length(), 0);
            } 
            // send the list of users in the channel
            std::set<std::string>::iterator it3 = channels[channel_name]->getUsers().begin();
            while (it3 != channels[channel_name]->getUsers().end())
            {
                std::string user = *it3;
                if (channels[channel_name]->isOperator("@" + user))
                    str = ":" + this->get_hostnames() + " " + this->to_string(RPL_NAMREPLY) + " " + this->get_nickname(fd) + " = " + channel_name + " :@" + user + "\r\n"; // Prefix '@' to operator's name
                else
                    str = ":" + this->get_hostnames() + " " + this->to_string(RPL_NAMREPLY) + " " + this->get_nickname(fd) + " = " + channel_name + " :" + user + "\r\n";
                send(fd, str.c_str(), str.length(), 0);
                it3++;
            }
            str = ":" + this->get_hostnames() + " " + this->to_string(RPL_ENDOFNAMES) + " " + this->get_nickname(fd) + " " + channel_name + " :End of /NAMES list\r\n";
            send(fd, str.c_str(), str.length(), 0);
            str.clear();
        } 
        else
        {
            // Channel has a key and the user didn't provide it
            std::string msg = ":" + this->get_hostnames() + " " + this->to_string(ERR_BADCHANNELKEY) + " " + this->get_nickname(fd) + " " + channel_name + " :Cannot join channel (+k)\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
        }
    }
    return (0);
}

void Server::handleChannels(std::vector<std::pair<std::string, std::string> >& pairs, int fd, const std::string& nickname )
{
    for(std::vector<std::pair<std::string, std::string> >::iterator it = pairs.begin(); it != pairs.end(); ++it) 
    {
        if(it->first.compare("#") == 0 || it->first.compare("&") == 0)
        {
            std::string msg = ":" + this->get_hostnames() + " " + this->to_string(ERR_BADCHANMASK) + " " + nickname + " " + it->first + " :Bad Channel Mask\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        }
        if(it->first.at(0) != '#' && it->first.at(0) != '&') 
        {
            // Channel name is invalid
            std::string msg = ":" + this->get_hostnames() + " " + this->to_string(ERR_BADCHANMASK) + " " + nickname + " " + it->first + " :Bad Channel Mask\r\n";
            send(fd, msg.c_str(), msg.length(), 0);
            continue;
        } 
        else
        {
            if(it->first.at(0) == '#' || it->first.at(0) == '&')
            {
                if(public_channel(it->first, it->second, fd) == 1)
                    continue;
            }
        }
    }
    pairs.clear();
}

int Server::JoinChannel(std::vector<std::string > strs , std::string nickname, int fd , std::string str)
{ 
    // pair of channel and key
    if(strs.size() > 3 && strs[2].at(0) != ':')
    {
        std::string str = ":" + this->get_hostnames() + " " + this->to_string(ERR_NEEDMOREPARAMS) + " " + this->get_nickname(fd) + " JOIN :Not enough parameters\r\n";
        send(fd, str.c_str(), str.length(), 0);
        return -2;
    }
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    std::vector<std::pair<std::string, std::string> > pair;
    std::stringstream ss(strs[1]);
    if (strs.size() >= 3) 
    {
        if(!strs[2].empty() && strs[2].at(0) == ':')
        {
            str = str.erase(0, str.find(strs[0]) + strs[0].length() + 1);
            str = str.erase(0, str.find(strs[1]) + strs[1].length() + 1);
            str = str.erase(0, 1);
            std::stringstream ss2(str);
            std::string token_keys;
            while (std::getline(ss2, token_keys, ' ')) 
            {
                keys.push_back(token_keys);
            }
            token_keys.clear();
        }
        if (!strs[2].empty() && strs[2].find(',') != std::string::npos) 
        {
            std::stringstream ss2(strs[2]);
            std::string token_keys;
            while (std::getline(ss2, token_keys, ',')) 
            {
                keys.push_back(token_keys);
            }
            token_keys.clear();
        } 
        else 
        {
            keys.push_back(strs[2]);
        }
    }
    if(strs[1].find(',') != std::string::npos) 
    {
        std::string token;
        while (std::getline(ss, token, ',')) 
        {
            channels.push_back(token);
        }
        token.clear();
    } 
    else 
    {
        channels.push_back(strs[1]);
    }
    if (channels.size() > 0) 
    {
        std::vector<std::string>::iterator it = channels.begin();
        std::vector<std::string>::iterator it2 = keys.begin();
        while (it != channels.end() && it2 != keys.end()) 
        {
            pair.push_back(std::make_pair(*it, *it2));
            it++;
            it2++;
        }
        if(it != channels.end())
        {
            while(it != channels.end())
            {
                pair.push_back(std::make_pair(*it, ""));
                it++;
            }
        }
    }
    handleChannels(pair, fd, nickname);
    channels.clear();
    keys.clear();
    return 0;
}

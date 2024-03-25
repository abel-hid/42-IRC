/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 02:08:17 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/25 23:34:09 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::vector<std::string> get_modes(std::string word)
{
    std::string::iterator it = word.begin();
    std::vector<std::string> modes;
    for (; it != word.end(); it++)
        modes.push_back(std::string(1, *it));
    return modes;
}

bool Server::isAllDigit(std::string str)
{
    for (std::string::iterator it = str.begin(); it != str.end(); it++)
    {
        if (!std::isdigit(*it))
            return false;
    }
    return true;
}

//check number of characters egale argument
int getNumMode(std::string mode)
{
    int sum = 0;
    int i = 0;
    while (i < (int)mode.length())
    {
        if (mode[i] == '+')
        {
            int j = i + 1;
            while (j < (int)mode.length() && (mode[j] != '+' && mode[j] != '-'))
            {
                if (mode[j] == 'o' || mode[j] == 'k' || mode[j] == 'l')
                    sum++;
                j++;
            }
            i = j;
        }
        if (mode[i] == '-')
        {
            int j = i + 1;
            while (j < (int)mode.length() && (mode[j] != '+' && mode[j] != '-'))
            {
                if (mode[j] == 'o' || mode[j] == 'k')
                    sum++;
                j++;
            }
            i = j;
        }
        else
            i++;
    }
    return sum;
}

void Server::addMode_I(std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            if (it->second->getInviteOnly() == add)
                return;
            else
                it->second->setInviteOnly(add);
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                        std::string mode = ":" + this->get_hostnames() + " MODE " + channelname + " " + mtype + "\r\n";
                        send(fdRe, mode.c_str(), mode.length(), 0);
                    }
                }
            }
        }
    }
}

void Server::addMode_T(std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            if (it->second->isTopicRestriction() == add)
                return ;
            else
                it->second->setTopicRestriction(add);
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                        std::string mode = ":" + this->get_hostnames() + " MODE " + channelname + " " + mtype + "\r\n";
                        send(fdRe, mode.c_str(), mode.length(), 0);
                    }
                }
            }
        }
    }
}

void Server::addMode_O(int fd, std::vector<std::string> words,std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    if (words.size() < 4)
    {
        std::string errorMode = ":" + this->get_hostnames() + " 461 "  + words[0] + " : Not enough parameters\r\n" ;
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return ;
    }
    if (!this->isClientExist(words[3]))
    {
        std::string errorMode = ":" + this->get_hostnames() + " 401 "  + words[3] + " : No such nick\r\n";
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return ;
    }
    if (!this->isClientInChannel(words[3], channelname, channel))
    {
        std::string errorMode = ":" + this->get_hostnames() + " 442 "  + words[3] + " " + channelname + " : You're not on that channel\r\n";
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return ;
    }
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            std::string nickname = "@" + words[3];
            if (add)
            {
                if (it->second->isOperator(nickname))
                    return ;
                else
                    it->second->addOperator(nickname);
                std::string addop = ":" + this->get_hostnames() + " MODE " + channelname + " +o " + words[3] + "\r\n";
                // send to all users in the channel
                for (std::set<std::string>::iterator it2 = it->second->getUsers().begin(); it2 != it->second->getUsers().end(); it2++)
                {
                    int fdRe;
                    std::string name;
                    for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                    {
                        fdRe = (*it1).first;
                        name = (*it1).second->getNickname();
                        if (name == *it2)
                            send(fdRe, addop.c_str(), addop.length(), 0);
                    }
                }
            }
            else
            {
                if (!it->second->isOperator(nickname))
                    return ;
                else
                    it->second->removeOperator(nickname);
                std::string removeop = ":" + this->get_hostnames() + " MODE " + channelname + " -o " + words[3] + "\r\n";
                // send to all users in the channel
                for (std::set<std::string>::iterator it2 = it->second->getUsers().begin(); it2 != it->second->getUsers().end(); it2++)
                {
                    int fdRe;
                    std::string name;
                    for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                    {
                        fdRe = (*it1).first;
                        name = (*it1).second->getNickname();
                        if (name == *it2)
                            send(fdRe, removeop.c_str(), removeop.length(), 0);
                    }
                }
            }
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                    }
                }
            }
        }
    }
}

void Server::addMode_L(int fd, std::vector<std::string> words,std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    if (add)
    {
        if (words.size() < 4)
        {
            std::string errorMode = ":" + this->get_hostnames() + " 461 "  + words[0] + " : Not enough parameters\r\n";
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        if (std::atol(words[3].c_str()) <= 0)
                    return ;
    }
    if (!add)
    {
        if (words.size() < 3)
        {
            std::string errorMode = ":" + this->get_hostnames() + " 461 "  + words[0] + " : Not enough parameters\r\n";
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
    }
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            if (add)
            {
                if (it->second->getLimit() == std::atol(words[3].c_str()))
                    return ;
                else
                    it->second->setLimit(std::atol(words[3].c_str()));
            }
            if (!add)
            {
                
                if (it->second->getLimit() == -1){
                    return ;
                }
                else
                {
                    it->second->setLimit(-1);
                }
            }
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                        std::string mode = ":" + this->get_hostnames() + " MODE " + channelname + " " + mtype + "\r\n";
                        send(fdRe, mode.c_str(), mode.length(), 0);
                    }
                }
            }
        }
    }
}

void Server::addMode_K(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &channel, std::string channelname, std::string modeType, bool add)
{
    if (words.size() < 4)
    {
        std::string errorMode = ":" + this->get_hostnames() + " 461 "  + words[0] + " : Not enough parameters\r\n";
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return ;
    }
    for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        if (it->first == channelname)
        {
            if (add)
            {
                if (it->second->getChannelKey().empty())
                    it->second->setChannelKey(words[3]);
                else
                {
                    std::string errorMode = ":" + this->get_hostnames() + " 475 " + this->get_nickname(fd) + " " + channelname + " :Channel key already set\r\n";
                    send(fd, errorMode.c_str(), errorMode.length(), 0);
                    return ;
                }
            }
            else
            {
                if (it->second->getChannelKey() == words[3])
                    it->second->removeChannelKey();
                else
                {
                    std::string errorMode = ":" + this->get_hostnames() + " 475 " + this->get_nickname(fd) + " " + channelname + " :Wrong channel key\r\n";
                    send(fd, errorMode.c_str(), errorMode.length(), 0);
                    return ;
                }
            }
            std::set<std::string> usersInChannel = it->second->getUsers();
            for (std::set<std::string>::iterator it2 = usersInChannel.begin(); it2 != usersInChannel.end(); it2++)
            {
                int fdRe;
                std::string name;
                for (std::map<int , Client *>::iterator it1 = this->clients.begin(); it1 != this->clients.end(); it1++)
                {
                    fdRe = (*it1).first;
                    name = (*it1).second->getNickname();
                    if (name == *it2)
                    {
                        std::string mtype = ((add)? "+": "-") + modeType;
                        std::string mode = ":" + this->get_hostnames() + " MODE " + channelname + " " + mtype + "\r\n";
                        send(fdRe, mode.c_str(), mode.length(), 0);
                    }
                }
            }
        }
    }
}

void Server::modecmd(std::string str, int fd)
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
    if ((int)(words.size() - 3) > getNumMode(words[2]))
    {
        return ;
    }
    //check is enough parameters
    if (words.size() < 2)
    {
        std::string errorMode = ":" + this->get_hostnames() + " 461 " + this->get_nickname(fd) + " " + words[0] + " :Not enough parameters\r\n";
        send(fd, errorMode.c_str(), errorMode.length(), 0);
        return;
    }
    else 
    {
        
        // check if channel exist && valid name of channel
        if (!this->isChannelExist(words[1]) || !this->isValidChannelName(words[1]))
        {
            std::string errorMode = ":" + this->get_hostnames() + " 403 " + this->get_nickname(fd) + " " + words[1] + " :No such channel\r\n";
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        if (words.size() == 2 && this->getChannels().find(words[1]) != this->getChannels().end())
        {
            std::map<std::string, Channel *>::iterator it = this->getChannels().find(words[1]);
            if (it != this->getChannels().end())
            {
                std::string msg = std::string(it->second->getInviteOnly() ? "i" : "") + (it->second->isTopicRestriction() ? "t" : "") + (!it->second->getChannelKey().empty() ? "k" : "") + (it->second->getLimit() == -1 ? "" : "l");
                std::string errorMode = ":" + this->get_hostnames() + " 324 " + this->get_nickname(fd) + " " + words[1] + (msg.empty() ? "" : " +" + msg) + " \r\n";
                send(fd, errorMode.c_str(), errorMode.length(), 0);
                return ;
            }
        }
        // check is sender in channel 
        if (!this->isSenderInChannel(this->get_nickname(fd), words[1], this->getChannels()))
        {
            std::string errorMode = ":" + this->get_hostnames() + " 442 " + this->get_nickname(fd) + " " + words[1] + " :You're not on that channel\r\n";
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        // check if sender is operator in channel
        if (!this->isClientOperatorInChannel(this->get_nickname(fd), words[1], this->getChannels()))
        {
            std::string errorMode = ":" + this->get_hostnames() + " 482 " + this->get_nickname(fd) + " " + words[1] + " :You're not channel operator\r\n";
            send(fd, errorMode.c_str(), errorMode.length(), 0);
            return ;
        }
        else
        {
            if (words[2].at(0) != '+' && words[2].at(0) != '-')
                return ;
            std::vector<std::string> modes = get_modes(words[2]);
            bool add;
            for (std::vector<std::string>::iterator it = modes.begin(); it != modes.end(); ++it)
            {
                if ((*it)[0] == '+')
                {
                    this->setFlagMode(true);
                    continue;
                }
                if ((*it)[0] == '-')
                {
                    this->setFlagMode(false);
                    continue;
                }
                add = this->getFlagMode();
                if (*it != "+" && *it != "-")
                {
                    switch ((*it)[0])
                    {
                        case 's':
                            break ;
                        case 'n':
                            break ;
                        case 'i':
                            this->addMode_I(this->getChannels(), words[1], *it, add);
                            break;
                        case 't':
                            this->addMode_T(this->getChannels(), words[1], *it, add);
                            break;
                        case 'o':
                            this->addMode_O(fd, words, this->getChannels(), words[1], *it, add);
                            if (words.size() > 3)
                                words.erase(words.begin() + 3);
                            break;
                        case 'l':
                            this->addMode_L(fd, words, this->getChannels(), words[1], *it, add);
                            if (words.size() > 3 && add == true)
                                words.erase(words.begin() + 3);
                            break;
                        case 'k':
                            this->addMode_K(fd, words, this->getChannels(), words[1], *it, add);
                            if (words.size() > 3)
                                words.erase(words.begin() + 3);
                            break;
                        default:
                            std::string errorMode = ":" + this->get_hostnames() + " 472 " + this->get_nickname(fd) + " " + *it + " :Unknown mode\r\n";
                            send(fd, errorMode.c_str(), errorMode.length(), 0);
                            break ;
                    }
                }
                else
                {
                    std::string errorMode = ":" + this->get_hostnames() + " 472 " + this->get_nickname(fd) + " " + *it + " :Unknown mode\r\n";
                    send(fd, errorMode.c_str(), errorMode.length(), 0);
                }
                
            }
        }
    }
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 02:02:31 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/19 01:34:13 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
void Server::comdBotBot(std::string SplitedMsg)
{
    std::string buffer = SplitedMsg;
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
    std::map <int, Client *>	clients = this->getClients();
    for (std::map<int, Client *>::iterator it = clients.begin(); it!= clients.end(); it++)
    {
        int fd;
        if (it->second->getNick() == "Bot")
            fd = it->second->getFd();
        if (it->second->getNick() == words[1] && words[2] == "time")
        {
            std::string botMsg = "🤖: Hello `" + words[1] + "' - 🕒 Current time is: " + words[2] + ":" + words[3] + " 😄.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        else if (it->second->getNick() == words[3] && words[1] == "level" && words[2] == "mush")
        {
            std::string botMsg = "🤖: Too many name for searching in the list 🤔.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        else if (it->second->getNick() == words[4] && words[1] == "level" && words[2] == "NOT" && words[3] == "Mush")
        {
            std::string botMsg = "🤖: Please enter a name for searching 🤷.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        if (it->second->getNick() == words[5] && words[1] == "level" && words[2] == "yes")
        {
            std::string botMsg = "🤖: Level Of `" + words[4] + "' is " + words[3] + "%, " + words[4] + " is a student of 1337-Khouribga 😄.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        else if (it->second->getNick() == words[4] && words[1] == "level"  && words[2] == "no")
        {
            std::string botMsg = "🤖: User `" + words[3] + "' is not found in the list 😞.";
            send_to_user(it->second->getNick(), botMsg, fd);
            return ;
        }
        else if (it->second->getNick() == words[1] && words[2] == "help" && words.size() == 3)
        {
            send_to_user(it->second->getNick(), "* Available commands: \n-Command: PASS / Parameters: <password> \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: PASS / Parameters: <password> \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: NICK / Parameters: <nickname> [ <hopcount> ] \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: USER / Parameters: <username> <hostname> <servername> <realname> \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: JOIN / Parameters: <channel>{,<channel>} [<key>{,<key>}] \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: TOPIC / Parameters: <channel> [<topic>] \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: INVITE / Parameters: <nickname> <channel> \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: KICK  / Parameters: <channel> <user> [<comment>] \n", fd);
            usleep(2);
            send_to_user(it->second->getNick(), "-Command: PRIVMSG / Parameters: <receiver>{,<receiver>} <text to be sent> \n", fd);
            usleep(2);
        }
        else if (it->second->getNick() == words[1] || (it->second->getNick() == words[2] && words[1] == "level"))
        {
            std::string botMsg = "🤖: I'm sorry, I don't understand what you mean 😞.";
            send_to_user(it->second->getNick(), botMsg, fd);
        }
    }
    iss.clear();
    words.clear();
}

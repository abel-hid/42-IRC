/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 11:56:01 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/21 02:38:59 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::send_to_all(std::string str, int fd)
{
    std::map<std::string, Channel*>::iterator it = this->getChannels().begin();
    std::vector<int> fds;
    for (; it != this->getChannels().end(); it++)
    {
        std::set<std::string>::iterator it2 = it->second->getUsers().begin();
        for (; it2 != it->second->getUsers().end(); it2++)
        {
            int user = this->get_fd_users(*it2);
            if (user != fd && std::find(fds.begin(), fds.end(), user) == fds.end())
                send(user, str.c_str(), str.length(), 0);
            fds.push_back(user);
        }
    }
    fds.clear();
}

void Server::quit_command(int fd)
{
    this->remove_client_from_channels(fd);
    this->removeClient(fd);
    delete this->clients[fd];
    this->clients.erase(fd);
    close(fd);
}

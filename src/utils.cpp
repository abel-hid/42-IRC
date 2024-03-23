/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:48:02 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/23 01:08:18 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool parsingInput(int ac, char **av)
{
    if (ac != 3)
	{
		std::cout << "\033[31;1mUsage: ./ircserv [port] [password]\033[0m 🤬." << std::endl;
		return false;
	}
	if (!parssing_port(av[1]))
	{
		std::cout << "\033[31;1mInvalid port\033[0m 🤬." << std::endl;
		return false;
	}
	if (std::strlen(av[2]) < 1)
	{
		std::cout << "\033[31;1mInvalid password\033[0m 🤬." << std::endl;
		return false;
	}
    return true;
}

int parssing_port(std::string port)
{
	int i = 0;
	if(std::atoi(port.c_str()) > 65535 || std::atoi(port.c_str()) < 1024 || port.size() > 5)
		return (0);
	for (std::string::iterator it = port.begin(); it != port.end(); it++)
    {
        if (!std::isdigit(*it))
            return (0);
        i++;
    }
	return (1);
}

std::string update_str(std::string str)
{
    if(str[0] == ':')
    {
        size_t space_pos = str.find(" ");
        if (space_pos != std::string::npos) 
		{
            str = str.substr(space_pos + 1);
        }
		else
		{
			str = "";
		}
    }
    return str;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:48:02 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/20 20:50:13 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int parssing_port(std::string port)
{
	int i = 0;
	if(std::atoi(port.c_str()) > 65535 || std::atoi(port.c_str()) < 1024)
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


bool isValidPassword(std::string password)
{
    if (password.empty())
        return false;
    for (std::string::iterator it = password.begin(); it != password.end(); it++)
    {
        if (!std::isalnum(*it))
            return false;
    }
    return true;
}

void handl_signal(int signum)
{
	if (signum == SIGINT)
		std::cout << "\n\033[31;1mServer is shutting down...\033[0m" << std::endl;
		exit(0);
}

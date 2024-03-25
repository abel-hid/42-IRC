/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 02:22:39 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/19 02:33:44 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool close_ser = false;

void handl_signal(int signum)
{
	if (signum == SIGINT)
	{
        std::cout << "\n\033[31;1mServer is shutting down...\033[0m" << std::endl;
		close_ser = true;
    }
}

int main(int ac, char **av)
{
    if (!parsingInput(ac, av))
        return (1);
	signal(SIGINT, handl_signal);
    signal(SIGPIPE, SIG_IGN);
    std::cout << Welcome << std::endl;
    Server server;
    server.setPort(atoi(av[1]));
    server.setServerPassword(av[2]);
	std::cout<< "\t[\033[32;1mINFO\033[0m] \033[32;1mServer is running on port \033[0m[\033[32;1m" << server.getPort() << "\033[0m]\n" << std::endl;
    if (!server.initServer())
        return (0);
    server.runServer(close_ser);
    return (0);
}

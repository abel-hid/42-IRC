/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bonus.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 17:43:05 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/22 01:47:41 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
	{
		std::cout << "\033[31;1mUsage: ./Bot [port] [password]\033[0m 🤬." << std::endl;
		return 0;
	}
	if (!parssing_port(av[1]))
	{
		std::cout << "\033[31;1mInvalid port\033[0m 🤬." << std::endl;
		return 0;
	}
    struct sockaddr_in bot_address;
    int port = atoi(av[1]);
    std::string pass = av[2];
    Bot *betbot = new Bot();
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    bot_address.sin_family = AF_INET;
    bot_address.sin_port = htons(port);
    bot_address.sin_addr.s_addr = INADDR_ANY;
    if ((connect(sockfd, (struct sockaddr*)&bot_address, sizeof(bot_address))) < 0) {
        std::cout << "Connection Failed  " << std::endl;;
        return -1;
    }
    betbot->ConnetToServer(sockfd, pass);
    delete betbot;
    close(sockfd); 
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:15:46 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/16 00:43:35 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int ac, char **av)
{
    
    if (ac != 3) {
        std::cerr << "Usage: ./Bot <port> <password>" << std::endl;
        return -1;
    }
    struct sockaddr_in serv_addr;
    int port = atoi(av[1]);
    std::string pass = av[2];
    Bot *betbot = new Bot();
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if ((connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        std::cout << "Connection Failed  " << std::endl;;
        return -1;
    }
    betbot->ConnetToServer(sockfd, pass);
    delete betbot;
    close(sockfd); 
}

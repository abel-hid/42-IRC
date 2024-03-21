/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:50:06 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/21 19:53:12 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

// CONSTRUCTOR & DESTRUCTOR
Bot::Bot(): name("BetBot"), nickName("Bot"), userName("betbot1"), fullName("dd")
{
    this->serverName = "irc.1337.ma";
    this->hostName = "irc.1337.ma";
}

Bot::~Bot() {}

// SETTERS FUNCTIONS
void    Bot::setStudent_13(std::map<std::string, float> Student_13) { this->Student_13 = Student_13;}


// GETTERS FUNCTIONS
std::map<std::string, float> Bot::getStudent_13() { return (this->Student_13);}

std::string Bot::get_current_time()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return (buf);
}

// UTILS FUNCTIONS

void    Bot::sendResponce(int fd, const std::string &responce) { send(fd, responce.c_str(), responce.length(), 0); }

void    Bot::ConnetToServer(int fd, std::string &pass)
{
    char message[1024];
    sleep(1);
    sprintf(message, "PASS %s\r\n", pass.c_str());
    send(fd, message, strlen(message), 0);
    sleep(1);
    sprintf(message, "USER %s * * :%s\r\n", this->userName.c_str(), this->fullName.c_str());
    send(fd, message, strlen(message), 0);
    sleep(1);
    sprintf(message, "NICK %s\r\n", this->nickName.c_str());
    send(fd, message, strlen(message), 0);
    sleep(1);
    while (true) 
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int rc = recv(fd, buffer, sizeof(buffer), 0);
        if (rc <= 0)
        {
            std::cerr << "Receive failed" << std::endl;
            break;
        }
        std::string message(buffer, rc);
        if (message.find("464") != std::string::npos && message.find("Password incorrect") != std::string::npos)
        {
            std::cerr << "\033[31;1mError: \033[0m \033[32;1mPassword incorrect\033[0m" << std::endl;
            close(fd);
            break ;
        }
        this->executeBot(message, fd);
    }
}

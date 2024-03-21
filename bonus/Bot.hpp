/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:50:47 by araiteb           #+#    #+#             */
/*   Updated: 2024/03/21 01:15:41 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
#ifndef BOT_HPP
#define BOT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <fcntl.h>
#include <cstddef> 
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <arpa/inet.h>

class Bot  {
    private:
        std::string name;
        std::string nickname;
        std::string username;
        std::string fullname;
        std::string servername;
        std::string hostname;

        std::map<std::string, float> Student_13;
		std::string allstring;

    public:
        Bot();
        ~Bot();
        Bot(Bot &b);
        Bot& operator=(Bot &b);
        void    ConnetToServer(int , std::string &);
        void    setStudent_13(std::map<std::string, float> Student_13);
        void 	executeBot(std::string &msg, int client_fd);
        void 	sendResponce(int fd, const std::string &responce);
        std::string     comdBot(std::vector<std::string> &words);
        std::string get_current_time();
        std::map<std::string, float> getStudent_13();
        std::map<std::string, float> first_read(std::string file);
        
};
int parssing_port(std::string port);
bool isValidPassword(std::string password);

#endif
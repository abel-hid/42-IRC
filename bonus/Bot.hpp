/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:50:38 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/21 19:53:09 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#ifndef BOT_HPP
# define BOT_HPP

# include <map>
# include <vector>
# include <stdio.h>
# include <errno.h>
# include <fstream>
# include <iomanip>
# include <cstring>
# include <fcntl.h>
# include <cstddef> 
# include <sstream>
# include <iostream>
# include <stdlib.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/poll.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <netinet/in.h>

class Bot
{
    private:
        std::string                     name;
        std::string                     nickName;
        std::string                     userName;
        std::string                     fullName;
        std::string                     hostName;
		std::string                     allstring;
        std::string                     serverName;
        std::map<std::string, float>    Student_13;
    public:
        // CONSTRUCTOR & DESTRUCTOR
        Bot();
        ~Bot();
        
        // SETTERS FUNCTIONS
        void                            setStudent_13(std::map<std::string, float> Student_13);
        
        // GETTERS FUNCTIONS
        std::string                     get_current_time();
        std::map<std::string, float>    getStudent_13();
        
        // BOT FUNCTIONS
        void 	                        executeBot(std::string &msg, int client_fd);
        std::string                     comdBot(std::vector<std::string> &words);

        // UTILS FUNCTIONS
        std::map<std::string, float>    first_read(std::string file);
        void                            ConnetToServer(int , std::string &);
        void 	                        sendResponce(int fd, const std::string &responce);
        
};
int                                     parssing_port(std::string port);
bool                                    isValidPassword(std::string password);

#endif
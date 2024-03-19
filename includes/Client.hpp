/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 17:41:59 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/19 19:51:59 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <vector>

class Client
{
    private:
        int fd;
        std::string password;
        std::string nickname;
        std::string username;
        std::string realname;
        std::string hostname;
        std::string servername;
        int 			is_registered;
		bool 			InviteToChannel;
		std::string		ip_address;
		std::vector<std::string> str;
    public:
        Client();
        Client(int fd);
        ~Client();
        void set_is_Registered(int is_registered);
        int is_Registered();
        void 			setInviteToChannel(bool InviteToChannel);
        bool			getInviteToChannel();
        void setHostname(std::string hostname);
        void setFd(int fd);
        void setPassword(std::string password);
        void setUsername(std::string username);
        void setRealname(std::string realname);
        void setServername(std::string servername);
        void setNickname(std::string nickname);
        std::vector<std::string> getStr();
        void setStr(std::string str);
        void clearStr();
        void			set_ip_address(std::string ip);
        int getFd();
        std::string		getipaddress();
        std::string getPassword();
        std::string getNickname();
        std::string getUsername();
        std::string getRealname();
        std::string getServername();
        std::string getHostname();
};

#endif
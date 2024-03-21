/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 17:41:59 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/21 02:15:50 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <vector>
# include <iostream>

class Client
{
    private:
        int                         fd;
		std::vector<std::string>    str;
        std::string                 password;
        std::string                 nickname;
        std::string                 username;
        std::string                 realname;
        std::string                 hostname;
        std::string                 servername;
		std::string		            ip_address;
        int 			            is_registered;
		bool 			            InviteToChannel;
    public:
        // CONSTRUCTOR & DESTRUCTOR
        Client();
        Client(int fd);
        ~Client();

        // SETTERS FUNCTIONS
        void                        setFd(int fd);
        void                        setStr(std::string str);
        void			            set_ip_address(std::string ip);
        void                        setHostname(std::string hostname);
        void                        setPassword(std::string password);
        void                        setUsername(std::string username);
        void                        setRealname(std::string realname);
        void                        setNickname(std::string nickname);
        void                        set_is_Registered(int is_registered);
        void                        setServername(std::string servername);
        void 			            setInviteToChannel(bool InviteToChannel);

        // GETTERS FUNCTIONS
        int                         getFd();
        std::vector<std::string>    getStr();
        std::string                 getPassword();
        std::string                 getNickname();
        std::string                 getUsername();
        std::string                 getRealname();
        std::string                 getHostname();
        std::string		            getipaddress();
        std::string                 getServername();
        bool			            getInviteToChannel();

        // UTILS FUNCTIONS
        void                        clearStr();
        int                         is_Registered();
};

#endif
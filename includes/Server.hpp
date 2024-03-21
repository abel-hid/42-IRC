/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 04:52:32 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/19 00:59:32 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#define Welcome "\033[33;1m\n\
░▒▓████████▓▒░▒▓████████▓▒          ░▒▓█▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░  \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        \n\
░▒▓██████▓▒░    ░▒▓█▓▒░    ▒▓███▓▒  ░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░        \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░  \033[0m\n\n\
\t\033[36;1mMade By: \033[0m\033[31mylamsiah \033[0m\033[32m| \033[0m\033[35mabel-hid \033[0m\n"

# include "Client.hpp"
# include "Channels.hpp"
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Channels.hpp"
#include <arpa/inet.h>
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

class Server
{
    private:
        int port;
        int server_fd;
        int new_socket;
        std::string server_password;
        sockaddr_in address;
        std::map<int, Client *> clients;
        std::map<std::string , Channel *> channels;
        std::map<std::string, float> Student_13;
		bool flagMode;
		std::string allstring;
		std::map<int, int> pport;
    public:
        Server() {
            this->port = 6667;
            this->server_fd = 0;
            this->new_socket = 0;
            this->address.sin_family = AF_INET;
            this->address.sin_addr.s_addr = INADDR_ANY;
            this->address.sin_port = htons(this->port);
        }
        ~Server() {}
        
        
        int getPort() const { return port;}
        int getServer_fd() const { return server_fd;}
        int getNew_socket() const { return new_socket;}
        sockaddr_in getAddress() const { return address;}
        
        void setPort(int port) { this->port = port;}
        void setServer_fd(int server_fd) { this->server_fd = server_fd;}
        void setNew_socket(int new_socket) { this->new_socket = new_socket;}
        void setAddress(sockaddr_in address) { this->address = address;}
        void setFlagMode(bool flag) 
        {
            flagMode = flag;
        }

        bool getFlagMode() 
        {
            return flagMode;
        }
        void setServerPassword(std::string password)
        {
            this->server_password = password;
        }
        std::string getServerPassword()
        {
            return this->server_password;
        }

        int receve_msg(int fd);
        int accept_client();
        bool initServer();
        void execute_command(std::string str, int fd);



        void addClient(int fd, Client *client);
        void removeClient(int fd);
        void setClients(std::map<int, Client *> clients);

        


        void setPport(int fd, int port) { pport[fd] = port; }
		int getPport(int fd) { return pport[fd]; }
		std::string &get_allstring() { return allstring; }
		void set_allstring(std::string str) { allstring = str; }


        bool			IsAuthorized(Client &);
        Client*			getClientByFd(int fdUser);
		Client*			getClientByNickname(std::string nickname);
        void set_realname(int fd, std::string realname);
        void set_hostname(int fd, std::string hostname);
        void set_servername(int fd, std::string servername);
        std::string get_password(int fd);
        std::string get_realname(int fd);
        void set_fd(int fd, int new_fd);
        void set_password(int fd, std::string password);
        void set_client(int fd, Client *client);
        void set_clients(std::map<int, Client *> clients);
        std::string get_servername(int fd);
		std::string get_ip_address(int fd);
		void set_ip_address(int fd, std::string ip_address);
        std::string get_current_time();
        int is_nickname_exist(std::string nickname);
        void set_is_registered(int fd, int is_registered);
        int is_registered(int fd);
        void addChannel(std::string name, Channel *channel);
        void set_creator(std::string channel, bool creater);
        std::string get_creator_name(std::string channel);
        int is_nickname_exist_and_registered(std::string nickname);
        void set_topic(std::string channel, std::string topic);
        std::string get_topic(std::string channel);
        void priny_users(std::string channel);
        std::string to_string(int number);
        void remove_client_from_channels(int fd);
        std::string get_users(std::string channel);
        long get_limit(std::string channel);
        std::map<int, Client *> &getClients();
		int get_fd_users(const std::string& nickname) const;
        std::string get_username(int fd);
		std::string get_hostname(int fd);
        void set_username(int fd, std::string username);
		std::string get_hostnames();
        std::string get_nickname(int fd);
		void set_nickname(int fd, std::string nickname);
        void		commandsIrc(std::vector <std::string> &words, std::string str, int fd);
		void			cmduser(Client *c, std::vector<std::string> &words , std::string str);
		void			cmdpass(std::vector<std::string>& words, Client *c , std::string str);
		void			cmdknick(std::vector<std::string> &words, Client *c );
		void			cmdprivmsg(std::vector<std::string>& words, Client *c , std::string str);
        bool is_authenticated(Client &client);
        bool getInviteToChannel(int fd);
		void comdBotBot(std::string str);
		void topic_command(std::vector<std::string > words , int fd , std::string str);
		void quit_command(int fd);
		void part_command(std::vector<std::string > words , int fd , std::string str);
		void join_command(std::vector<std::string > words , int fd , std::string str);
		void kick_command(std::vector<std::string > words , int fd , std::string str);

        void nickCmd1(std::string msg, Client *c);
        Client*   getClientByNickname(std::string nick, std::map <int, Client *> clients);
        std::map<std::string, Channel *> &getChannels();
        bool isClientExist(std::string nickname);
        bool isValidChannelName(std::string name);
        bool isChannelExist(std::string channelname);
        void invitecmd(std::string str, int fd);
        bool isClientInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel);
        bool isClientOperatorInChannel(std::string clientname, std::string channelname, std::map<std::string, Channel *> &channel);
        bool isSenderInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel);
		void privmsg_command(std::vector<std::string > words  , int fd , std::string str);
		void send_to_user(std::string nickname , std::string str ,int fd);
		void send_to_channel(std::string channel_name , std::string str, int fd);
		void send_to_all(std::string str, int fd);
		void join_topic_part_kick_privmsg(int fd , std::string str);
        int	JoinChannel(std::vector<std::string> strs , std::string nickname, int fd, std::string str);
        int public_channel(std::string channel_name , std::string key , int fd);
		void join_broadcast_msg(std::map<std::string, Channel*>& channels , std::string msg, std::string channelName);
        int	PartChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd, std::string nickname , std::string str);
        void topic_broadcast_msg(std::map<std::string, Channel*>& channels, const std::string& channelName, const std::string& nickname);
		void handleChannels(std::vector<std::pair<std::string, std::string> >& pairs, int fd, const std::string& nickname);
        int	TopicChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd , std::string str);
        bool isAllDigit(std::string str);
		int is_nickname_exist_and_registered(int fd, std::string nickname);
        void KickChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, std::string str);
		void modecmd(std::string str, int fd);
        void addMode_I(std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_O(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_T(std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_L(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void addMode_K(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void send_nick(Client *c, std::string nickMsg);
        void users_update(std::string old_nick, std::string new_nick, std::map<std::string, Channel*> channels);
};
int parssing_port(std::string port);
std::string update_str(std::string str);
bool isValidPassword(std::string password);
void handl_signal(int signum);

#endif

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

# include <map>
# include <vector>
# include <stdio.h>
# include <iomanip>
# include <fstream>
# include <cstring>
# include <fcntl.h>
# include <cstddef> 
# include <sstream>
# include <errno.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <iostream>
# include <stdlib.h>
# include "Client.hpp"
# include <sys/poll.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include "Channels.hpp"
# include "Channels.hpp"
# include <sys/socket.h>
# include <netinet/in.h>

# define Welcome "\033[33;1m\n\
░▒▓████████▓▒░▒▓████████▓▒          ░▒▓█▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░  \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        \n\
░▒▓██████▓▒░    ░▒▓█▓▒░    ▒▓███▓▒  ░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░        \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n\
░▒▓█▓▒░         ░▒▓█▓▒░             ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░  \033[0m\n\n\
\t\033[36;1mMade By: \033[0m\033[31mylamsiah \033[0m\033[32m| \033[0m\033[35mabel-hid \033[0m\n"


class Server
{
    private:
        int                                 port;
		std::map<int, int>                  pport;
        std::map<int, Client *>             clients;
        sockaddr_in                         address;
		bool                                flagMode;
        std::map<std::string , Channel *>   channels;
        int                                 server_fd;
		std::string                         allstring;
        std::map<std::string, float>        Student_13;
        int                                 new_socket;
        std::string                         server_password;
    public:
        // CONSTRUCTOR & DESTRUCTOR
        Server();
        ~Server();

        // SERVER FUNCTIONS
        bool                                initServer();
        int                                 accept_client();
        int                                 receve_msg(int fd);
        void                                addClient(int fd, Client *client);
        void                                addChannel(std::string name, Channel *channel);
        void                                execute_command(std::string str, int fd);
        void                                commandsIrc(std::vector <std::string> &words, std::string str, int fd);

        //UTILS FUNCTIONS
        std::string                         to_string(int number);
		void                                comdBotBot(std::string str);
        void                                priny_users(std::string channel);
        void                                remove_client_from_channels(int fd);

        // USER && PASS FUNCTIONS
		void                                cmduser(Client *c, std::vector<std::string> &words , std::string str);
		void                                cmdpass(std::vector<std::string>& words, Client *c , std::string str);

        // TOPIC FUNCTIONS
		void                                topic_command(std::vector<std::string > words , int fd , std::string str);
        int	                                TopicChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd , std::string str);
        void                                topic_broadcast_msg(std::map<std::string, Channel*>& channels, const std::string& channelName, const std::string& nickname);

        // PART && INVITE && QUIT FUNCTIONS
        void                                removeClient(int fd);
        void                                invitecmd(std::string str, int fd);
        void                                part_command(std::vector<std::string > words , int fd , std::string str);
        int	                                PartChannel(std::vector<std::string> strs ,std::map<std::string, Channel *> &channels,  int fd, std::string nickname , std::string str);

        //KICK FUNCTIONS
		void                                kick_command(std::vector<std::string > words , int fd , std::string str);
        void                                KickChannel(std::vector<std::string> strs, std::map<std::string, Channel *> &channels, int fd, std::string nickname, std::string str);


        // PRIVMSG FUNCTIONS
		void                                send_to_user(std::string nickname , std::string str ,int fd);
		void                                send_to_channel(std::string channel_name , std::string str, int fd);
		void                                privmsg_command(std::vector<std::string > words  , int fd , std::string str);

        // QUIT FUNCTIONS
		void                                quit_command(int fd);
		void                                send_to_all(std::string str, int fd);

        // MODE FUNCTIONS
		void                                modecmd(std::string str, int fd);
        void                                addMode_I(std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void                                addMode_T(std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void                                addMode_O(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void                                addMode_L(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);
        void                                addMode_K(int fd, std::vector<std::string> words, std::map<std::string, Channel *> &Channel, std::string channelname, std::string modeType, bool add);

        // JOIN FUNCTIONS
        void                                join_topic_part_kick_privmsg(int fd , std::string str);
        int                                 public_channel(std::string channel_name , std::string key , int fd);
		void                                join_command(std::vector<std::string > words , int fd , std::string str);
        int	                                JoinChannel(std::vector<std::string> strs , std::string nickname, int fd, std::string str);
        void                                join_broadcast_msg(std::map<std::string, Channel*>& channels , std::string msg, std::string channelName);
		void                                handleChannels(std::vector<std::pair<std::string, std::string> >& pairs, int fd, const std::string& nickname);

        // NICK FUNCTIONS
        void                                nickCmd1(std::string msg, Client *c);
        void                                send_nick(Client *c, std::string nickMsg);
        void                                cmdknick(std::vector<std::string> &words, Client *c );
        void                                users_update(std::string old_nick, std::string new_nick, std::map<std::string, Channel*> channels);

        // SETTERS FUNCTIONS
        void                                setPort(int port);
        void                                setFlagMode(bool flag);
        void                                setPport(int fd, int port);
        void                                set_fd(int fd, int new_fd);
        void                                setServer_fd(int server_fd);
        void                                setNew_socket(int new_socket);
		void                                set_allstring(std::string str);
        void                                setAddress(sockaddr_in address);
        void                                set_client(int fd, Client *client);
        void                                setServerPassword(std::string password);
        void                                set_password(int fd, std::string password);
        void                                set_realname(int fd, std::string realname);
        void                                set_hostname(int fd, std::string hostname);
        void                                set_username(int fd, std::string username);
		void                                set_nickname(int fd, std::string nickname);
        void                                setClients(std::map<int, Client *> clients);
        void                                set_clients(std::map<int, Client *> clients);
        void                                set_is_registered(int fd, int is_registered);
        void                                set_servername(int fd, std::string servername);
        void                                set_creator(std::string channel, bool creater);
        void                                set_topic(std::string channel, std::string topic);

        // GETTERS FUNCTIONS
        bool                                getFlagMode();
        std::map<int, Client *>             &getClients();
        std::map<std::string, Channel *>    &getChannels();
		std::string                         get_hostnames();
        int                                 getPort() const;
		std::string                         &get_allstring();
		int                                 getPport(int fd);
        sockaddr_in                         getAddress() const;
        std::string                         get_current_time();
        std::string                         getServerPassword();
        std::string                         get_password(int fd);
        std::string                         get_realname(int fd);
        std::string                         get_username(int fd);
		std::string                         get_hostname(int fd);
        std::string                         get_nickname(int fd);
        int                                 getServer_fd() const;
        int                                 getNew_socket() const;
		std::string                         get_ip_address(int fd);
        std::string                         get_servername(int fd);
        Client*			                    getClientByFd(int fdUser);
        bool                                getInviteToChannel(int fd);
        long                                get_limit(std::string channel);
        std::string                         get_topic(std::string channel);
        std::string                         get_users(std::string channel);
        std::string                         get_creator_name(std::string channel);
		Client*			                    getClientByNickname(std::string nickname);
		int                                 get_fd_users(const std::string& nickname) const;
        Client*                             getClientByNickname(std::string nick, std::map <int, Client *> clients);

        // CHECKER FUNCTIONS
        int                                 is_registered(int fd);
        bool			                    IsAuthorized(Client &);
        bool                                isAllDigit(std::string str);
        bool                                is_authenticated(Client &client);
        bool                                isClientExist(std::string nickname);
        bool                                isValidChannelName(std::string name);
        int                                 is_nickname_exist(std::string nickname);
        bool                                isChannelExist(std::string channelname);
        int                                 is_nickname_exist_and_registered(std::string nickname);
        int                                 is_nickname_exist_and_registered(int fd, std::string nickname);
        bool                                isClientInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel);
        bool                                isSenderInChannel(std::string nickname, std::string channelname, std::map<std::string, Channel *> &channel);
        bool                                isClientOperatorInChannel(std::string clientname, std::string channelname, std::map<std::string, Channel *> &channel);
};
void                                        handl_signal(int signum);
std::string                                 update_str(std::string str);
int                                         parssing_port(std::string port);
bool                                        isValidPassword(std::string password);

#endif

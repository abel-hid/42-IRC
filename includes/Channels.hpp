/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 03:03:46 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/19 01:31:04 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include <iostream>
# include <vector>
# include <string>
# include <set>

# define     RPL_NICK 353 
# define     RPL_TOPIC 332
# define     RPL_NOTOPIC 331
# define     RPL_INVITING 341 
# define     RPL_NAMREPLY 353
# define     RPL_ENDOFNAMES 366
# define     ERR_NOSUCHNICK 401
# define     ERR_NOTOPLEVEL 413
# define     ERR_BADCHANMASK 476
# define     ERR_NORECIPIENT 411
# define     RPL_TOPICWHOTIME 333
# define     ERR_NOSUCHSERVER 402
# define     ERR_NOTEXTTOSEND 412
# define     ERR_WILDTOPLEVEL 414
# define     ERR_NOTONCHANNEL 442
# define     ERR_NOSUCHCHANNEL 403
# define     ERR_NICKNAMEINUSE 433
# define     ERR_USERONCHANNEL 443
# define     ERR_NICKCOLLISION 436
# define     ERR_NOTREGISTERED 451
# define     ERR_BADCHANNELKEY 475
# define     ERR_TOOMANYTARGETS 407
# define     ERR_UNKNOWNCOMMAND 421
# define     ERR_NEEDMOREPARAMS 461
# define     ERR_PASSWDMISMATCH 464
# define     ERR_NONICKNAMEGIVEN 431
# define     ERR_CANNOTSENDTOCHAN 404
# define     ERR_ERRONEUSNICKNAME 432
# define     ERR_ALREADYREGISTRED 462
# define     ERR_CHANOPRIVSNEEDED 482

class Channel
{
    private:
        std::string             name;
        long                    limit;
        std::string             topic;
        std::set<std::string>   users;
        bool                    creater;
        std::string             password;
        std::set<std::string>   operators;
        bool                    inviteOnly;
        std::string             channelKey;
        bool                    topicRestriction;
    public:
        // CONSTRUCTOR & DESTRUCTOR
        Channel(const std::string& channelName, const std::string& channelTopic = "no topic is set", const std::string& channelPassword = ""  , bool creater = false);
        ~Channel();

        // SETTERS FUNCTIONS
        void                    setLimit(long limit);
        void                    set_creater(bool creater);
        void                    setInviteOnly(bool isInviteOnly);
        void                    set_topic(const std::string& topic);
        void                    setChannelKey(const std::string& key);
        void                    setTopicRestriction(bool isTopicRestriction);
        void                    setChannelName(const std::string& channelName);
        void                    setChannelTopic(const std::string& channelTopic);
        void                    setChannelPassword(const std::string& channelPassword);

        // GETTERS FUNCTIONS
        long                    getLimit();
        std::set<std::string>   &getUsers();
        std::string             get_topic();
        bool                    get_creater();
        std::set<std::string>   &getOperators();
        bool                    getInviteOnly();
        std::string             getChannelKey();
        std::string             getChannelName();
        std::string             getChannelTopic();
        std::string             get_creator_name();
        std::string             getChannelPassword();
        
        // CHECKER FUNCTIONS
        bool                    isInviteOnly();
        bool                    isTopicRestriction();
        bool                    isUser(const std::string& nickname);
        bool                    isOperator(const std::string& nickname);

        // UTILS FUNCTIONS
        void                    print_users();
        void                    removeLimit();
        void                    removeChannelKey();
        void                    quit_channel(std::string nickname);
        void                    addUser(const std::string& nickname);
        void                    addOperator(const std::string& user);
        void                    removeUser(const std::string& nickname);
        void                    removeOperator(const std::string& user);
};

#endif

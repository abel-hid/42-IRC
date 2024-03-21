/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 03:02:59 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/21 02:29:01 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// CONSTRUCTOR & DESTRUCTOR

Channel::Channel(const std::string& channelName, const std::string& channelTopic, const std::string& channelPassword, bool creater)
{
    name = channelName;
    topic = channelTopic;
    password = channelPassword;
    inviteOnly = false;
    topicRestriction = false;
    this->creater = creater;
    limit = -1;
}

Channel::~Channel() {}

// SETTERS FUNCTIONS

void Channel::setLimit(long limit) { this->limit = limit; }

void Channel::set_creater(bool creater){ this->creater = creater; }

void Channel::set_topic(const std::string& topic) { this->topic = topic; }

void Channel::setChannelKey(const std::string& key) { channelKey = key; }

void Channel::setInviteOnly(bool isInviteOnly) { inviteOnly = isInviteOnly; }

void Channel::setChannelName(const std::string& channelName) { name = channelName; }

void Channel::setChannelTopic(const std::string& channelTopic) { topic = channelTopic; }

void Channel::setChannelPassword(const std::string& channelPassword) { password = channelPassword; }

void Channel::setTopicRestriction(bool isTopicRestriction) { topicRestriction = isTopicRestriction; }


// GETTERS FUNCTIONS

long Channel::getLimit() { return this->limit; }

bool Channel::get_creater() { return (creater); }

std::string Channel::get_topic(){ return (topic); }


bool Channel::getInviteOnly() { return inviteOnly; }

std::string Channel::getChannelName() { return name; }

std::string Channel::getChannelTopic() { return topic; }

std::string Channel::getChannelKey() { return channelKey; }

std::set<std::string> &Channel::getUsers() { return users; }

std::string Channel::getChannelPassword() { return password; }


std::set<std::string> &Channel::getOperators() { return operators; }

std::string Channel::get_creator_name()
{
    std::set<std::string>::iterator it = operators.begin();
    while(it != operators.end())
    {
        if (creater == true)
            return (*it);
        it++;
    }
    if(operators.size() > 0)
        return (*operators.begin());
    return ("no operator found");
}

// CHECKER FUNCTIONS

bool Channel::isInviteOnly() { return inviteOnly; }

bool Channel::isTopicRestriction() { return topicRestriction; }

bool Channel::isOperator(const std::string& user) { return operators.find(user) != operators.end(); }

bool Channel::isUser(const std::string& user) { return users.find(user) != users.end(); }


// UTILS FUNCTIONS

void Channel::removeLimit() { this->limit = -1; }

void Channel::removeChannelKey() { this->channelKey = ""; }

void Channel::addUser(const std::string& user) { users.insert(user); }

void Channel::removeUser(const std::string& user) { users.erase(user); }

void Channel::addOperator(const std::string& user) { operators.insert(user); }

void Channel::removeOperator(const std::string& user) { operators.erase(user); }

void Channel::quit_channel(std::string nickname)
{
    if (users.find(nickname) != users.end())
        users.erase(nickname);
    if (operators.find(nickname) != operators.end())
        operators.erase(nickname);
    if(creater == true)
        creater = false;
}

void Channel::print_users()
{
    std::set<std::string>::iterator it = users.begin();
    while(it != users.end())
    {
        std::cout << *it << std::endl;
        it++;
    }
}

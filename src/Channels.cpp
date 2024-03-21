/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 03:02:59 by abel-hid          #+#    #+#             */
/*   Updated: 2024/03/20 23:44:39 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

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

void Channel::setChannelName(const std::string& channelName) { name = channelName; }

void Channel::setChannelTopic(const std::string& channelTopic) { topic = channelTopic; }

void Channel::setChannelPassword(const std::string& channelPassword) { password = channelPassword; }

void Channel::setInviteOnly(bool isInviteOnly) { inviteOnly = isInviteOnly; }

void Channel::setTopicRestriction(bool isTopicRestriction) { topicRestriction = isTopicRestriction; }

bool Channel::getInviteOnly() { return inviteOnly; }

void Channel::setChannelKey(const std::string& key) { channelKey = key; }

std::string Channel::getChannelName() { return name; }

std::string Channel::getChannelTopic() { return topic; }

std::string Channel::getChannelPassword() { return password; }

bool Channel::isInviteOnly() { return inviteOnly; }

bool Channel::isTopicRestriction() { return topicRestriction; }

std::string Channel::getChannelKey() { return channelKey; }

std::set<std::string> &Channel::getOperators() { return operators; }

void Channel::addOperator(const std::string& user) { operators.insert(user); }

void Channel::removeOperator(const std::string& user) { operators.erase(user); }

bool Channel::isOperator(const std::string& user) { return operators.find(user) != operators.end(); }

void Channel::addUser(const std::string& user) { users.insert(user); }

void Channel::removeUser(const std::string& user) { users.erase(user); }

bool Channel::isUser(const std::string& user) { return users.find(user) != users.end(); }

std::set<std::string> &Channel::getUsers() { return users; }

std::string Channel::get_topic(){ return (topic); }

void Channel::set_topic(const std::string& topic) { this->topic = topic; }

void Channel::set_creater(bool creater){ this->creater = creater; }

bool Channel::get_creater() { return (creater); }

void Channel::setLimit(long limit) { this->limit = limit; }

long Channel::getLimit() { return this->limit; }

void Channel::removeLimit() { this->limit = -1; }

void Channel::removeChannelKey() { this->channelKey = ""; }

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
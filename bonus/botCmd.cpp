/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botCmd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylamsiah <ylamsiah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 19:48:54 by ylamsiah          #+#    #+#             */
/*   Updated: 2024/03/25 00:02:10 by ylamsiah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int parssing_port(std::string port)
{
	int i = 0;
	if (port.empty())
        return (0);
	while (port[i] == '0')
		i++;
	port = port.substr(i);
	if(std::atoi(port.c_str()) > 65535 || std::atoi(port.c_str()) < 1024 || port.size() > 5)
		return (0);
	for (std::string::iterator it = port.begin(); it != port.end(); it++)
    {
        if (!std::isdigit(*it))
            return (0);
    }
	return (1);
}

std::string update_str(std::string str)
{
    if(str[0] == ':')
    {
        size_t space_pos = str.find(" ");
        if (space_pos != std::string::npos) 
		{
            str = str.substr(space_pos + 1);
        }
		else
		{
			str = "";
		}
    }
    return str;
}

std::map<std::string, float> Bot::first_read(std::string file)
{
    std::ifstream file1(file);
    if (!file1.is_open())
    {
        std::cerr << "Error: Can't open file" << std::endl;
        exit (1);
    }
    std::string line;
    std::map<std::string, float> listStud;
    while (std::getline(file1, line))
    {
        if (line.empty())
            continue ;
        std::string data;
        std::string value;

        data = line.substr(0, line.find(':'));
        data.erase(0, data.find_first_not_of(" \t\n\r\f\v"));
        data.erase(data.find_last_not_of(" \t\n\r\f\v") + 1);
        value = line.substr(line.find(':') + 1);
        value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
        value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
        listStud[data] = std::atof(value.c_str());
    }
    file1.close();
    return listStud;
}

std::string Bot::comdBot(std::vector<std::string> &words)
{
    std::string file = "bot.txt";
    this->setStudent_13(first_read(file));
    std::map<std::string, float> listStud = this->getStudent_13();

    std::map<std::string, float>::iterator it = listStud.find(words[4]);
    if (it != listStud.end())
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << it->second;
        std::string botMsg = "yes " + ss.str() + " " + words[4] + " ";
        return botMsg;
    }
    else
    {
        std::string botMsg = "no "+ words[4] + " ";
        return botMsg;
    }
}

void 	Bot::executeBot(std::string &msg, int client_fd)
{
    char message[1548];
    std::cout << msg;
    std::string sender;
    std::string buffer = msg;
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
    if(buffer.empty())
        return ;
    std::vector<std::string> words;
    std::string word;
    std::istringstream iss(buffer);
    while (iss >> word)
    {
        words.push_back(word);
    }
    if(words.size() == 0)
        return ;
    sender = words[0].substr(1, words[0].find("!") - 1);
    if (!words[3].empty() && !words[3].compare(":level") && words.size() > 5)
    {
        std::string mssg = "🤖:  level  mush " + sender;
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && (!words[3].compare("::level") || !words[3].compare(":level")) && words.size() == 4)
    {
        std::string mssg = "🤖:  level  NOT Mush " + sender;
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && !words[3].compare(":time") && words.size() == 4)
    {
        std::string mssg = "🤖: " + sender + " " + "time "+ " " + this->get_current_time();
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].empty() && !words[3].compare(":level"))
    {
        std::string mssg = "🤖:  level " + this->comdBot(words) + sender.c_str();
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else if (!words[3].compare(":help") && words.size() == 4)
    {
        std::string mssg = "🤖: " + sender + " " + "help";
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
    else 
    {
        std::string mssg = "🤖: " + sender + " " + words[3] + ((words.size() > 4)? " " + words[4] : "");
        sprintf(message, "%s\r\n", mssg.c_str());
        sendResponce(client_fd, message);
    }
}

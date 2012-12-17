/*
 * Copyright (c) 2002-2009 Moxie Marlinspike
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "Logger.hpp"

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/BasicLayout.hh>
#include <iostream>
#include <fstream>

void Logger::initialize(std::string &path, bool postOnly) {
  log4cpp::Appender* app  = new log4cpp::FileAppender("FileAppender", path);
  log4cpp::Layout* layout;

  layout = new log4cpp::BasicLayout();
  app->setLayout(layout);

  log4cpp::Category &sslsniff = log4cpp::Category::getInstance("sslsniff");
  
  sslsniff.setAdditivity(false);
  sslsniff.setAppender(app);
  if (postOnly) sslsniff.setPriority(log4cpp::Priority::INFO);  
  else          sslsniff.setPriority(log4cpp::Priority::DEBUG);
}

void Logger::logFromServer(std::string &name, char *buf, int len) {
  std::string data(buf, len);
  std::string message = "Read from Server (";
  message.append(name);
  message.append(") :\n");
  message.append(data);

  log4cpp::Category::getInstance("sslsniff").debug(message);
}

void Logger::logFromClient(std::string &name, char* buf, int len) {
  std::string data(buf, len);
  std::string message = "Read from Client (";
  message.append(name);
  message.append(") :\n");
  message.append(data);

  log4cpp::Category::getInstance("sslsniff").debug(message);
}

void Logger::logFromClient(std::string &name, HttpHeaders &headers) {
  std::string message = "Got POST (";
  message.append(name);
  message.append(") :\n");
  message.append(headers.getPostData());

  log4cpp::Category::getInstance("sslsniff").info(message);
}

// Logs the keys used by the SSL session
void Logger::logKeys(SSL_SESSION *session) {
 std::ofstream keyFile;

  keyFile.open ("/home/kjetil/Desktop/master.txt", std::ios::app);
  //keyFile.close();

  char str[100];

// Sjekk at session id er større enn 0?
if((unsigned int)session->session_id_length)
{
   // Read the session ID
 std::string message = "RSA Session-ID:";
 for(int k = 0; k < (unsigned int)session->session_id_length; k++)
 {
	snprintf(str, 100, "%02X", (unsigned char)session->session_id[k]); // Convert to hex.
	message.append(str);
 }

    // Read the master key
  message.append(" Master-Key:");
  for(int i = 0; i < (unsigned int)session->master_key_length; i++)
  {
	snprintf(str, 100, "%02X", (unsigned char)session->master_key[i]); // Convert to hex.
	message.append(str);
	//std::cout << "\n" << str;
  }
	//std::cout << message;

  //log4cpp::Category::getInstance("sslsniff").info(message);
  keyFile << message << std::endl;
}
}

void Logger::logUpdateRequest(std::string &product, std::string &version, 
			      std::string &buildId, std::string &buildTarget,
			      std::string &locale, std::string &channel,
			      std::string &filename)
{
  std::string message = 
    "Got update request:\n Product: " + product     + "\n" +
    "Version: "                       + version     + "\n" +
    "ID: "                            + buildId     + "\n" +
    "Target: "                        + buildTarget + "\n" +
    "Locale: "                        + locale      + "\n" +
    "Channel: "                       + channel     + "\n" +
    "Filename: "                      + filename;

  log4cpp::Category::getInstance("sslsniff").info(message);
}

void Logger::logError(std::string error) {
  log4cpp::Category::getInstance("sslsniff").debug(error);
}

void Logger::logAddonUpdate(std::string &appId) {
  log4cpp::Category::getInstance("sslsniff").info("Updating Firefox Extension: " +  appId);
}

void Logger::logInit(std::string message) {
  log4cpp::Category::getInstance("sslsniff").info(message);
}

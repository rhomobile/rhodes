/*
 *  SyncManagerI.h
 *  RhoSyncClient
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

char *get_session(const char *url_string);
char *fetch_remote_data(char *url_string);
int push_remote_data(char* url, char* data, size_t data_size);
int login(const char *login, const char *password);

/**
 * login to rhosync server (default implementation)
 * If succeeded stores session into the database
 * 
 * To start using this method you will need to implement 
 * get_session_from_login callback for you platform 
 * 
 * @param login
 * @param password
 * @return 1 - succeeded, 0 - failed
 */
//int db_login ( const char* login, const char* password );

/**
 * This callback is required be db_login (platform specific part)
 * 
 * @param url
 * @param login
 * @param password
 * @return session
 */
//char* get_session_from_login(char* url, char* data );//login, char* password);
int makeLoginRequest(char* url, char* data );
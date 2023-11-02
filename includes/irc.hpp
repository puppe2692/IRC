/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:55:36 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/02 16:12:45 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <cstdio>
# include <cstdlib>
# include <string>
# include <cstring>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <sys/select.h>
# include <netinet/in.h>

# define RED "\e[0;31m"
# define RESET "\e[0m"
# define GREEN "\e[0;32m"
# define YELLOW "\e[0;33m"

struct sockaddr_in srv;
fd_set fr, fw, fe; // 3 fd_set (fr:socket descriptore for which I'm ready to read, Fw:write, Fe:exception)
int nMaxFd;
int nSocket;
int nArrClient[5];
struct timeval tv;

#endif
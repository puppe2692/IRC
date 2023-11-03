/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc2.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:55:36 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/03 10:41:58 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCDEUX_HPP
# define IRCDEUX_HPP

# include <cstdio>
# include <cstdlib>
# include <string>
# include <cstring>
# include <iostream>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <sys/epoll.h>
# include <netinet/in.h>

# define MAX_EVENTS 10

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
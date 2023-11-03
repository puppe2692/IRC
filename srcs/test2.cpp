/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:43:44 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/03 16:39:18 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc2.hpp"

int count_set_fds(fd_set *fds, int max_fd) {
    int count = 0;
    for (int i = 0; i <= max_fd; i++) {
        if (FD_ISSET(i, fds)) {
            count++;
        }
    }
    return count;
}

void ProcessNewMessage(int ClientSocket)
{
	std::cout << "Processing the new message for client socket: " << ClientSocket << std::endl;
	char buff[1024 + 1] = { 0, };
	int nRet = recv(ClientSocket, buff, 1024, 0);
	if (nRet < 0)
	{
		std::cout << "Oups something went wrong...Closing the connnection for client" << std::endl;
		close(ClientSocket);
		for (int nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nArrClient[nIndex] == ClientSocket)
				{
					nArrClient[nIndex] = 0;
					break;
				}
			}
	}
	else
	{
		std::cout << "the message from the client is: " << buff << std::endl;
		//Send the response to client
		//send(ClientSocket, "Processed your request", 23, 0);
		std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
	}
} 

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "arg missing" << std::endl;
		return(1);
	}

	int port = atoi(argv[1]);
	int nRet = 0;
	//Initialize the socket
	nSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nSocket < 0)
	{
		std::cout << "The socket not opened" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "Socket opened success" << std::endl;

	//Initialize the environment for sockaddr struct
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port); //host to network start;
	srv.sin_addr.s_addr = INADDR_ANY; // donne l adresse de la machine local
	//srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(srv.sin_zero), 0, 8);

	//setsockopt

	int nOptVal = 0;
	int nOptLen = sizeof (nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);
	if (nRet != 0)
		std::cout << "setsockopt call failed" << std::endl;
	else
		std::cout << "setsockopt call success" << std::endl;
	//Set the socket as non blocking socket
	//optval = 0 means blocking and !=0 means non blocking
	u_long optval = 1;
	nRet = ioctl (nSocket, FIONBIO, &optval);
	if (nRet != 0)
		std::cout << "ioctl call failed" << std::endl;
	else
		std::cout << "ioctl call success" << std::endl;


	//Bind the socket to the local port
	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0)
	{
		std::cout << "Fail to bind to local port" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "Successfully bind to local port" << std::endl;

	//Listen the request from client (queues the requests)
	nRet = listen(nSocket, 5);
	if (nRet < 0)
	{
		std::cout << "Fail to start listen to local port" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "Successfully listen to local port" << std::endl;

	// cree une instance epoll avec un nombre max d'evenement specifie;
	int epfd = epoll_create(MAX_EVENTS);
    if (epfd == -1) 
	{
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

	// initialise une structure pour l'enregistrement d'événements.
	struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = nSocket;

	// Ajoute le socket principal au groupe epoll.
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, nSocket, &event) == -1) 
	{
    	perror("epoll_ctl");
    	close(epfd);
    	exit(EXIT_FAILURE);
    }

	struct epoll_event events[MAX_EVENTS];

	while (1)
    {
        int ready = epoll_wait(epfd, events, MAX_EVENTS, -1);

		// en gros si j ai bien saisi, wait va attendre que se produisent des evenements sur les sockets presents dans epoll, 
		//il va stocker les cartes d identiter des sockets conernes dans events, puis en parcourant events on va pouvoir realiser
		// les actions en fonction

        if (ready == -1) {
            perror("epoll_wait");
            close(epfd);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < ready; i++) {
            if (events[i].data.fd == nSocket) 
			{
                // New connection request
                socklen_t nLen = sizeof(struct sockaddr);
                int nClientSocket = accept(nSocket, NULL, &nLen);
                if (nClientSocket > 0)
                {
                    event.events = EPOLLIN | EPOLLOUT;  // Add write event if needed
                    event.data.fd = nClientSocket;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, nClientSocket, &event);
					std::string RPL001 = "001 :Got the connection done successfully\r\n";
                    send(nClientSocket, RPL001.c_str(), RPL001.size() , 0);
                }
            }
            else 
			{
                // Existing client socket has data
                int nClientSocket = events[i].data.fd;
                ProcessNewMessage(nClientSocket);
            }
        }
    }

    close(epfd);
    return 0;
}
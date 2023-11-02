/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:43:44 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/02 11:57:58 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

int count_set_fds(fd_set *fds, int max_fd) {
    int count = 0;
    for (int i = 0; i <= max_fd; i++) {
        if (FD_ISSET(i, fds)) {
            count++;
        }
    }
    return count;
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
	int nSocket = socket(AF_INET, SOCK_STREAM, 0);
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

	nMaxFd = nSocket;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while (1)
	{
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);
		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);

		std::cout << "Before select call: " << count_set_fds(&fr, nMaxFd + 1) << std:: endl;
		//Keep waiting for new requests and proceed as per the request
		nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);
		if (nRet > 0)
		{
			// when someone connect  or cmuunicaes with a message over
			// a dedicated communication
		}
		else if (nRet == 0)
		{
			//No connection or any communication request made or you can 
			// say that none of the socket descriptors are ready
			std::cout << "Nothing on port: " << port << std:: endl;

		}
		else
		{
			//it failed and your application should show some useful message
			std::cout << "It failed" << std:: endl;
			exit(EXIT_FAILURE);

		}

		std::cout << "After select call: " << count_set_fds(&fr, nMaxFd + 1) << std:: endl;
	}
}
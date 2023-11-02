/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:43:44 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/02 18:04:42 by nwyseur          ###   ########.fr       */
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
		send(ClientSocket, "Processed your request", 23, 0);
		std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
	}
} 

void ProcessTheNewRequest()
{
	//New connection request
	if (FD_ISSET(nSocket, &fr))
	{
		socklen_t nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nSocket, NULL, &nLen); 
		//nSocket = listener adress through which using this fd you're opening 
		//a channel able to receive request from the client but you can't comminicate through this channel
		// using the accept function oyou will get a new client socket a new socket id to communicate with the client
		if (nClientSocket > 0)
		{
			// put it into the client fd_set.
			int nIndex;
			for (nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nArrClient[nIndex] == 0)
				{
					nArrClient[nIndex] = nClientSocket;
					send(nClientSocket, "Got the connection done successfully", 37, 0);
					break;
				}
			}
			if (nIndex == 5)
				std::cout << "No space for a new connection" << std::endl;
		}
	}
	else
	{
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if( FD_ISSET(nArrClient[nIndex], &fr))
			{
				//got a new message from the client
				//Just recv new message
				//just queue that for new worker of your server to fullfill the request
				ProcessNewMessage(nArrClient[nIndex]);
			}
		}
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


		//the new client socket have to be put in the fd set
		for(int nIndex = 0; nIndex < 5; nIndex++)
		{
			if ( nArrClient[nIndex] != 0)
			{
				FD_SET(nArrClient[nIndex], &fr);
				FD_SET(nArrClient[nIndex], &fe);
			}
		}
		//std::cout << "Before select call: " << count_set_fds(&fr, nMaxFd + 1) << std:: endl;
		//Keep waiting for new requests and proceed as per the request
		nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);
		if (nRet > 0)
		{
			// when someone connect  or comunicates with a message over
			// a dedicated communication

			//Please remember that the socket listening to new clients
			//and then comminucating with the same client later are not the same.
			//After connection, you get one more socket to communicate with client
			//You need to send/recv message over the network using that new socket.
			std::cout << "Data on port....Processing now..." << std::endl;
			ProcessTheNewRequest();
		}
		else if (nRet == 0)
		{
			//No connection or any communication request made or you can 
			// say that none of the socket descriptors are ready
			//std::cout << "Nothing on port: " << port << std:: endl;
		}
		else
		{
			//it failed and your application should show some useful message
			std::cout << "It failed" << std:: endl;
			exit(EXIT_FAILURE);

		}

		//std::cout << "After select call: " << count_set_fds(&fr, nMaxFd + 1) << std:: endl;
	}
}
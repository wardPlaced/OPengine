#include "./Communication/include/OPnetworkServer.h"
#include "./Communication/include/OPnetworkSocket.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"
#include "./Data/include/OPstring.h"
#include "./Communication/include/OPnetworkState.h"

void OPnetworkServer::Init(OPnetworkProtocolType::Enum protocol, ui32 port) {

    protocolType = protocol;

	// On Windows the network has to be initialized
	// Doesn't actually do anything on Unix based systems
	if (OPnetwork::Initialize() != 0) {
        OPlogErr("[SERVER] Network could not be initialized");
		return;
	}



    // Gets the Loopback address on port
    OPnetworkAddress local4 = OPnetworkAddress(port, OPnetworkFamily::INET);
    if(!local4.valid) {
        OPlogErr("[SERVER] couldn't locate loopback address for port %d", port);
        return;
    }

    OPnetworkAddress local6 = OPnetworkAddress(port, OPnetworkFamily::INET6);
    if(!local6.valid) {
        OPlogErr("[SERVER] couldn't locate loopback address for port %d", port);
        return;
    }

    serverSocket4 = OPnetworkSocket(local4, protocol);
    if(!serverSocket4.valid) {
        return;
    }

    serverSocket6 = OPnetworkSocket(local6, protocol);
    if(!serverSocket6.valid) {
        return;
    }

    OPlogInfo("[SERVER] Socket created for loopback on port %d", port);

    serverSocket4.Bind();
    serverSocket6.Bind();

    for(ui32 i = 0; i < MAX_CLIENTS; i++) {
	    //OPbzero(&clients[i], sizeof(clients[i]));
        clients[i] = OPnetworkSocket();
    }
    
    // Only does the full listen if TCP
    serverSocket4.Listen();
    serverSocket6.Listen();

    printf("[SERVER] listening\n");
}

void OPnetworkServer::HandleServerReceive(OPnetworkSocket* serverSocket) {
    if(protocolType == OPnetworkProtocolType::TCP) {
        // New TCP Connection

        OPlogInfo("[SERVER] Handling new TCP connection");
        if(serverSocket->Accept(&clients[clientIndex])) {
            clients[clientIndex].GenCode();

            OPnetworkPacket packetCode;
            packetCode.I8(clients[clientIndex].code);
            clients[clientIndex].Send(&packetCode);

            clientIndex++;
        }
    } else {

        // Handle UDP Communication

		clients[clientIndex].networkPacket.buffer.Init(clients[clientIndex].networkPacket.internal_buffer, MAX_PACKET_SIZE_DOUBLE);
        clients[clientIndex].networkPacket.buffer.Zero();
        i32 bytes = serverSocket->ReceiveFrom(&clients[clientIndex]);
        clients[clientIndex].networkPacket.buffer.Rewind();

        // Look for an existing client socket
        OPnetworkSocket* existingClient = NULL;
        for(ui32 i = 0; i < clientIndex; i++) {
            if(clients[i].Match(&clients[clientIndex])) {
                existingClient = &clients[i];
                // write networkPacket buffer into existing
                existingClient->networkPacket.buffer.Write(&clients[clientIndex].networkPacket.buffer);
                existingClient->networkPacket.buffer.Rewind();
                break;
            }
        }

        // If no existing client could be found and the message received was CONNECT
        if(existingClient == NULL && OPstringEquals("CONNECT", clients[clientIndex].networkPacket.Str())) {
            OPlogInfo("[SERVER] New Client Found!");

            existingClient = &clients[clientIndex];
            existingClient->GenCode();

            // Send client their code to verify they are who they say there
            OPnetworkPacket packetCode;
            packetCode.I8(existingClient->code);
#ifdef _DEBUG
			if (simulatedLag) {
				// Sleep(simulatedLag + OPrandom() * simulatedJitter);
			}
#endif
            serverSocket->Send(existingClient, &packetCode);
            OPlogInfo("[SERVER] Sent Code '%d' to Client to Verify", existingClient->code);

            clientIndex++;
        } else if(existingClient != NULL) {
            if(!existingClient->verified) {
				// ui8 code = existingClient->networkPacket.buffer.PeekI8();
#ifdef _DEBUG
				//if (simulatedLag) {
				//	Sleep(simulatedLag + OPrandom() * simulatedJitter);
				//}

				//if (OPrandom() < simulatedPacketLossPercent) {
				//	OPlogErr("[SERVER] (SIMULATED) Packet Loss");
				//	return;
				//}
#endif
                if(existingClient->Verify(existingClient->networkPacket.UI8())) {
                    OPlogInfo("[SERVER] Server Verified the client");
                    if(ActiveNetworkState != NULL) {
                        ActiveNetworkState->Connected(existingClient);
                    }
                } else {
                    OPlogErr("[SERVER] Failed to verify client '%d'", existingClient->code);
                }
            } else if(ActiveNetworkState != NULL) {
                existingClient->timeoutTimer = 0; // reset timeout
                i8 header = existingClient->networkPacket.buffer.PeekI8();
                if(header == 0) {
                    // ping/pong ignore
                    existingClient->networkPacket.buffer.I8(); // read forward
                    OPlogInfo("[SERVER] Received ping/pong back");
                } else {
#ifdef _DEBUG
					//if (simulatedLag) {
					//	Sleep(simulatedLag + OPrandom() * simulatedJitter);
					//}

					//if (OPrandom() < simulatedPacketLossPercent) {
					//	// OPlogErr("[SERVER] (SIMULATED) Packet Loss");
					//	OPlg("*");
					//	existingClient->networkPacket.buffer.FastForward();
					//	return;
					//}
#endif
                    ActiveNetworkState->Message(existingClient, &existingClient->networkPacket);
                }

                existingClient->networkPacket.buffer.FastForward();
            }
        }

    }
}

void OPnetworkServer::SendPingPacket(OPnetworkSocket* networkSocket) {
    OPnetworkPacket packet = OPnetworkPacket();
    packet.I8(0); // ping/pong message
    Send(networkSocket, &packet);
}

void OPnetworkServer::Update(ui64 elapsed) {

    for(ui32 i = 0; i < clientIndex; i++) {
        if(clients[i].verified) {
            //clients[i].timeoutTimer += elapsed;
            //if(clients[i].timeoutTimer > 5000) {
            //    // client hasn't sent anything in 5 seconds
            //    // they've timed out at this point
            //    if(ActiveNetworkState != NULL) {
            //        ActiveNetworkState->Disconnected(&clients[i]);
            //    }
            //    if(i < clientIndex - 1) {
            //        clients[i] = clients[clientIndex - 1];
            //    }
            //    clientIndex--;
            //    i--;
            //     OPlogInfo("Client timed out");
            //} else if(clients[i].timeoutTimer > 2500) {
            //     // client is timing out, send ping/pong to keep alive
            //     SendPingPacket(&clients[i]);
            //     OPlogInfo("Client timing out sending ping/pong");
            //}

            continue;
        }

        clients[i].verifyTimer -= elapsed;
        if(clients[i].verifyTimer <= 0) {
            // failed to verify in time
            if(i < clientIndex - 1) {
                clients[i] = clients[clientIndex - 1];
                i--;
            }
            clientIndex--;
        }
    }

    selector.Zero();
    selector.SetRead(&serverSocket4);
    selector.SetRead(&serverSocket6);

    if(protocolType == OPnetworkProtocolType::TCP) {
        for(ui32 i = 0; i < clientIndex; i++) {
            selector.SetRead(&clients[i]);
        }
    }

    i32 selectResult = selector.Select();
    if(selectResult > 0) {

        if(selector.IsReadSet(&serverSocket4)) {
            HandleServerReceive(&serverSocket4);
        }

        if(selector.IsReadSet(&serverSocket6)) {
            HandleServerReceive(&serverSocket6);
        }

        // TODO: (garrett) put this on the server itself
        if(protocolType == OPnetworkProtocolType::TCP) {
            for(ui32 i = 0; i < clientIndex; i++) {
                if(selector.IsReadSet(&clients[i])) {
                    i32 bytes = clients[i].Receive();
                    clients[i].networkPacket.buffer.Rewind();

                    if(bytes == 0) {
                        if(ActiveNetworkState != NULL) {
                            ActiveNetworkState->Disconnected(&clients[i]);
                        }

                        if(i != clientIndex - 1) {
                            clients[i] = clients[clientIndex - 1];
                            i--;
                        }
                        clientIndex--;
                    } else {
                        if(!clients[i].verified) {
                            if(clients[i].Verify(clients[i].networkPacket.UI8())) {
                                OPlogInfo("[SERVER] Server Verified the client");
                                if(ActiveNetworkState != NULL) {
                                    ActiveNetworkState->Connected(&clients[i]);
                                }
                            } else {
                                OPlogErr("[SERVER] Failed to verify client '%d'", clients[i].code);
                            }
                        } else if(ActiveNetworkState != NULL) {
                            OPlogInfo("[SERVER] Received client message");
                            clients[i].timeoutTimer = 0; // reset timeout
                            if(clients[i].networkPacket.buffer.PeekI8() == 0) {
                                // ping/pong ignore
                                clients[i].networkPacket.buffer.I8(); // read forward
							    OPlogInfo("[SERVER] Received ping/pong back");
                            } else {
                                // pass message along
                                ActiveNetworkState->Message(&clients[i], &clients[i].networkPacket);
                            }

                            clients[i].networkPacket.buffer.FastForward();
                        }
                    }
                }
            }
        }
    } else if(selectResult == -1) {
        OPlogErr("Failed to select");
    }
}

bool OPnetworkServer::Send(OPnetworkPacket* packet) {

    if(protocolType == OPnetworkProtocolType::TCP) {
        for(ui32 i = 0; i < clientIndex; i++) {
            if(clients[i].verified)
                clients[i].Send(packet);
        }
    } else {
        for(ui32 i = 0; i < clientIndex; i++) {
            if(clients[i].verified)
                serverSocket4.Send(&clients[i], packet);
        }
    }
	return true;
}

bool OPnetworkServer::Send(OPnetworkSocket* client, OPnetworkPacket* packet) {
    if(!client->verified) return false;

    if(protocolType == OPnetworkProtocolType::TCP) {
        client->Send(packet);
    } else {
        serverSocket4.Send(client, packet);
    }
}

void OPnetworkServer::Destroy() {

}
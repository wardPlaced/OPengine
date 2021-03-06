#pragma once

#include "./Core/include/OPtypes.h"
#include "./Communication/include/Enums/OPnetworkFamily.h"
#include "./Communication/include/Enums/OPnetworkSocketType.h"
#include "./Communication/include/Enums/OPnetworkProtocolType.h"
#include "./Communication/include/OPnetworkPlatform.h"

struct OPnetworkAddress {
    OPchar networkAddressStr[128];
    ui32 networkPort = 0;
    OPchar networkPortStr[6];
    OPnetworkFamily::Enum networkFamily;
    
	bool valid;

    OPnetworkAddress() { }
    OPnetworkAddress(const OPchar* address, ui32 port) {
        Init(address, port);
    }
    OPnetworkAddress(ui32 port, OPnetworkFamily::Enum networkFamily) {
        Init(port, networkFamily);
    }
    
    OPnetworkAddress(struct sockaddr_storage* sockAddr) {
        Init(sockAddr);
    }
    
    void Init(ui32 port, OPnetworkFamily::Enum networkFamily);
    void Init(struct sockaddr_storage* sockAddr);
    void Init(const OPchar* address, ui32 port);
    void Destroy();
};
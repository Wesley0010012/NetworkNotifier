#ifndef MONITOR_H

#define MONITOR_H

#include "notification.h"

typedef enum
{
    ERROR = -1,
    NETWORK_OFFLINE = 0,
    NETWORK_ONLINE = 1
} NetworkStatus;

NetworkStatus getNetworkStatus(void);

Notification *createNetworkNotification(NetworkStatus status);

#endif
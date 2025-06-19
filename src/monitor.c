#include "monitor.h"
#include <stdlib.h>
#include <sys/wait.h>

NetworkStatus getNetworkStatus(void)
{
    int status = system("ping -c 1 -W 1 8.8.8.8 > /dev/null 2>&1");

    if (status == -1)
    {
        return ERROR;
    }

    if (WIFEXITED(status))
    {
        switch (WEXITSTATUS(status))
        {
        case 0:
            return NETWORK_ONLINE;
        case 1:
        case 2:
            return NETWORK_OFFLINE;
        }
    }

    return ERROR;
}

Notification *createNetworkNotification(NetworkStatus status)
{
    const char *title = NULL;
    const char *message = NULL;

    switch (status)
    {
    case NETWORK_ONLINE:
        title = "🟢 ONLINE";
        message = "Your network connection is active and working properly.";
        break;
    case NETWORK_OFFLINE:
        title = "🔴 OFFLINE";
        message = "No network connection detected. Please check your cables or Wi-Fi.";
        break;
    default:
        title = "⚠️ ERROR";
        message = "Unable to determine network status. Please check your system settings.";
        break;
    }

    return makeNotification(title, message);
}
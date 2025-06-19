#include "monitor.h"
#include "notification.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

#define DEFAULT_SLEEP 1
#define LIMIT_ERRORS 30

void app(void);

void runAsDaemon(void);

int main(void)
{
    app();

    return 0;
}

void runAsDaemon(void)
{
    pid_t pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);

    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void app(void)
{
    runAsDaemon();

    int errorsCounter = 0;

    NetworkStatus latestState = ERROR;

    while (1)
    {
        NetworkStatus actualState = getNetworkStatus();

        if (actualState == ERROR)
        {
            if (latestState != ERROR)
            {
                Notification *networkNotification = createNetworkNotification(ERROR);

                notify(networkNotification);

                freeNotification(&networkNotification);

                latestState = ERROR;
            }

            errorsCounter++;

            if (errorsCounter >= LIMIT_ERRORS)
            {
                Notification *notification = makeNotification(
                    "⚠️ ERROR",
                    "Unable to determine network status. Exiting and restarting monitor.");

                notify(notification);
                
                freeNotification(&notification);

                exit(EXIT_FAILURE);
            }
        }
        else
        {
            errorsCounter = 0;
            if (actualState != latestState)
            {
                Notification *notification = createNetworkNotification(actualState);

                notify(notification);

                freeNotification(&notification);

                latestState = actualState;
            }
        }

        sleep(DEFAULT_SLEEP);
    }
}
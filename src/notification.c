#include "notification.h"
#include <stdio.h>
#include <stdlib.h>

void logNotification(Notification *notification);

void notify(Notification *notification)
{
    char comando[256];
    snprintf(comando, sizeof(comando), "notify-send \"%s\" \"%s\"", notification->title, notification->body);
    system(comando);

    logNotification(notification);

    return;
}

Notification *makeNotification(const char *title, const char *body)
{
    Notification *notification = malloc(sizeof(Notification));
    notification->title = title;
    notification->body = body;

    return notification;
}

void logNotification(Notification *notification)
{
    FILE *f = fopen("/tmp/networks_notification.log", "a");
    if (f)
    {
        fprintf(f, "%s - %s\n", notification->title, notification->body);
        fclose(f);
    }
}

void freeNotification(Notification **notification)
{
    free((*notification));

    *notification = NULL;
}
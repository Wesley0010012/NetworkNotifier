#ifndef NOTIFICATION_H

#define NOTIFICATION_H

typedef struct
{
    const char *title;
    const char *body;
} Notification;

void notify(Notification *notification);

Notification *makeNotification(const char *title, const char *body);

void freeNotification(Notification **notification);

#endif
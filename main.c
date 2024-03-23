#include <iso646.h>
#include <libnotify/notification.h>
#include <libnotify/notify.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  char *summary;
  char *body;
  char *icon;
  NotifyUrgency urgency;
} notification_details;

#define Buffer_size 128
#define EXT_FILE_NOT_ACCESSED 1
#define EXT_READING_ERROR 2

int battery_percentage(int *display);
int charger_status(int *prev_state);
void send_notifications(int a);

int battery_percentage(int *displayed) {
  FILE *fp;
  char *Buffer = malloc(Buffer_size * (sizeof(char)));
  if ((fp = fopen("/sys/class/power_supply/BAT0/capacity", "r")) == NULL) {
    perror("file not opened");
    exit(EXT_FILE_NOT_ACCESSED);
  }
  if (fgets(Buffer, Buffer_size, fp) == NULL) {
    perror("error reding file");
    exit(EXT_READING_ERROR);
  }
  long percentage = strtol(Buffer, NULL, 10);
  if (*displayed == 0 && percentage <= 15) {
    send_notifications(2);
    (*displayed)++;
  }
  if (*displayed == 1 && percentage > 15)
    (*displayed)--;
  fclose(fp);
  free(Buffer);
  return 0;
}

int charger_status(int *prev_state) {
  FILE *fp;
  int pres_state;
  if ((fp = fopen("/sys/class/power_supply/ACAD/online", "r")) == NULL) {
    perror("file not opened");
    exit(1);
  }
  if ((fscanf(fp, "%d", &pres_state)) != 1) {
    perror("error reading files");
    exit(1);
  }
  if (*prev_state == 2)
    *prev_state = pres_state;
  int xor_result = pres_state ^ *prev_state;
  if (xor_result == 1) {
    switch (pres_state) {
    case 0:
      send_notifications(1);
      break;
    case 1:
      send_notifications(0);
    }
    *prev_state = pres_state;
  }
  fclose(fp);
  return 0;
}

void send_notifications(int a) {
  notification_details notifyData[] = {
      {"Charger Conected", "", "./icons8-connected-50.png",
       NOTIFY_URGENCY_NORMAL},
      {"Charger Disconected", "", "./icons8-disconnected-50.png",
       NOTIFY_URGENCY_NORMAL},
      {"Battery Low", "Battery at 15%,connect Charger",
       "./icons8-battery-low-50.png", NOTIFY_URGENCY_CRITICAL},
  };
  notify_init("Linux Notifications");
  NotifyNotification *notif = notify_notification_new(
      notifyData[a].summary, notifyData[a].body, notifyData[a].icon);
  notify_notification_set_urgency(notif, notifyData[a].urgency);
  notify_notification_show(notif, NULL);
  g_object_unref(G_OBJECT(notif));
  notify_uninit();
}

int main() {
  int a = 0;
  int *displayed = &(a);
  int b = 2;
  int *prev_state = &(b);
  while (1) {
    battery_percentage(displayed);
    charger_status(prev_state);
  }
  return 0;
}

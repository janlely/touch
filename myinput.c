#include <libinput.h>
#include <errno.h>

static int open_restricted(const char *path, int flags, void *user_data)
{
        int fd = open(path, flags);
        return fd < 0 ? -errno : fd;
}
static void close_restricted(int fd, void *user_data)
{
        close(fd);
}
const static struct libinput_interface interface = {
        .open_restricted = open_restricted,
        .close_restricted = close_restricted,
};

int main(void) {
        struct udev *udev;
        struct libinput *li;
        struct libinput_event *event;
        udev = udev_new();
        if (!udev) {
            printf("Can't create udev\n");
            exit(1);
        }
        li = libinput_udev_create_context(&interface, NULL, udev);
        libinput_udev_assign_seat(li, "seat0");
        libinput_dispatch(li);
        while ((event = libinput_get_event(li)) != NULL) {
                // handle the event here
                printf("hello world");
                libinput_event_destroy(event);
                libinput_dispatch(li);
        }
        libinput_unref(li);
        return 0;
}


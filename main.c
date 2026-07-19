#include <signal.h>
#include<v4l2.h>
static volatile int g_running = 1;
void signal_handler(int sig)
{
    (void)sig;
    g_running = 0;
}

DmaBuffer dma_bufs[BUF_NUM] = {0};


int main(int argc, char **argv)
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    const char *v4l2_dev = "/dev/video9";
    int fd = open(v4l2_dev, O_RDWR);
    if(fd < 0)
    {
        perror("open v4l2 device");
        return 1;
    }
    if(!capability(fd))
    {
        close(fd);
        return 1;
    }
    capfmt fmt_li={0};
    fs fs_li={0};
    fi fi_li={0};
    if(!select_v4l2(fd, &fmt_li, &fs_li, &fi_li))
    {
        close(fd);
        return 1;
    }
    if(!set_fmt(fd, &fmt_li, &fs_li))
    {
        close(fd);
        return 1;
    }
    if(!set_streamparm(fd, &fi_li))
    {
        close(fd);
        return 1;
    }
    
    if (!qbuf(fd, dma_bufs))
    {
        close(fd);
        return 1;
    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(fd, VIDIOC_STREAMON, &type) < 0)
    {
        perror("VIDIOC_STREAMON");  
        close(fd);
        return 1;
    }
    FILE *fp = fopen("frame.mjpg", "ab");
    if(!fp)    {
        perror("fopen");
        return 0;
    }
    while(g_running)
    {
        if(!dqbuf(fd, dma_bufs, fp))
        {
            ioctl(fd, VIDIOC_STREAMOFF, &type);
            fclose(fp);
            close(fd);
            return 1;
        }
    }
    ioctl(fd, VIDIOC_STREAMOFF, &type);
    fclose(fp);
    close(fd);
    return 0;
}

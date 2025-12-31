
// main/video_stub.c
#include "common.h"

// 无摄像头模式下提供空实现，避免链接错误
int start_video_proc(void)
{
    return 0;
}

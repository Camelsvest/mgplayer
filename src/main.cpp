#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "mgplayerwindow.h"
#include "logging.h"

int MiniGUIMain(int agrc, const char *argv[])
{
    MGPlayerWindow mainWnd;
    
    logging_init(NULL);    
    ::ncsInitialize();

    mainWnd.create(0, 0, RECTW(g_rcDesktop), RECTH(g_rcDesktop));
    mainWnd.run();

    ::ncsUninitialize();
    logging_uninit();
    
    return 0;
}

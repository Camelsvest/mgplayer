#include "videowindow.h"
#include "mgplayerwindef.h"
#include "logging.h"

NCS_EVENT_HANDLER VideoWindow::m_evHandlers[] = {
    {MSG_CREATE, (void *)VideoWindow::onEvCreate},
    {MSG_PAINT, (void *)VideoWindow::onEvPaint},
    {MSG_DESTROY, (void *)VideoWindow::onEvDestroy},
    {0, NULL},
};

VideoWindow::VideoWindow()
    : m_videoWdgt(NULL)
{
}

VideoWindow::~VideoWindow()
{
}

bool VideoWindow::create(HWND parent, RECT *rc)
{
    NCS_RDR_INFO rdrInfo [] = {
        {"classic", "skin", NULL}
    };
    
    m_videoWdgt = ::ncsCreateWindow(
        NCSCTRL_PANEL,
        DEFAULT_VIDEO_WINDOW_CAPTION,
        WS_CHILD|WS_VISIBLE|WS_BORDER, 
        WS_EX_NONE,
        VIDEO_WINDOW_ID, 
        rc->left,
        rc->top,
        RECTWP(rc),
        RECTHP(rc),
        parent,
        NULL,
        rdrInfo,
        m_evHandlers,
        (DWORD)this);

    return (m_videoWdgt != NULL);
}

BOOL VideoWindow::onEvCreate(mWidget *_this, DWORD dwAddData)
{
    VideoWindow *pThis = (VideoWindow *)(dwAddData);
    if (pThis)
        return pThis->onCreate(_this->hwnd, dwAddData);
    else
        return FALSE;
}

BOOL VideoWindow::onCreate(HWND hwnd, DWORD dwAddData)
{
    RECT rc;

    if (!addMap(VIDEO_WINDOW_ID, this))
    {
        logging_error("Failed to add object \"VideoWindow\" %p into COLLECTION.\r\n", this);
        return false;
    }
    
    ::GetClientRect(hwnd, &rc);
    
    return TRUE;
}


void VideoWindow::onEvPaint(mWidget *_this, HDC hdc, const CLIPRGN* inv)
{
    VideoWindow *pThis = getThisObject(_this);
    if (pThis)
        pThis->onPaint(_this->hwnd, hdc, inv);
}

void VideoWindow::onPaint(HWND hwnd, HDC hdc, const CLIPRGN *inv)
{
}

BOOL VideoWindow::onEvDestroy(mWidget* _this, int message)
{
    VideoWindow *pThis = getThisObject(_this);
    if (pThis)
        return pThis->onDestroy(_this->hwnd, message);
    else
        return FALSE;
}

BOOL VideoWindow::onDestroy(HWND hwnd, int message)
{
    BOOL ret = removeMap(VIDEO_WINDOW_ID, this);

    if (!ret)
        logging_error("Failed to remove object \"VideoWindow\" %p from COLLECTION.\r\n", this);

    return ret;
}


VideoWindow* VideoWindow::getThisObject(mWidget *_this)
{
    int id;
    VideoWindow *pThis = NULL;

    id = _c(_this)->getId(_this);
    if (id != 0)
    {
        pThis = dynamic_cast<VideoWindow *>(getWindowObject(id));
    }

    return pThis;
}



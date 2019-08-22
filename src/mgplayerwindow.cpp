#include "mgplayerwindow.h"
#include "mgplayerwindef.h"
#include "logging.h"

NCS_EVENT_HANDLER MGPlayerWindow::m_evHandlers[] = {
    {MSG_CREATE, (void *)MGPlayerWindow::onEvCreate},
    {MSG_PAINT, (void *)MGPlayerWindow::onEvPaint},
    {MSG_CLOSE, (void *)MGPlayerWindow::onEvClose},
    {MSG_DESTROY, (void *)MGPlayerWindow::onEvDestroy},
    {0, NULL}
};

MGPlayerWindow::MGPlayerWindow(const char *caption)
    : m_mainWdgt(NULL)
{
    if (caption == NULL)
        m_caption = DEFAULT_WINDOW_CAPTION;
    else
        m_caption = caption;
}

MGPlayerWindow::~MGPlayerWindow()
{
}


bool MGPlayerWindow::create(int x, int y, int w, int h)
{
    bool succeed;
       
    m_mainWdgt = ::ncsCreateMainWindow(
        NCSCTRL_MAINWND,                        // className
        m_caption.c_str(),                      // caption
        WS_CAPTION | WS_BORDER | WS_VISIBLE,    // style
        WS_EX_NONE,                             // exStyle
        MAIN_WINDOW_ID,                         // id
        x,                                      // x
        y,                                      // y
        w,                                      // w
        h,                                      // h
        HWND_DESKTOP,                           // host
        0,                                      // hIcon
        0,                                      // hMenu
        NULL,                                   // props
        NULL,                                   // rdrinfo
        m_evHandlers,                           // handlers
        (DWORD)this);

    succeed = (m_mainWdgt != NULL);
    logging_trace("Create MGPlayerWindow: %s.\r\n", succeed ? "Succeed" : "Failed");

    return succeed;
}

void MGPlayerWindow::run()
{
    MSG msg;
    
    // START_OF_MSGLOOP
    while (GetMessage (&msg, m_mainWdgt->hwnd))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    // END_OF_MSGLOOP

    ::MainWindowThreadCleanup(m_mainWdgt->hwnd);    
}

BOOL MGPlayerWindow::onEvCreate(mWidget *_this, DWORD dwAddData)
{
    MGPlayerWindow *pThis = (MGPlayerWindow *)dwAddData;
    if (pThis)
        return pThis->onCreate(_this->hwnd, dwAddData);
    else
        return FALSE;
}

BOOL MGPlayerWindow::onCreate(HWND hwnd, DWORD dwAddData)
{
    BOOL bRet;
    RECT rc;

    ENTER_CLASS_FUNCTION("MGPlayerWindow");
    
    if (!addMap(MAIN_WINDOW_ID, this))
    {
        logging_error("Failed to add object \"MGPlayerWindow\" %p into COLLECTION.\r\n", this);
        return false;
    }

    ::GetClientRect(hwnd, &rc);
    
    bRet = m_videoWnd.create(hwnd, &rc);
    logging_trace("Create VideoWindow: %s\r\n", bRet ? "Succeed" : "Failed");

    EXIT_CLASS_FUNCTION("MGPlayerWindow");
    
    return bRet;
}


void MGPlayerWindow::onEvPaint(mWidget *_this, HDC hdc, const CLIPRGN* inv)
{
    MGPlayerWindow *pThis = getThisObject(_this);
    if (pThis)
        pThis->onPaint(_this->hwnd, hdc, inv);
}

void MGPlayerWindow::onPaint(HWND hwnd, HDC hdc, const CLIPRGN *inv)
{
}

BOOL MGPlayerWindow::onEvClose (mWidget* _this, int message)
{
    MGPlayerWindow *pThis = getThisObject(_this);
    if (pThis)
        return pThis->onClose(_this->hwnd, message);
    else
        return FALSE;
}

BOOL MGPlayerWindow::onClose(HWND hwnd, int message)
{
    ENTER_CLASS_FUNCTION("MGPlayerWindow");
    
    DestroyMainWindow(hwnd);

    EXIT_CLASS_FUNCTION("MGPlayerWindow");
    
    return TRUE;
}

BOOL MGPlayerWindow::onEvDestroy(mWidget* _this, int message)
{
    MGPlayerWindow *pThis = getThisObject(_this);
    if (pThis)
        return pThis->onDestroy(_this->hwnd, message);
    else
        return FALSE;
}

BOOL MGPlayerWindow::onDestroy(HWND hwnd, int message)
{
    BOOL ret;

    ENTER_CLASS_FUNCTION("MGPlayerWindow");
    
    ret = removeMap(MAIN_WINDOW_ID, this);
    if (!ret)
        logging_error("Failed to remove object \"MGPlayerWindow\" %p into COLLECTION.\r\n", this);

    EXIT_CLASS_FUNCTION("MGPlayerWindow");
    
    return ret;
}


MGPlayerWindow* MGPlayerWindow::getThisObject(mWidget *_this)
{
    int id;
    MGPlayerWindow *pThis = NULL;

    id = _c(_this)->getId(_this);
    if (id != 0)
    {
        pThis = dynamic_cast<MGPlayerWindow *>(getWindowObject(id));
    }

    return pThis;
}

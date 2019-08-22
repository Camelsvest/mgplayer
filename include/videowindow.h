#ifndef _VIDEO_WINDOW_H_
#define _VIDEO_WINDOW_H_

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgncs/mgncs.h>

#include "mgwindow.h"

class VideoWindow : public MGWindow
{
public:
    VideoWindow();
    virtual ~VideoWindow();

    bool create(HWND parent, RECT *rc);
    
protected:
    static BOOL onEvCreate(mWidget *_this, DWORD dwAddData);
    BOOL onCreate(HWND hwnd, DWORD dwAddData);
    
    static void onEvPaint (mWidget *_this, HDC hdc, const CLIPRGN* inv);
    void onPaint(HWND hwnd, HDC hdc, const CLIPRGN* inv);

    static BOOL onEvDestroy(mWidget* _this, int message);
    BOOL onDestroy(HWND hwnd, int message);
    
private:
    static VideoWindow* getThisObject(mWidget *_this);
    static NCS_EVENT_HANDLER m_evHandlers[];
    
    mWidget *m_videoWdgt;
};

#endif


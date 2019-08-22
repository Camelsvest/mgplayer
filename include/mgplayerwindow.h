#ifndef _MG_PLAYER_WINDOW_H_
#define _MG_PLAYER_WINDOW_H_

#include <string>
using namespace std;

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgncs/mgncs.h>

#include "mgwindow.h"
#include "videowindow.h"

class MGPlayerWindow : public MGWindow
{
public:
    MGPlayerWindow(const char *caption = NULL);
    virtual ~MGPlayerWindow();

    bool create(int x, int y, int w, int h);
    void run();

protected:
    static BOOL onEvCreate(mWidget *_this, DWORD dwAddData);
    BOOL onCreate(HWND hwnd, DWORD dwAddData);
    
    static void onEvPaint (mWidget *_this, HDC hdc, const CLIPRGN* inv);
    void onPaint(HWND hwnd, HDC hdc, const CLIPRGN* inv);
    
    static BOOL onEvClose (mWidget* _this, int message);
    BOOL onClose(HWND hwnd, int message);

    static BOOL onEvDestroy(mWidget* _this, int message);
    BOOL onDestroy(HWND hwnd, int message);
    
private:
    static MGPlayerWindow* getThisObject(mWidget *_this);
    
    static NCS_EVENT_HANDLER m_evHandlers[];
    
    std::string m_caption;
    mWidget     *m_mainWdgt;
    VideoWindow m_videoWnd;

};

#endif

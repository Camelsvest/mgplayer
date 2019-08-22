#include <cstddef>
#include "mgwindow.h"

std::map<int, MGWindow *> MGWindow::m_mapWindowObjects;

MGWindow::MGWindow()
{
}

MGWindow::~MGWindow()
{
}

bool MGWindow::addMap(int id, MGWindow* window)
{
    std::map<int, MGWindow *>::iterator it;
    
    it = m_mapWindowObjects.find(id);
    if (it != m_mapWindowObjects.end())
        return false;
    else
    {
        m_mapWindowObjects[id] = window;
        return true;
    }
}

bool MGWindow::removeMap(int id, MGWindow* window)
{
    std::map<int, MGWindow *>::iterator it;
    
    it = m_mapWindowObjects.find(id);
    if (it != m_mapWindowObjects.end())
    {
        m_mapWindowObjects.erase(it);
        return true;
    }
    else
        return false;
}

MGWindow * MGWindow::getWindowObject(int id)
{
    std::map<int, MGWindow *>::iterator it;
    
    it = m_mapWindowObjects.find(id);
    if (it != m_mapWindowObjects.end())
    {
        return it->second;
    }
    else
        return NULL;
}


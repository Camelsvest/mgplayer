#ifndef _MG_WINDOW_H_
#define _MG_WINDOW_H_

#include <map>
using namespace std;

class MGWindow
{
public:
    MGWindow();
    virtual ~MGWindow();

protected:
    static bool addMap(int id, MGWindow* window);
    static bool removeMap(int id, MGWindow* window);
    static MGWindow * getWindowObject(int id);
    
private:
    static std::map<int, MGWindow*> m_mapWindowObjects;       
};

#endif

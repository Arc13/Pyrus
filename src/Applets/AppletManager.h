#ifndef PYRUS_APPLETMANAGER_H
#define PYRUS_APPLETMANAGER_H

#include <memory>
#include <stack>

#include "../Events/BaseEvent.h"

#include "BaseApplet.h"

namespace Applet {

class AppletManager {
public:
    void dispatchEvent(Event::BaseEvent *event);
    void update(Platform::BasePlatform &platform);
    void draw(Hardware::Screen::BaseScreen &target);

    void pushApplet(std::unique_ptr<BaseApplet> applet);
    void popApplet();

    BaseApplet &getCurrentApplet();

    void setAppletChangedHandler(const std::function<void()> &appletChangedHandler);

private:
    std::stack<std::unique_ptr<BaseApplet>> m_applets;

    std::function<void()> m_appletChangedHandler;
}; // class AppletManager

} // namespace Applet

#endif //PYRUS_APPLETMANAGER_H

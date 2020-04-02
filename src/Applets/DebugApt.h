#ifndef PYRUS_DEBUGAPT_H
#define PYRUS_DEBUGAPT_H

#include "../Widgets/Text.h"

#include "BaseApplet.h"

namespace Applet {

class DebugApt: public BaseApplet {
public:
    DebugApt();

    void processEvent() override;
    void update(Platform::BasePlatform &platform) override;
    void draw(Hardware::Screen::BaseScreen &target) override;

    bool showStatusBar() const override;

private:
    Widget::Text m_clkText;
    Widget::Text m_bleText;
}; // class DebugApt

} // namespace Applet

#endif //PYRUS_DEBUGAPT_H
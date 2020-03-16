#ifndef PYRUS_BASEWIDGET_H
#define PYRUS_BASEWIDGET_H

#include "Hardware/Screen/BaseScreen.h"

namespace Widget
{

class BaseContainer;

class BaseWidget {
public:
    virtual ~BaseWidget() = default;

    virtual void draw(Hardware::Screen::BaseScreen &target) = 0;

    virtual void setPosition(Vec2D_t position) = 0;
    virtual const Vec2D_t &getPosition() const = 0;

    virtual Vec2D_t getAbsolutePosition() const = 0;
    virtual Vec2D_t getSize() const = 0;

    virtual const BaseContainer *getParent() const = 0;

    virtual void markDirty() = 0;
}; // class BaseWidget

} // namespace Widget

#endif //PYRUS_BASEWIDGET_H

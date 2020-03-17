#ifndef PYRUS_BASEWIDGET_H
#define PYRUS_BASEWIDGET_H

#include <string>

#include "Hardware/Screen/BaseScreen.h"

namespace Widget
{

class BaseContainer;

class BaseWidget {
public:
    BaseWidget(std::string id, Vec2D_t position);
    virtual ~BaseWidget() = default;

    virtual void draw(Hardware::Screen::BaseScreen &target) = 0;

    virtual void setPosition(Vec2D_t position);
    virtual const Vec2D_t &getPosition() const;

    virtual Vec2D_t getAbsolutePosition() const = 0;

    virtual uint16_t getWidth() const = 0;
    virtual uint16_t getHeight() const = 0;
    virtual Vec2D_t getSize() const = 0;

    virtual void setParent(BaseContainer *parent);
    virtual const BaseContainer *getParent() const;

    virtual const std::string &getId() const;

    virtual void markDirty();

protected:
    BaseContainer *m_parent;
    const std::string m_id;

    bool m_dirty;
    bool m_clearLastPosition;

    Vec2D_t m_position;

}; // class BaseWidget

} // namespace Widget

#endif //PYRUS_BASEWIDGET_H

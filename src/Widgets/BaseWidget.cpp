#include <utility>

#include "BaseContainer.h"
#include "BaseWidget.h"

Widget::BaseWidget::BaseWidget(std::string id, const Graphics::Vec2D position)
: m_parent(nullptr)
, m_id(std::move(id))
, m_dirty(1 << DirtyState::Global)
, m_position(position)
, m_loopVerticalPosition(false)
{

}

void Widget::BaseWidget::setPosition(Graphics::Vec2D position)
{
    m_position = position;

    setDirty(DirtyState::Position, true);
}

const Graphics::Vec2D &Widget::BaseWidget::getPosition() const
{
    return m_position;
}

void Widget::BaseWidget::setLoopVerticalPosition(bool loopVerticalPosition)
{
    m_loopVerticalPosition = loopVerticalPosition;
}

bool Widget::BaseWidget::isLoopingVerticalPosition() const
{
    return m_loopVerticalPosition;
}

void Widget::BaseWidget::setParent(Widget::BaseContainer *parent)
{
    m_parent = parent;
}

const Widget::BaseContainer *Widget::BaseWidget::getParent() const
{
    return m_parent;
}

const std::string &Widget::BaseWidget::getId() const
{
    return m_id;
}

void Widget::BaseWidget::setDirty(DirtyState state, bool dirty)
{
    m_dirty.set(state, dirty);

    if (m_parent != nullptr)
        m_parent->setDirty(DirtyState::Child, true);
}

void Widget::BaseWidget::clearDirty()
{
    m_dirty.reset();
}

bool Widget::BaseWidget::isDirty(Widget::BaseWidget::DirtyState state)
{
    return m_dirty.test(state);
}

Graphics::Color Widget::BaseWidget::getParentBackgroundColor() const
{
    if (m_parent == nullptr)
        return {0, 0, 0};

    return m_parent->getBackgroundColor();
}

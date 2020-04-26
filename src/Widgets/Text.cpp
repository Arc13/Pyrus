#include "../Graphics/GfxUtils.h"
#include "Container.h"

#include "Text.h"

#define INTERCHAR_SIZE 2

Widget::Text::Text(const std::string &id, const std::string &text, const FONT_INFO *fontInfo, Graphics::Vec2D position,
                   const Graphics::Color &textColor, const Graphics::Color &backgroundColor)
: BaseWidget(id, position)
, m_text(text)
, m_fontInfo(fontInfo)
, m_horizontalAlignment(HorizontalAlignment::Left)
, m_lastDrawPosition({0, 0})
, m_lastSize({0, 0})
, m_textColor(textColor)
, m_backgroundColor(backgroundColor)
, m_width(0)
{
    m_fontInfo = fontInfo;
}

void Widget::Text::draw(Hardware::Screen::BaseScreen &target)
{
    // Dirty flag not set, we don't need to redraw the widget
    if (m_dirty == 0) return;

    // Geometry has changed, we need to clear the last occupied space
    if (isDirty(DirtyState::Global) || isDirty(DirtyState::Position) || isDirty(DirtyState::Size))
    {
        Graphics::Vec2D lastAbsolutePosition = getLastAbsolutePosition();
        if (m_loopVerticalPosition)
            lastAbsolutePosition.y %= target.getFramebufferSize().y;

        Graphics::GfxUtils::drawFilledRectangle(target, lastAbsolutePosition, m_lastSize,
                                                getParentBackgroundColor(), m_loopVerticalPosition);
    }

    // Recompute width if it's dirty
    if (isDirty(DirtyState::Global) || isDirty(DirtyState::Size))
    {
        m_width = computeWidth();
    }

    // Store the position of this drawing
    Graphics::Vec2D position = getDrawPosition();
    m_lastDrawPosition = position;

    // Loop the vertical axis if enabled
    if (m_loopVerticalPosition)
        position.y %= target.getFramebufferSize().y;

    // Draw the background
    Graphics::GfxUtils::drawFilledRectangle(target, position, getSize(), m_backgroundColor, m_loopVerticalPosition);

    // Loop through all the characters and draw them
    for (const char c : m_text)
    {
        if (c != ' ')
        {
            uint16_t charWidth = Graphics::GfxUtils::drawChar(target, position, c, *m_fontInfo, m_textColor,
                    m_backgroundColor, m_loopVerticalPosition);
            position.x += charWidth;
        }
        else
        {
            position.x += m_fontInfo->spacePixels;
        }

        position.x += INTERCHAR_SIZE;
    }

    // Store the size of this drawing
    // Note that m_lastDrawPosition is used here because it holds the initial values of position
    m_lastSize = {static_cast<int16_t>(position.x - m_lastDrawPosition.x), m_fontInfo->height};

    // Reset the dirty flag
    clearDirty();
}

void Widget::Text::setText(const std::string &text)
{
    m_text = text;

    setDirty(DirtyState::Size, true);
}

const std::string &Widget::Text::getText() const
{
    return m_text;
}

void Widget::Text::setFont(const FONT_INFO *fontInfo)
{
    m_fontInfo = fontInfo;

    setDirty(DirtyState::Size, true);
}

const FONT_INFO *Widget::Text::getFont() const
{
    return m_fontInfo;
}

Graphics::Vec2D Widget::Text::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

Graphics::Vec2D Widget::Text::getDrawPosition() const
{
    Graphics::Vec2D position = getAbsolutePosition();

    switch (m_horizontalAlignment)
    {
        case HorizontalAlignment::Centered:
            return {static_cast<int16_t>(position.x - getWidth() / 2), position.y};
        case HorizontalAlignment::Right:
            return {static_cast<int16_t>(position.x - getWidth()), position.y};
        default:
            return position;
    }
}

int16_t Widget::Text::getWidth() const
{
    return m_width;
}

int16_t Widget::Text::getHeight() const
{
    return m_fontInfo->height;
}

Graphics::Vec2D Widget::Text::getSize() const
{
    return {getWidth(), getHeight()};
}

void Widget::Text::setTextColor(const Graphics::Color &textColor)
{
    m_textColor = textColor;

    setDirty(DirtyState::Color, true);
}

const Graphics::Color &Widget::Text::getTextColor() const
{
    return m_textColor;
}

void Widget::Text::setBackgroundColor(const Graphics::Color &backgroundColor)
{
    m_backgroundColor = backgroundColor;

    setDirty(DirtyState::Color, true);
}

const Graphics::Color &Widget::Text::getBackgroundColor() const
{
    return m_backgroundColor;
}

Graphics::Vec2D Widget::Text::getLastAbsolutePosition() const
{
    if (m_parent == nullptr)
        return m_lastDrawPosition;

    return m_parent->getAbsolutePosition() + m_lastDrawPosition;
}

uint16_t Widget::Text::computeWidth() const
{
    // Prepare the width
    uint16_t width = 0;

    // Loop through all the characters and count their width
    for (const char c : m_text)
    {
        if (c != ' ')
        {
            const size_t descriptorOffset = c - m_fontInfo->startChar;
            const FONT_CHAR_INFO descriptor = m_fontInfo->charInfo[descriptorOffset];

            width += descriptor.widthBits;
        }
        else
        {
            width += m_fontInfo->spacePixels;
        }

        width += INTERCHAR_SIZE;
    }

    // Return the width
    return width;
}

void Widget::Text::setHorizontalAlignment(Widget::Text::HorizontalAlignment horizontalAlignment)
{
    m_horizontalAlignment = horizontalAlignment;

    setDirty(DirtyState::Position, true);
}

Widget::Text::HorizontalAlignment Widget::Text::getHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

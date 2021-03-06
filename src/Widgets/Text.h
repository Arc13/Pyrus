#ifndef PYRUS_TEXT_H
#define PYRUS_TEXT_H

#include <string>

#include "../Graphics/GfxUtils2.h"

#include "BaseWidget.h"

namespace Widget
{

class Text : public BaseWidget {
public:
    enum HorizontalAlignment {
        Left,
        Centered,
        Right,
    };

    Text(const std::string &id, std::string text, const FONT_INFO *fontInfo, Graphics::Vec2D position,
         const Graphics::Color &textColor = {255, 255, 255},
         const Graphics::Color &backgroundColor = {0, 0, 0});
    ~Text() override = default;

    void draw(Hardware::Screen::BaseScreen &target) override;

    void setText(const std::string &text);
    const std::string &getText() const;

    void setFont(const FONT_INFO *fontInfo);
    const FONT_INFO *getFont() const;

    Graphics::Vec2D getAbsolutePosition() const override;
    Graphics::Vec2D getDrawPosition() const;

    int16_t getWidth() const override;
    int16_t getHeight() const override;
    Graphics::Vec2D getSize() const override;

    void setTextColor(const Graphics::Color &textColor);
    const Graphics::Color &getTextColor() const;

    void setBackgroundColor(const Graphics::Color &backgroundColor);
    const Graphics::Color &getBackgroundColor() const;

    void setHorizontalAlignment(HorizontalAlignment horizontalAlignment);
    HorizontalAlignment getHorizontalAlignment() const;

    void setWrapEnabled(bool wrapEnabled);
    bool isWrapEnabled() const;

    void setClippingEnabled(bool clippingEnabled);
    bool isClippingEnabled() const;

    void setClippingStart(const Graphics::Vec2D &clippingStart);
    const Graphics::Vec2D &getClippingStart() const;

    void setClippingEnd(const Graphics::Vec2D &clippingEnd);
    const Graphics::Vec2D &getClippingEnd() const;

private:
    Graphics::Vec2D getLastAbsolutePosition() const;

    uint16_t computeWidth(const std::string &str) const;

    void drawTextAt(Graphics::GfxUtils2 *gfxTarget, Graphics::Vec2D &size);
    void drawStringAt(Graphics::GfxUtils2 *gfxTarget, const std::string &str, Graphics::Vec2D &position,
                      Graphics::Vec2D &size, const Graphics::Vec2D &basePosition);

    std::string m_text;
    const FONT_INFO *m_fontInfo;
    HorizontalAlignment m_horizontalAlignment;

    bool m_wrapEnabled;
    Graphics::Vec2D m_size;

    bool m_clippingEnabled;
    Graphics::Vec2D m_clippingStart;
    Graphics::Vec2D m_clippingEnd;

    Graphics::Vec2D m_oldClippingStart;
    Graphics::Vec2D m_oldClippingEnd;

    Graphics::Vec2D m_lastDrawPosition;
    Graphics::Vec2D m_lastSize;

    Graphics::Color m_textColor;
    Graphics::Color m_backgroundColor;

    uint16_t m_width;

}; // class Text

} // namespace Widget

#endif //PYRUS_TEXT_H

#include <algorithm>
#include <cstring>

#include "GfxUtils.h"

#define BUFFER_SIZE 254

void Graphics::GfxUtils::drawLine(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
                                  Graphics::Vec2D secondPoint, const Graphics::Color &color, bool loopVerticalAxis)
{
    // This is an implementation of the Bresenham's line algorithm

    // Reorder the points
    if (secondPoint.x < firstPoint.x)
        std::swap(firstPoint, secondPoint);

    // Bresenham's algorithm parameters
    const int16_t dx = secondPoint.x - firstPoint.x;
    const int16_t dy = secondPoint.y - firstPoint.y;
    int16_t D = 2 * dy - dx;
    Graphics::Vec2D cursorPosition = firstPoint;

    // Bresenham's algorithm
    for (; cursorPosition.x <= secondPoint.x; ++cursorPosition.x)
    {
        target.drawPixel(cursorPosition, color);

        if (D > 0)
        {
            ++cursorPosition.y;
            D = D - 2 * dx;
        }

        D = D + 2 * dy;
    }
}

void Graphics::GfxUtils::drawFastLine(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
                                      Graphics::Vec2D secondPoint, const Graphics::Color &lineColor,
                                      const Graphics::Color &backgroundColor, bool loopVerticalAxis)
{
    // This is an implementation of the Bresenham's line algorithm

    // Reorder the points
    if (secondPoint.y < firstPoint.y)
        std::swap(firstPoint, secondPoint);

    // Set the window
    Graphics::Vec2D size {static_cast<int16_t>(std::max(firstPoint.x, secondPoint.x) - std::min(firstPoint.x, secondPoint.x)),
                          static_cast<int16_t>(secondPoint.y - firstPoint.y)};
    Graphics::Vec2D position {std::min(firstPoint.x, secondPoint.x), firstPoint.y};
    target.setWindow(position, size);

    // Get some properties from the target
    uint32_t rawLineColor = target.convertColorToRaw(lineColor);
    uint32_t rawBackgroundColor = target.convertColorToRaw(backgroundColor);

    // Calculate how many parts we must send to the screen
    const uint16_t pixelCount = size.x * size.y;
    const uint8_t pixelSize = target.getPixelSize();
    const size_t totalBufferSize = size.x * size.y * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Keep track of the current position
    size_t actualPixel = 0;
    Graphics::Vec2D actualPosition = position;

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    // Prepare a large buffer of pixels for faster transmission
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);

    // Bresenham's algorithm parameters
    const int16_t dx = secondPoint.x - firstPoint.x;
    const int16_t dy = secondPoint.y - firstPoint.y;
    int16_t D = 2 * dy - dx;
    Graphics::Vec2D cursorPosition = firstPoint;
    size_t cursorPixelNbr = positionToPixelNbr(cursorPosition, position, size);

    target.prepareDrawBuffer();

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Clear the buffer
        fillBufferWithColor(target, buffer, BUFFER_SIZE, rawBackgroundColor);

        // Calculate the number of pixels to feed to the screen and the next cursor position
        const size_t pixelsToFeed = (actualStep == numberOfStep - 1) ? (size.x * size.y) % BUFFER_SIZE : BUFFER_SIZE / pixelSize;
        const size_t endPixel = (actualStep == numberOfStep - 1) ? pixelCount : (BUFFER_SIZE / pixelSize) * (actualStep + 1);

        while (cursorPixelNbr < endPixel &&
        ((cursorPosition.x <= secondPoint.x && firstPoint.x < secondPoint.x) || (cursorPosition.x >= secondPoint.x && firstPoint.x > secondPoint.x)))
        {
            target.putPixelInBuffer(buffer, rawLineColor, (cursorPixelNbr * pixelSize) % BUFFER_SIZE);

            if (D > 0)
            {
                ++cursorPosition.y;
                D = D - 2 * dx;
            }

            D = D + 2 * dy;
            if (firstPoint.x < secondPoint.x)
                ++cursorPosition.x;
            else if (firstPoint.x > secondPoint.x)
                --cursorPosition.x;

            cursorPixelNbr = positionToPixelNbr(cursorPosition, position, size);
        }

        // Draw the buffer
        const bool mustContinue = target.drawBuffer(position, size, actualPixel, actualPosition, buffer,
                                                    pixelsToFeed, verticalLoopCount, loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

void Graphics::GfxUtils::drawHorizontalLine(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
                                            uint16_t width, const Graphics::Color &color, bool loopVerticalAxis)
{
    drawFilledRectangle(target, position, {static_cast<int16_t>(width), 1}, color, loopVerticalAxis);
}

void Graphics::GfxUtils::drawVerticalLine(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
                                          uint16_t height, const Graphics::Color &color, bool loopVerticalAxis)
{
    drawFilledRectangle(target, position, {1, static_cast<int16_t>(height)}, color, loopVerticalAxis);
}

void Graphics::GfxUtils::drawFilledRectangle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
                                             const Graphics::Vec2D &size, const Graphics::Color &color, bool loopVerticalAxis)
{
    // Set the window
    target.setWindow(position, size);

    // Get some properties from the target
    uint32_t rawColor = target.convertColorToRaw(color);

    // Calculate how many parts we must send to the screen
    const uint8_t pixelSize = target.getPixelSize();
    const size_t totalBufferSize = size.x * size.y * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Keep track of the current position
    size_t actualPixel = 0;
    Graphics::Vec2D actualPosition = position;

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    // Prepare a large buffer of pixels for faster transmission
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);
    fillBufferWithColor(target, buffer, BUFFER_SIZE, rawColor);

    target.prepareDrawBuffer();

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Calculate the number of pixels to feed to the screen and the next cursor position
        const size_t pixelsToFeed = (actualStep == numberOfStep - 1) ? (size.x * size.y) % BUFFER_SIZE : BUFFER_SIZE / pixelSize;
        actualPixel += pixelsToFeed;

        // Draw the buffer
        const bool mustContinue = target.drawBuffer(position, size, actualPixel, actualPosition, buffer,
                pixelsToFeed,verticalLoopCount, loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

void Graphics::GfxUtils::drawFilledCircle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &origin,
                                          uint16_t radius, const Graphics::Color &circleColor,
                                          const Graphics::Color &backgroundColor, bool loopVerticalAxis)
{
    // This is an implementation of https://stackoverflow.com/a/1237519

    // Set the window
    Graphics::Vec2D size {static_cast<int16_t>(radius*2), static_cast<int16_t>(radius*2)};
    Graphics::Vec2D position {static_cast<int16_t>(origin.x - radius), static_cast<int16_t>(origin.y - radius)};
    target.setWindow(position, size);

    const uint32_t squareRadius = radius * radius;

    // Get some properties from the target
    uint32_t rawCircleColor = target.convertColorToRaw(circleColor);
    uint32_t rawBackgroundColor = target.convertColorToRaw(backgroundColor);

    // Calculate how many parts we must send to the screen
    const uint8_t pixelSize = target.getPixelSize();
    const size_t totalBufferSize = size.x * size.y * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Keep track of the current position
    size_t actualPixel = 0;
    Graphics::Vec2D actualPosition = position;
    Graphics::Vec2D radiusPosition {static_cast<int16_t>(-radius), static_cast<int16_t>(-radius)};

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    // Prepare a large buffer of pixels for faster transmission
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);

    target.prepareDrawBuffer();

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Calculate the number of pixels to feed to the screen and the next cursor position
        const size_t pixelsToFeed = (actualStep == numberOfStep - 1) ? (size.x * size.y) % BUFFER_SIZE : BUFFER_SIZE / pixelSize;
        size_t bufferPos = 0;

        for (size_t i = 0; i < pixelsToFeed; ++i)
        {
            if (static_cast<uint16_t>(radiusPosition.x * radiusPosition.x) + static_cast<uint16_t>(radiusPosition.y * radiusPosition.y) <= squareRadius)
                bufferPos = target.putPixelInBuffer(buffer, rawCircleColor, bufferPos);
            else
                bufferPos = target.putPixelInBuffer(buffer, rawBackgroundColor, bufferPos);

            if (++radiusPosition.x == radius)
            {
                radiusPosition.x = -radius;
                ++radiusPosition.y;
            }
        }

        // Draw the buffer
        const bool mustContinue = target.drawBuffer(position, size, actualPixel, actualPosition, buffer,
                                                    pixelsToFeed, verticalLoopCount, loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

uint16_t Graphics::GfxUtils::drawChar(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position, char c,
                                      const FONT_INFO &fontInfo, const Graphics::Color &textColor,
                                      const Graphics::Color &backgroundColor, bool loopVerticalAxis)
{
    // Check if the char is supported
    if (c < fontInfo.startChar || c > fontInfo.endChar)
        c = '?';

    // Set the window
    const size_t descriptorOffset = c - fontInfo.startChar;
    const FONT_CHAR_INFO descriptor = fontInfo.charInfo[descriptorOffset];
    Graphics::Vec2D glyphSize = {descriptor.widthBits, fontInfo.height};

    const Graphics::Vec2D framebufferSize = target.getFramebufferSize();
    const uint8_t pixelSize = target.getPixelSize();

    // Check if the character will be seen on the X axis
    if (position.x < -descriptor.widthBits || position.x > framebufferSize.x)
        return descriptor.widthBits;

    target.setWindow(position, glyphSize);

    // Get raw color
    uint32_t rawTextColor = target.convertColorToRaw(textColor);
    uint32_t rawBackgroundColor = target.convertColorToRaw(backgroundColor);

    // Prepare the Screen controller to receive data
    target.prepareDrawBuffer();

    // Allocate the buffer
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);

    // Check if the glyph will overflow on the X axis
    uint16_t overflowX = 0;
    if (position.x + descriptor.widthBits > framebufferSize.x)
    {
        overflowX = position.x + descriptor.widthBits - framebufferSize.x;
        glyphSize.x -= overflowX;
    }

    // Calculate the geometry of this glyph
    const uint16_t pixelCount = glyphSize.x * glyphSize.y;
    const uint8_t bytesPerLine = (descriptor.widthBits % 8 > 0) ? descriptor.widthBits / 8 + 1 : descriptor.widthBits / 8;

    // Calculate how many parts we must send to the screen
    const size_t totalBufferSize = pixelCount * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Those hold the current position of the cursor
    size_t actualPixel = 0;
    size_t actualPixelInGlyph = 0;
    Graphics::Vec2D actualPosition = position;

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Prepare the buffer to send data to the Screen
        const size_t endPixel = (actualStep == numberOfStep - 1) ? pixelCount : (BUFFER_SIZE / pixelSize) * (actualStep + 1);
        size_t bufferPos = 0;

        // Read data from the font definition
        while (actualPixel < endPixel)
        {
            const uint8_t bitInBlock = actualPixelInGlyph % descriptor.widthBits;
            if (bitInBlock < glyphSize.x)
            {
                const size_t actualByte = (bitInBlock / 8) + (bytesPerLine * (actualPixelInGlyph / descriptor.widthBits));
                const uint8_t actualVal = fontInfo.data[descriptor.offset + actualByte] & (1 << (bitInBlock % 8));

                if (actualVal)
                    bufferPos = target.putPixelInBuffer(buffer, rawTextColor, bufferPos);
                else
                    bufferPos = target.putPixelInBuffer(buffer, rawBackgroundColor, bufferPos);

                ++actualPixel;
                ++actualPixelInGlyph;
            }
            else
            {
                actualPixelInGlyph += overflowX;
            }
        }

        // Draw the buffer
        const bool mustContinue = target.drawBuffer(position, glyphSize, actualPixel, actualPosition, buffer,
                                             bufferPos / pixelSize, verticalLoopCount, loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);

    // Return the char width
    return descriptor.widthBits;
}

inline size_t Graphics::GfxUtils::positionToPixelNbr(const Graphics::Vec2D &position, const Graphics::Vec2D &basePosition,
                                              const Graphics::Vec2D &size)
{
    const Graphics::Vec2D normPosition = position - basePosition;
    return normPosition.y * size.x + normPosition.x;
}

inline void Graphics::GfxUtils::fillBufferWithColor(const Hardware::Screen::BaseScreen &target, uint8_t *buffer,
                                                const size_t size, const uint32_t rawColor)
{
    size_t i = 0;
    while ((i = target.putPixelInBuffer(buffer, rawColor, i)) < size) {}
}

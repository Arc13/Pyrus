#include "../../Fonts/Ubuntu24Font.h"
#include "../../Graphics/GfxUtils2.h"

#include "TestClipping.h"

#define APPLET_NAME "TestWrap"

Applet::TestClipping::TestClipping()
: BaseApplet(APPLET_NAME)
{

}

void Applet::TestClipping::processEvent(Event::BaseEvent *event)
{

}

void Applet::TestClipping::update(Platform::BasePlatform &platform)
{
    if (platform.getScreenManager().getVerticalScrollOffset() != 300)
        platform.getScreenManager().setVerticalScrollOffset(300);
}

void Applet::TestClipping::draw(Hardware::Screen::BaseScreen &target)
{
    Graphics::GfxUtils2 gfxUtils2(target);

    // Test every clipping axis
    gfxUtils2.setClippingEnabled(true);

    /*
     * Right clipping
     */
    gfxUtils2.setClippingStart({0, 0});
    gfxUtils2.setClippingEnd({18, 320});

    gfxUtils2.drawChar({8, 8}, 'A', ubuntu_24ptFontInfo);
    gfxUtils2.drawLine({8, 88}, {28, 119});
    gfxUtils2.drawLine({28, 88}, {8, 119});

    /*
     * Left clipping
     */
    gfxUtils2.setClippingStart({46, 0});
    gfxUtils2.setClippingEnd({56, 320});

    gfxUtils2.drawChar({36, 8}, 'A', ubuntu_24ptFontInfo);
    gfxUtils2.drawLine({36, 88}, {56, 119});
    gfxUtils2.drawLine({56, 88}, {36, 119});

    /*
     * Top clipping
     */
    gfxUtils2.setClippingStart({64, 8});
    gfxUtils2.setClippingEnd({84, 24});
    gfxUtils2.drawChar({64, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({64, 88});
    gfxUtils2.setClippingEnd({84, 104});
    gfxUtils2.drawLine({64, 88}, {84, 119});
    gfxUtils2.drawLine({84, 88}, {64, 119});

    /*
     * Bottom clipping
     */
    gfxUtils2.setClippingStart({92, 24});
    gfxUtils2.setClippingEnd({112, 40});
    gfxUtils2.drawChar({92, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({92, 104});
    gfxUtils2.setClippingEnd({112, 120});
    gfxUtils2.drawLine({92, 88}, {112, 119});
    gfxUtils2.drawLine({112, 88}, {92, 119});

    /*
     * All-axis clipping
     */
    gfxUtils2.setClippingStart({123, 13});
    gfxUtils2.setClippingEnd({131, 35});
    gfxUtils2.drawChar({120, 8}, '3', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({123, 93});
    gfxUtils2.setClippingEnd({131, 115});
    gfxUtils2.drawLine({120, 88}, {134, 119});
    gfxUtils2.drawLine({134, 88}, {120, 119});

    // Test clipping on vertical looping
    gfxUtils2.setLoopVerticalAxis(true);

    /*
     * Left clipping
     */
    gfxUtils2.setClippingStart({0, 320});
    gfxUtils2.setClippingEnd({18, 640});

    gfxUtils2.drawChar({8, 368}, 'A', ubuntu_24ptFontInfo);
    gfxUtils2.drawLine({8, 320+36+88}, {28, 320+36+119});
    gfxUtils2.drawLine({28, 320+36+88}, {8, 320+36+119});

    /*
     * Right clipping
     */
    gfxUtils2.setClippingStart({46, 320});
    gfxUtils2.setClippingEnd({56, 640});

    gfxUtils2.drawChar({36, 368}, 'A', ubuntu_24ptFontInfo);
    gfxUtils2.drawLine({36, 320+36+88}, {56, 320+36+119});
    gfxUtils2.drawLine({56, 320+36+88}, {36, 320+36+119});

    /*
     * Top clipping
     */
    gfxUtils2.setClippingStart({64, 368});
    gfxUtils2.setClippingEnd({84, 384});
    gfxUtils2.drawChar({64, 368}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({64, 320+36+88});
    gfxUtils2.setClippingEnd({84, 320+36+104});
    gfxUtils2.drawLine({64, 320+36+88}, {84, 320+36+119});
    gfxUtils2.drawLine({84, 320+36+88}, {64, 320+36+119});

    /*
     * Bottom clipping
     */
    gfxUtils2.setClippingStart({92, 384});
    gfxUtils2.setClippingEnd({112, 400});
    gfxUtils2.drawChar({92, 368}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({92, 320+36+104});
    gfxUtils2.setClippingEnd({112, 320+36+120});
    gfxUtils2.drawLine({92, 320+36+88}, {112, 320+36+119});
    gfxUtils2.drawLine({112, 320+36+88}, {92, 320+36+119});

    /*
     * All-axis clipping
     */
    gfxUtils2.setClippingStart({123, 373});
    gfxUtils2.setClippingEnd({131, 395});
    gfxUtils2.drawChar({120, 368}, '3', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({123, 320+36+93});
    gfxUtils2.setClippingEnd({131, 320+36+115});
    gfxUtils2.drawLine({120, 320+36+88}, {134, 320+36+119});
    gfxUtils2.drawLine({134, 320+36+88}, {120, 320+36+119});
}

bool Applet::TestClipping::allowsStatusBar() const
{
    return false;
}


#include <iostream>
#include <string>
#include <ctime>

#include <directfb/directfb.h>
#include "config.h"

/**
  * 演示Flip
  */

using namespace std;

namespace
{
    static IDirectFB *dfb;
    static IDirectFBSurface *screen;

    //event
    static IDirectFBEventBuffer   *keybuffer;

    static long myclock()
    {
        struct timeval tv;

        gettimeofday (&tv, NULL);

        return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
    }

}

bool init(int argc, char *argv[])
{
    DFBResult ret;
    DFBSurfaceDescription dsc;
   //初始化DirectFB
   ret = DirectFBInit(&argc, &argv);
   if (ret != DFB_OK) {
       cout<<"初始化DirectFB失败!"<<endl;
       return  false;
   }
   ret = DirectFBCreate(&dfb);
   if (ret != DFB_OK) {
       cout<<"DirectFBCreate失败!"<<endl;
       return  false;
   }

   //request full screen mode
   ret = dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN);
   if (ret != DFB_OK) {
       cout<<"SetCooperativeLevel失败!"<<endl;
       return  false;
   }

    dsc.flags = DSDESC_CAPS;
    dsc.caps = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_DOUBLE);
    ret = dfb->CreateSurface(dfb, &dsc, &screen);
    if (ret != DFB_OK) {
        cout<<"CreateSurface失败!"<<endl;
        return  false;
    }

    ret = dfb->CreateInputEventBuffer (dfb, DICAPS_KEYS,  DFB_FALSE, &keybuffer);
    if (ret != DFB_OK) {
        cout<<"IDirectFB::CreateEventBuffer() 失败!"<<endl;
        return  false;
    }

    return true;
}

void update()
{
    //更新屏幕
    screen->Flip (screen, NULL, DSFLIP_WAITFORSYNC);
    static int i = 0;
    if (i == 0) {
        screen->Clear(screen, 0, 0, 0, 0);
        i = 1;
    } else {
        screen->Clear(screen, 0xFF, 0xFF, 0xFF, 0xFF);
        i = 0;
    }
}

void clean_up()
{
    keybuffer->Release (keybuffer);
    screen->Release (screen);
    dfb->Release (dfb);
}

int main(int argc, char *argv[])
{
    bool quit = false;

    long t1, t2;
    unsigned int frame = 0, fps = 0;

    if (init(argc, argv) == false)
        return 1;

    screen->Clear(screen, 0, 0, 0, 0);
    screen->Flip(screen, NULL, (DFBSurfaceFlipFlags)0);
    screen->Clear(screen,  0xff, 0xff, 0xff, 0xff);

    t1 = t2 = myclock();

    while (quit == false) {
        DFBInputEvent           evt;

        while (keybuffer->GetEvent (keybuffer, DFB_EVENT(&evt)) == DFB_OK) {
            if (evt.type == DIET_KEYPRESS) {
                switch (evt.key_symbol) {
                case DIKS_ESCAPE:
                case DIKS_CAPITAL_Q:
                case DIKS_SMALL_Q:
                  quit = true;
                  break;
                default:
                  break;
                }
            }
        }
        update();
        frame++;
        t2 = myclock();
        if (t2 - t1 > 1000) {
           fps = frame * 1000 / (t2 - t1);

           t1 = t2;

           frame = 0;

           cout<<fps<<" fps"<<endl;
      }
    }

    clean_up();

   return 0;
}

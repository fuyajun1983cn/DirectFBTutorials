
#include <iostream>
#include <string>
#include <ctime>

#include <directfb/directfb.h>
#include "config.h"

/**
  * 演示Window
  */

using namespace std;

namespace
{
    static IDirectFB *dfb;
    static IDirectFBDisplayLayer *layer;


    static IDirectFBWindow *window1;
    static IDirectFBSurface *window1_surface;
    static IDirectFBWindow *window2;
    static IDirectFBSurface *window2_surface;

    static DFBDisplayLayerConfig  layer_config;
    static DFBGraphicsDeviceDescription  gdesc;
    //static IDirectFBWindow*              upper;
    static DFBWindowID                   id1;
    static DFBWindowID                   id2;

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
//    DFBSurfaceDescription dsc;
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

   dfb->GetDeviceDescription( dfb, &gdesc );
   dfb->GetDisplayLayer( dfb, DLID_PRIMARY, &layer );

   //request full screen mode
   ret = layer->SetCooperativeLevel (layer, DLSCL_ADMINISTRATIVE);
   if (ret != DFB_OK) {
       cout<<"SetCooperativeLevel失败!"<<endl;
       return  false;
   }

   if (!((gdesc.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL) &&
              (gdesc.blitting_flags & DSBLIT_BLEND_COLORALPHA  )))
    {
         layer_config.flags = DLCONF_BUFFERMODE;
         layer_config.buffermode = DLBM_BACKSYSTEM;

         layer->SetConfiguration( layer, &layer_config );
    }

   layer->SetBackgroundColor(layer, 0xFF, 0, 0, 0xFF);
   layer->SetBackgroundMode(layer, DLBM_COLOR);

    return true;
}

void init_windows()
{
    //DFBSurfaceDescription sdsc;
    DFBWindowDescription  desc;
    DFBResult ret;
    IDirectFBImageProvider *img_provider;

    //init window1
    desc.flags = (DFBWindowDescriptionFlags)( DWDESC_POSX | DWDESC_POSY |
                 DWDESC_WIDTH | DWDESC_HEIGHT );
    desc.posx = 20;
    desc.posy = 120;
    desc.width = 100;
    desc.height = 100;
    layer->CreateWindow(layer, &desc, &window1);
    window1->GetSurface(window1, &window1_surface);
    window1->SetOpacity(window1, 0xFF);
    window1->CreateEventBuffer(window1, &keybuffer);
    window1->GetID(window1, &id1);

    window1_surface->Clear(window1_surface, 0x0, 0x0, 0xFF, 0xFF);
    window1_surface->SetColor( window1_surface,
                                              0x0, 0xFF, 0x0, 0xFF );
    window1_surface->DrawRectangle( window1_surface,
                                   0, 0, desc.width, desc.height );

    //init window2
    desc.posx =  400;
    desc.posy =  200;
    desc.width = 72 * 2;
    desc.height = 72 * 2;
    layer->CreateWindow(layer, &desc, &window2);
    window2->GetSurface(window2, &window2_surface);
    window2_surface->Clear(window2_surface, 0xFF, 0xFF, 0xFF, 0xFF);
    //加载图片
    ret = dfb->CreateImageProvider (dfb, IMGDIR"/logo.png", &img_provider);
    if (ret != DFB_OK) {
         cout<<"加载图片失败!"<<endl;
        return;
    }
    img_provider->RenderTo(img_provider,  window2_surface, NULL);
    img_provider->Release(img_provider);
    window2->SetOpacity(window2, 0xFF);
    window2->AttachEventBuffer(window2, keybuffer);
    window2->GetID(window2, &id2);
}

void update()
{
    //更新屏幕
    window1_surface->Flip(window1_surface, NULL, (DFBSurfaceFlipFlags)0);
    window2_surface->Flip(window2_surface, NULL, (DFBSurfaceFlipFlags)0);
}

void clean_up()
{
    keybuffer->Release (keybuffer);
    window1_surface->Release(window1_surface);
    window1->Release(window1);
    layer->Release (layer);
    dfb->Release (dfb);
}

int main(int argc, char *argv[])
{
    bool quit = false;

    long t1, t2;
    unsigned int frame = 0, fps = 0;

    if (init(argc, argv) == false)
        return 1;

    init_windows();

    t1 = t2 = myclock();

    while (quit == false) {
        DFBWindowEvent           evt;
        static IDirectFBWindow *active = NULL;

        keybuffer->WaitForEventWithTimeout(keybuffer, 0, 10);
        cout<<"Test Event"<<endl;
        window1->RequestFocus(window1);
        while (keybuffer->GetEvent (keybuffer, DFB_EVENT(&evt)) == DFB_OK) {
            if (evt.type == DWET_KEYDOWN) {
                if (evt.window_id == id1)
                    active = window1;
                else
                    active = window2;
                cout <<"Get A Window Event"<<endl;
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


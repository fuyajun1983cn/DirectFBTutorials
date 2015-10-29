#include <iostream>
#include <string>
#include <directfb/directfb.h>

#include "config.h"

using namespace std;

namespace
{
    static IDirectFB *dfb;
    static IDirectFBSurface *screen;
    static IDirectFBImageProvider *img_provider;
    static IDirectFBSurface *message; //图片
    static IDirectFBSurface *background;

    static DFBSurfaceDescription dsc;

    const int SCREEN_WIDTH =  640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32;

    static IDirectFBSurface *load_image(string filename)
    {
        DFBResult ret;
        IDirectFBSurface *surf;
        ret = dfb->CreateImageProvider (dfb,  filename.c_str(), &img_provider);
        if (ret != DFB_OK) {
             cout<<"加载图片失败!"<<endl;
            return NULL;
        }

        img_provider->GetSurfaceDescription(img_provider, &dsc);
        dfb->CreateSurface(dfb, &dsc, &surf);
        img_provider->RenderTo(img_provider, surf, NULL);
        img_provider->Release(img_provider);

        return surf;
    }

    static void apply_surface(int x, int y, IDirectFBSurface *source, IDirectFBSurface *destination)
    {
        destination->Blit(destination, source, NULL, x, y);
    }

}

int main(int argc, char *argv[])
{
    DFBResult ret;
   //初始化DirectFB
   ret = DirectFBInit(&argc, &argv);
   if (ret != DFB_OK) {
       cout<<"初始化DirectFB失败!"<<endl;
       return -1;
   }
   DirectFBCreate(&dfb);

   dfb->SetCooperativeLevel (dfb, DFSCL_NORMAL);
   //设置屏幕模式
   dfb->SetVideoMode(dfb, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

    dsc.flags = DSDESC_CAPS;
    dsc.caps = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);
    dfb->CreateSurface(dfb, &dsc, &screen);

    //load images
    message = load_image(string(IMGDIR"/hello.png"));
    background = load_image(string(IMGDIR"/background.png"));

    apply_surface(0, 0, background, screen);
    apply_surface( 320, 0, background, screen );
    apply_surface( 0, 240, background, screen );
    apply_surface( 320, 240, background, screen );

    //Apply the message to the screen
    apply_surface( 180, 140, message, screen );

    //更新屏幕
    screen->Flip (screen, NULL, DSFLIP_WAITFORSYNC);

   //Pause
   sleep(2);

   message->Release (message);
   background->Release(background);
   screen->Release (screen);
   dfb->Release (dfb);

   return 0;
}

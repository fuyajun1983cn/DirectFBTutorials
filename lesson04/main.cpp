#include <iostream>
#include <string>

#include <directfb/directfb.h>
#include "config.h"

/**
  * 演示Color Key, 一般用于去掉背景色
  */

using namespace std;

namespace
{
    static IDirectFB *dfb;
    static IDirectFBSurface *screen;
    static IDirectFBImageProvider *img_provider;
    static IDirectFBSurface *background; //图片
    static IDirectFBSurface *foo;

    const int SCREEN_WIDTH =  640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32;

    //event
    static IDirectFBEventBuffer   *keybuffer;

    static IDirectFBSurface *load_image(string filename)
    {
        DFBResult ret;
        IDirectFBSurface *surf;
        DFBSurfaceDescription dsc;
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
        destination->SetBlittingFlags(destination, DSBLIT_SRC_COLORKEY );
        source->SetSrcColorKey(source, 0x0, 0xFF, 0xFF);
        destination->DisableAcceleration(destination, DFXL_BLIT);
        destination->Blit(destination, source, NULL, x, y);
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

   ret = dfb->SetCooperativeLevel (dfb, DFSCL_NORMAL);
   if (ret != DFB_OK) {
       cout<<"SetCooperativeLevel失败!"<<endl;
       return  false;
   }
   //设置屏幕模式
   ret = dfb->SetVideoMode(dfb, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
   if (ret != DFB_OK) {
       cout<<"SetVideoMode失败!"<<endl;
       return  false;
   }

    dsc.flags = DSDESC_CAPS;
    dsc.caps = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);
    ret = dfb->CreateSurface(dfb, &dsc, &screen);
    if (ret != DFB_OK) {
        cout<<"CreateSurface失败!"<<endl;
        return  false;
    }
    screen->Clear ( screen, 255, 255, 255, 255 );

    return true;
}

void update()
{
    //更新屏幕
    screen->Flip (screen, NULL, DSFLIP_WAITFORSYNC);
}

bool load_files()
{
    //load images
    background = load_image(string(IMGDIR"/background2.png"));
    if (background == NULL)
        return false;

    foo = load_image(string(IMGDIR"/foo.png"));
    if (foo == NULL)
        return false;

    return true;
}

void clean_up()
{
    keybuffer->Release (keybuffer);
    background->Release (background);
    foo->Release(foo);
    screen->Release (screen);
    dfb->Release (dfb);
}

int main(int argc, char *argv[])
{
    bool quit = false;

    if (init(argc, argv) == false)
        return 1;

    if (load_files() == false)
        return 1;

    apply_surface(0, 0, background, screen);
    apply_surface( 240, 190, foo, screen );

    update();

    dfb->CreateInputEventBuffer (dfb, DICAPS_KEYS,  DFB_FALSE, &keybuffer);

    while (quit == false) {
        DFBInputEvent           evt;

        while (keybuffer->GetEvent (keybuffer, DFB_EVENT(&evt)) == DFB_OK) {
            if (evt.type == DIET_KEYPRESS) {
                switch (evt.key_id) {
                case DIKI_ESCAPE:
                case DIKI_Q:
                  quit = true;
                  break;
                default:
                  break;
                }
            }
        }
  //     update();
    }

    clean_up();

   return 0;
}

#include <iostream>
#include <string>

#include <directfb/directfb.h>
#include "config.h"

/**
  * 演示图像裁剪, Sprites Sheet
  */

using namespace std;

namespace
{
    static IDirectFB *dfb;
    static IDirectFBSurface *screen;
    static IDirectFBImageProvider *img_provider;
    static IDirectFBSurface *dots; //图片

    const int SCREEN_WIDTH =  640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32;

    //event
    static IDirectFBEventBuffer   *keybuffer;
    static DFBRectangle clip[4];

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

    static void apply_surface(int x, int y, IDirectFBSurface *source, IDirectFBSurface *destination, DFBRectangle *clip = NULL)
    {
 //       destination->SetBlittingFlags(destination, DSBLIT_SRC_COLORKEY );
 //       destination->SetSrcColorKey(destination, 0, 0xff, 0xff);
        destination->Blit(destination, source, clip, x, y);
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
    dots = load_image(string(IMGDIR"/dots.png"));
    if (dots == NULL)
        return false;
    return true;
}

void clean_up()
{
    keybuffer->Release (keybuffer);
    dots->Release (dots);
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

    //Clip range for the top left
    clip[ 0 ].x = 0;
    clip[ 0 ].y = 0;
    clip[ 0 ].w = 100;
    clip[ 0 ].h = 100;

    //Clip range for the top right
    clip[ 1 ].x = 100;
    clip[ 1 ].y = 0;
    clip[ 1 ].w = 100;
    clip[ 1 ].h = 100;

    //Clip range for the bottom left
    clip[ 2 ].x = 0;
    clip[ 2 ].y = 100;
    clip[ 2 ].w = 100;
    clip[ 2 ].h = 100;

    //Clip range for the bottom right
    clip[ 3 ].x = 100;
    clip[ 3 ].y = 100;
    clip[ 3 ].w = 100;
    clip[ 3 ].h = 100;

    screen->SetColor(screen, 0xFF, 0xFF, 0xFF, 0xFF);
    screen->FillRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    //Apply the sprites to the screen
    apply_surface( 0, 0, dots, screen, &clip[ 0 ] );
    apply_surface( 540, 0, dots, screen, &clip[ 1 ] );
    apply_surface( 0, 380, dots, screen, &clip[ 2 ] );
    apply_surface( 540, 380, dots, screen, &clip[ 3 ] );

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
    }

    clean_up();

   return 0;
}

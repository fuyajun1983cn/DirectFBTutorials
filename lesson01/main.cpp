#include <iostream>
#include <directfb/directfb.h>

#include "config.h"

using namespace std;
//load an image and display on the screen
namespace
{
    static IDirectFB *dfb;
    static IDirectFBSurface *screen;
    static IDirectFBImageProvider *img_provider;
    static IDirectFBSurface *hello; //图片

    static DFBSurfaceDescription dsc;

    const int SCREEN_WIDTH =  640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32;
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
    //加载图片
    ret = dfb->CreateImageProvider (dfb, IMGDIR"/helloworld.png", &img_provider);
    if (ret != DFB_OK) {
         cout<<"加载图片失败!"<<endl;
        return -1;
    }
    img_provider->GetSurfaceDescription(img_provider, &dsc);
    dfb->CreateSurface(dfb, &dsc, &hello);
    img_provider->RenderTo(img_provider, hello, NULL);
    img_provider->Release(img_provider);

   //将图片应用到屏幕上
    screen->Blit(screen, hello, NULL, 0, 0);
    //更新屏幕
    screen->Flip (screen, NULL, DSFLIP_WAITFORSYNC);

   //Pause
   sleep(2);

   hello->Release (hello);
   screen->Release (screen);
   dfb->Release (dfb);

   return 0;
}


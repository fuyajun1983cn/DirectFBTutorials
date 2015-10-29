
#include <iostream>
#include <string>
#include <ctime>

#include <directfb/directfb.h>
#include "config.h"

/**
  * 演示Multi-language
  */

using namespace std;

namespace
{
    static IDirectFB *dfb;
    static IDirectFBSurface *screen;

    //event
    static IDirectFBEventBuffer   *keybuffer;

    const int SCREEN_WIDTH =  640;
    const int SCREEN_HEIGHT = 480;
    const int SCREEN_BPP = 32;

    typedef enum
    {
        LANG_ENGLISH = 0,
        LANG_CHINESE ,
        LANG_JAPANESE,
        LANG_KOREA,
        LANG_FRENCH,
        LANG_GERMANY,
        LANG_VIETNAM,
        LANG_COUNT
    }LANG;

//    unsigned char english[] = {0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x54, 0x65, 0x73, 0x74, 0x21, 0};
    unsigned char chs[] = { 0xa9, 0x96, 0x21,  0};

    static unsigned char *msg[LANG_COUNT][1] =
    {
        {"This is a Test!"},
        {chs},
        {"これはテストです！"},
        {"이것은 테스트입니다!"},
        {"Ceci est un test!"},
        {"Dies ist ein Test!"},
        {"Đây là một thử nghiệm!"}
    };

    LANG selected_lang = LANG_ENGLISH;

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

   dfb->SetCooperativeLevel (dfb, DFSCL_NORMAL);
   //设置屏幕模式
   dfb->SetVideoMode(dfb, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

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

static DFBEnumerationResult TextEncodingCallback (
    DFBTextEncodingID 	 		encoding_id ,
    const char 	 	* 	name ,
    void 	 	* 	context
)
{
    cout<<"Name: "<<name<<" ecoding_id: "<<encoding_id<<endl;
}

void font_test(LANG lang)
{
    DFBFontDescription fontdesc;
    IDirectFBFont *font;
    DFBResult ret;

    fontdesc.flags = (DFBFontDescriptionFlags)(DFDESC_HEIGHT | DFDESC_HEIGHT);
    fontdesc.height = 70;
    fontdesc.width = 70;

    //ret = dfb->CreateFont(dfb, NULL, &fontdesc, &font);
    ret = dfb->CreateFont(dfb, FONTDIR"/FZCCHK.TTF", &fontdesc, &font);
    if (ret != DFB_OK) {
        cout<<"failed to create  font"<<endl;
        return;
    }
    font->EnumEncodings(font,  TextEncodingCallback, NULL);

    screen->SetFont(screen, font);

    screen->SetColor(screen, 0xFF, 0, 0, 0xFF);
    screen->DrawString(screen, msg[lang][0], -1, 20, 240, DSTF_LEFT);
    cout<<"msg["<<lang<<"][0]: "<<msg[lang][0]<<endl;

    font->Release(font);
}

void update()
{
    //更新屏幕
    screen->Clear(screen, 0xFF, 0xFF, 0xFF, 0xFF);
    font_test(selected_lang);
    screen->Flip (screen, NULL, DSFLIP_WAITFORSYNC);
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

    if (init(argc, argv) == false)
        return 1;

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
                case DIKS_ENTER:
                {
                    switch (selected_lang)
                    {
                    case LANG_ENGLISH:
                        selected_lang = LANG_CHINESE;
                        break;
                     case LANG_CHINESE:
                        selected_lang = LANG_JAPANESE;
                        break;
                      case LANG_JAPANESE:
                        selected_lang = LANG_KOREA;
                        break;
                      case LANG_KOREA:
                         selected_lang = LANG_FRENCH;
                         break;
                      case LANG_FRENCH:
                         selected_lang = LANG_GERMANY;
                         break;
                      case LANG_GERMANY:
                         selected_lang = LANG_VIETNAM;
                         break;
                      case LANG_VIETNAM:
                         selected_lang = LANG_ENGLISH;
                         break;
                    }
                    update();
                }
                    break;
                  default:
                  break;
                }
            }
        }
        update();

    }

    clean_up();

   return 0;
}

#include <stdio.h>
#include "pd_api.h"
#include "display.h"

static int update(void* userdata);
static void initialize(void);

const char* fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont* font = NULL;

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
    (void)arg; // unused

    if ( event == kEventInit )
    {
         font = pd->graphics->loadFont(fontpath, NULL);

        if (font == NULL)
        {
            pd->system->error("Could not load font %s", fontpath);
        }

        initialize();
        pd->system->setUpdateCallback(update, pd);
    }

    return 0;
}

static void initialize(void)
{
    // Initialization code goes here
}

static int update(void* userdata)
{
    PlaydateAPI* pd = (PlaydateAPI*)userdata;

    pd->graphics->clear(kColorWhite);
    pd->graphics->setFont(font);

    pd->graphics->drawText("Hello World!", strlen("Hello World!"), kASCIIEncoding, 10, 100);

    return 1;
}

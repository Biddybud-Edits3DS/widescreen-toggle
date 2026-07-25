#include <3ds.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    printf("\x1b[2;2HChecking Widescreen Patch...\n");

    const char* enabledPath = "sdmc:/luma/sysmodules/TwlBg.cxi";
    const char* disabledPath = "sdmc:/luma/sysmodules/TwlBg.cxi.off";
    bool swapSuccessful = false;

    if(access(enabledPath, F_OK) == 0) {
        if(rename(enabledPath, disabledPath) == 0) {
            printf("\n\x1b[32m  [DISABLED] Widescreen Patch\x1b[0m\n");
            printf("  TwlBg.cxi -> TwlBg.cxi.off\n");
            printf("\n  DSpico will now boot in 4:3 (Fixed)!");
            swapSuccessful = true;
        } else {
            printf("\n\x1b[31m  [ERROR] Could not deactivate patch!\x1b[0m\n");
        }
    } 
    else if(access(disabledPath, F_OK) == 0) {
        if(rename(disabledPath, enabledPath) == 0) {
            printf("\n\x1b[32m  [ENABLED] Widescreen Patch\x1b[0m\n");
            printf("  TwlBg.cxi.off -> TwlBg.cxi\n");
            printf("\n  Warning: DSpico will use stretched scaling.");
            swapSuccessful = true;
        } else {
            printf("\n\x1b[31m  [ERROR] Could not activate patch!\x1b[0m\n");
        }
    } 
    else {
        printf("\n\x1b[31m  [ERROR] TwlBg.cxi not found!\x1b[0m\n");
        printf("  Please make sure your widescreen patch file\n");
        printf("  is placed inside: luma/sysmodules/\n");
    }

    if(swapSuccessful) {
        printf("\n\n\n  Rebooting console in 3 seconds...");
        gfxFlushBuffers();
        gfxSwapBuffers();
        svcSleepThread(3000000000LL);
        APT_HardwareResetAsync();
    } else {
        printf("\n\n\n  Press START to exit back to Homebrew Launcher.");
        while (aptMainLoop()) {
            gspWaitForVBlank();
            hidScanInput();
            u32 kDown = hidKeysDown();
            if (kDown & KEY_START) break;
        }
    }

    gfxExit();
    return 0;
}

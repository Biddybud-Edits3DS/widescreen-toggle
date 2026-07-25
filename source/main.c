#include <3ds.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    // Initialize standard 3DS console graphics
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    printf("\x1b[2;2HChecking Widescreen Patch...\n");

    // Define the SD file paths
    const char* enabledPath = "sdmc:/luma/sysmodules/TwlBg.cxi";
    const char* disabledPath = "sdmc:/luma/sysmodules/TwlBg.cxi.off";

    bool swapSuccessful = false;

    // Check if the widescreen file is currently active
    if(access(enabledPath, F_OK) == 0) {
        // Widescreen is enabled: Turn it OFF
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
        // Widescreen is disabled: Turn it ON
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
        // Neither file was found in the folder
        printf("\n\x1b[31m  [ERROR] TwlBg.cxi not found!\x1b[0m\n");
        printf("  Please make sure your widescreen patch file\n");
        printf("  is placed inside: luma/sysmodules/\n");
    }

    // If the swap worked, announce and execute an automated console reboot
    if(swapSuccessful) {
        printf("\n\n\n  Rebooting console in 3 seconds...");
        gfxFlushBuffers();
        gfxSwapBuffers();
        
        // Wait 3 seconds (3,000,000,000 nanoseconds)
        svcSleepThread(3000000000LL);
        
        // Trigger an asynchronous console hardware reset
        APT_HardwareResetAsync();
    } else {
        printf("\n\n\n  Press START to exit back to Homebrew Launcher.");
        // Main loop if no action was taken
        while (aptMainLoop())
        {
            gspWaitForVBlank();
            hidScanInput();

            u32 kDown = hidKeysDown();
            if (kDown & KEY_START) break;
        }
    }

    gfxExit();
    return 0;
}

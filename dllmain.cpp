#include <Windows.h>
#include "mem.h"
#include "hook.h"
#include "hack.h"
#include "menu.h"

typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);

twglSwapBuffers wglSwapBuffersGateway;

Menu menu;

BOOL __stdcall hkwglSwapBuffers(HDC hDc) {
    
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        hack::bAmmo = !hack::bAmmo;
    if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        hack::bHealth = !hack::bHealth;
    if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        hack::bRecoil = !hack::bRecoil;
    if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        hack::bESP = !hack::bESP;
    if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        hack::bAimbot = !hack::bAimbot;
    if (GetAsyncKeyState(VK_INSERT) & 1)
        hack::bMenu = !hack::bMenu;
  
    if (hack::bAmmo)
        hack::doAmmo();

    if (hack::bHealth)
        hack::doHealth();
    
    if (hack::bRecoil)
        hack::doRecoil();
    else if (!hack::bRecoil)
        hack::fixRecoil();

    if(hack::bAimbot)
        hack::doAimbot();

    if (hack::bESP)
        hack::doESP();

    menu.RenderMenu();

    return wglSwapBuffersGateway(hDc);
}

BOOL WINAPI HackThread(HMODULE hModule) {

#ifdef _DEBUG
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
#endif

    hack::init();
    menu.Init();
    menu.SetHook();

    
    Hook swapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBuffers, (BYTE*)&wglSwapBuffersGateway, 5);

    swapBuffersHook.Enable();

    // Listen for exit key
    while (true) {
        if (GetAsyncKeyState(VK_DELETE)) {
            break;
        }
    }

    // Disable our hook
    swapBuffersHook.Disable();
    menu.Shutdown();
    Sleep(100);

    #ifdef _DEBUG
        FreeConsole();
        fclose(f);
    #endif

    FreeLibraryAndExitThread(hModule, 0);
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackThread, hModule, NULL, NULL));
    }
    return TRUE;
}
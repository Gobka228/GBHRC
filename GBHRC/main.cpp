#include <cstdio>

#include "includes/win.h"
#include "CheatApi/Hooks/d3d11/d3d11hook.h"
#include "CheatApi/Hooks/wndproc/wndprochook.h"
#include "Form/Form.h"

#include "Forms/Menu/MenuMain.h"

#include "CheatApi/BrokeProtocol/Mono/Mono.h"
#include "CheatApi/BrokeProtocol/BrokeProtocol.h"

#include <string>
#include <sstream>

HINSTANCE DllInst;
Application::Form* menu;
void wnd_key_hook(UINT msg, WPARAM wParam, LPARAM lParam);

void init_callback(Application::Render::Engine* instance)
{
    menu = new Application::Form();

    MainMenuMarkup(menu, instance);

    instance
        ->append_scene(menu);

    Application::register_form(menu);
}

void MainThread()
{
    HWND hwnd = Hooks::D3D11::FindMainWindow(GetCurrentProcessId());
	
    Application::set_main_hwnd(hwnd);
#ifdef _DEBUG
    AllocConsole();
    auto nigger = freopen("CONOUT$", "w", stdout);
    nigger = freopen("CONIN$", "r", stdin);
    system("clear");
#endif
	
    Hooks::WndProc::init_hook(hwnd);
    Hooks::WndProc::callback(wnd_key_hook);

    // Hooks::D3D11::hook(Hooks::D3D11::GetPresentAddress(), init_callback);
    // WndProcHook::setInputState(false);
}

void wnd_key_hook(UINT msg, WPARAM wParam, LPARAM lParam)
{
    Application::wnd_proc(msg, wParam, lParam);

    if (msg == WM_KEYUP)
    {
        if (wParam == VK_DELETE)
        {

        }

        if (wParam == VK_INSERT)
        {
            static bool interactive_state = true;

            menu->hidden = interactive_state;
            Hooks::WndProc::setInputState(interactive_state);

            interactive_state = !interactive_state;
        }

        if (wParam == VK_F5)
        {
            BrokeProtocol::jump();
        }

        if(wParam == VK_F4)
        {
            auto* image = Mono::mono_image_loaded("D:\\Steam\\steamapps\\common\\BROKE PROTOCOL\\BrokeProtocol_Data\\Managed\\Scripts.dll");
            auto* method_desc = Mono::mono_method_desc_new("BrokeProtocol.Entities.ShPlayer:ClearInjuries()", true);
            auto* pClass = Mono::mono_class_from_name(image, method_desc->namespace_name, method_desc->class_name);
            DEBUG_LOG("CLASS: " << pClass);
            auto* method = Mono::mono_class_get_method_from_name(pClass, method_desc->method_name, -1);
            DEBUG_LOG("METHOD: " << method);
            Mono::mono_method_desc_free(method_desc);

            Mono::mono_thread_attach(Mono::mono_get_root_domain());
            Mono::mono_runtime_invoke(method, BrokeProtocol::GetLocalPlayer(), nullptr, nullptr);

            DEBUG_LOG("============INVOKED============");
        }

    	if(wParam == VK_F2){

            auto* pdict = BrokeProtocol::GetPlayersCollection();
            auto* array = Mono::mono_array_new(
                Mono::mono_get_root_domain(),
                Mono::mono_class_from_name(Mono::get_script_image(), "BrokeProtocol.Entities", "ShPlayer"),
                pdict->count
            );
    		// ���� ���
            DEBUG_LOG("DICK: "<<pdict);
            //pdict->copy_to(array, 0);
            DEBUG_LOG(pdict->contains(BrokeProtocol::GetLocalPlayer()));
            //DEBUG_LOG("COPIED");
            //DEBUG_LOG(((BrokeProtocol::Players::ShPlayer*)array->vector[0])->health);
    	}

        if (wParam == VK_F3)
        {
            BrokeProtocol::fire();
            //BrokeProtocol::send_global_chat((char*)"FEFE FUCKING SLAVE MAZAFAKA");
        }
    }

}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DllInst = hinstDLL;
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, nullptr, 0, nullptr);
        break;

    case DLL_PROCESS_DETACH:
        FreeLibraryAndExitThread(hinstDLL, 0);
    }
    return TRUE;
}

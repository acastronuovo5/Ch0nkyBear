#include <windows.h>
#include <mmsystem.h>
#include "resource.h"
#include <stdio.h>
#include <iostream>
#include <thread>

HMODULE DllHandle = NULL;
// lazy Extract and play. Sue me. 
 bool ExtractAndPlayResource(int ResourceID, bool block)
    {
        if (!DllHandle){
  
            return false;
        }
        try
        {
            HRSRC hResource = FindResource(DllHandle, MAKEINTRESOURCE(ResourceID), RT_RCDATA);
            if (hResource == nullptr)
            {
                     std::cout << "Cant fnd "<< ResourceID  << std::endl;
                return false;
            }

            HGLOBAL hFileResource = LoadResource(DllHandle, hResource);
            if (hFileResource == nullptr)
            {
                     std::cout << "cant load" << std::endl;
                return false;
            }

            void* lpFile = LockResource(hFileResource);
            if (lpFile == nullptr)
            {
                     std::cout << "Cant lock" << std::endl;
                return false;
            }
                 DWORD flags = SND_MEMORY;
                 if (!block){
                     flags = flags |SND_ASYNC;}

               auto bRtn = PlaySoundA((LPCSTR)lpFile, NULL, flags);//SND_ASYNC will make it work so we don't lock the process
               std::cout << bRtn << std::endl;
            std::uint32_t dwSize = SizeofResource(DllHandle, hResource);
            if (dwSize == 0)
            {
                  std::cout << "zero size" << std::endl;
                return false;
            } else{
                std::cout << "Size: "<< dwSize << std::endl;
            }
            

            return true;
        }
        catch (...) {}
        return false;
    }

void Patriotic(){
    
    
     auto x = ExtractAndPlayResource(IDR_SURPRISE, true);
    
    auto y = ExtractAndPlayResource(IDR_SOVIET, false);
    
    printf("%d,%d\n", x,y);
    if(x && y){
    MessageBoxW(NULL, L"Hello", L"FromMyFunction", MB_OK);
    }
}


// C++ will mangle names in exported functions. This is why we use edtern "C"
extern "C" __declspec(dllexport) int Surprise()
{
    Patriotic();
  return 0;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{

    // Fun fact, doing anything crazy in here will BREAK EVERYTHING YOU KNOW AND LOVE

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    DllHandle = hinstDLL;
        ::OutputDebugStringW(L"DLL_PROCESS_ATTACH");
        break;

    case DLL_THREAD_ATTACH:
        ::OutputDebugStringW(L"DLL_THREAD_ATTACH");
        break;

    case DLL_THREAD_DETACH:
        ::OutputDebugStringW(L"DLL_THREAD_DETACH");
        break;

    case DLL_PROCESS_DETACH:
        ::OutputDebugStringW(L"DLL_PROCESS_DETACH");
        break;
    }

    return TRUE;
}
#include <windows.h>
#include <string>


bool flag = true;
const wchar_t* TARGET_MODULE_NAME = L"qagamex86.dll";

typedef void (*TargetPM_Accelerate)(int, float, float);
TargetPM_Accelerate oFuncPM_Accelerate;
BYTE origPM_Accelerate[5];
uint8_t* PM_Accelerate = reinterpret_cast<uint8_t*>(0x20000000 + 0x035050);

BYTE origG_Damage[5];
uint8_t* G_Damage = reinterpret_cast<uint8_t*>(0x20000000 + 0x043A50);

float* lengthKnife = reinterpret_cast<float*>(0x20000000 + 0x07B184);

CRITICAL_SECTION cs;

void SetHook(uint8_t* targetFunctionAddress, void* hookFunction, void* originalBytes, size_t sizeOfOriginalBytes);
void RemoveHook(uint8_t* targetFunctionAddress, void* originalBytes, size_t sizeOfOriginalBytes);



LONG CALLBACK VectoredExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION)
    {
        DWORD exceptionAddress = (DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress;
        DWORD oldProtect;
        // Removing PAGE_GUARD protection from the address where the exception occurred.
        VirtualProtect((void*)exceptionAddress, 1, PAGE_EXECUTE_READWRITE, &oldProtect);

        if (flag)
        {
           // The exception occurred at the start of the function.
            PCONTEXT context = ExceptionInfo->ContextRecord;
            DWORD* pStack = (DWORD*)context->Esp;
            if (pStack[5] == 27031128 || pStack[5] == 27031056)
            {
                pStack[6] = 256;
            }
            else
            {
                pStack[1] = 542734600;
                pStack[5] = 0;
            }
            // Removing PAGE_GUARD protection
            VirtualProtect(G_Damage, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
            // Setting PAGE_GUARD protection on ret
            VirtualProtect(reinterpret_cast<LPVOID>(pStack[0]), 1, PAGE_EXECUTE_READ | PAGE_GUARD, &oldProtect);
            flag = false;
        }
        else 
        {
            // The exception occurred at one of the exit points
            // Setting PAGE_GUARD protection at the start of the function
            VirtualProtect(G_Damage, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &oldProtect);
            flag = true;
        }

        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}


void __cdecl HookedPM_Accelerate(int param1, float param2, float param3)
{
    EnterCriticalSection(&cs);
    RemoveHook(PM_Accelerate, origPM_Accelerate, 5);

    if (param1 == 0x19c7684)
    {
        if (param2 == 352)
        {
            param2 = 1500.0;
        }
        else
            param2 = 500.0;
    }
    oFuncPM_Accelerate(param1, param2, param3);
    SetHook(PM_Accelerate, HookedPM_Accelerate, origPM_Accelerate, 5);
    LeaveCriticalSection(&cs);
}


void SetHook(uint8_t* targetFunctionAddress, void* hookFunction, void* originalBytes, size_t sizeOfOriginalBytes)
{
    DWORD oldProtect;
    VirtualProtect(targetFunctionAddress, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    targetFunctionAddress[0] = 0xE9; // JMP
    *(DWORD*)(targetFunctionAddress + 1) = (uint8_t*)hookFunction - targetFunctionAddress - 5;
    VirtualProtect(targetFunctionAddress, 5, oldProtect, &oldProtect);
}


void RemoveHook(uint8_t* targetFunctionAddress, void* originalBytes, size_t sizeOfOriginalBytes)
{
    DWORD oldProtect;
    VirtualProtect(targetFunctionAddress, sizeOfOriginalBytes, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(targetFunctionAddress, originalBytes, sizeOfOriginalBytes);
    VirtualProtect(targetFunctionAddress, sizeOfOriginalBytes, oldProtect, &oldProtect);
}


void ChangeKnifeLength()
{
   
   DWORD oldProtect;
    // Change access rights
    if (VirtualProtect(lengthKnife, sizeof(float), PAGE_READWRITE, &oldProtect))
    {
        // Modify the value
        *lengthKnife = 3000.0;  // New value
        // Restore original access rights
        VirtualProtect(lengthKnife, sizeof(float), oldProtect, &oldProtect);

    }
}


void InitHook()
{
    // Saving the original bytes
    memcpy(origPM_Accelerate, PM_Accelerate, 5);
    oFuncPM_Accelerate = (TargetPM_Accelerate)PM_Accelerate;
    InitializeCriticalSection(&cs);
    SetHook(PM_Accelerate, HookedPM_Accelerate, origPM_Accelerate, 5);
    //SetHook(G_Damage, HookedG_Damage, origG_Damage, 5);
    AddVectoredExceptionHandler(1, VectoredExceptionHandler);
    DWORD oldProtect;
    VirtualProtect(G_Damage, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &oldProtect);
    ChangeKnifeLength();

}


DWORD WINAPI ThreadProc(LPVOID lpParameter) {
    bool wasModuleLoaded = false;
    HMODULE g_targetModule = nullptr;
    while (true) {
        g_targetModule = GetModuleHandle(TARGET_MODULE_NAME);

        if (g_targetModule && !wasModuleLoaded) {
            wasModuleLoaded = true;
            OutputDebugStringA("Loaded");
            Sleep(2000);
            InitHook();
            OutputDebugStringA("Hooks setted");
        }
        else if (!g_targetModule && wasModuleLoaded) {
            wasModuleLoaded = false;
            OutputDebugStringA("Unloaded");
        }

        Sleep(500);  // Checking every half a second.
    }

    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        OutputDebugStringA("Debug message from DLL\n");
        CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    }
    
    return TRUE;
}

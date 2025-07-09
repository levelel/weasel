#include <windows.h>
#include <string>

// Function pointer types for the original Dll functions
typedef HRESULT (WINAPI *PFNDllCanUnloadNow)();
typedef HRESULT (WINAPI *PFNDllGetClassObject)(REFCLSID rclsid, REFIID riid, void** ppvObject);
typedef HRESULT (WINAPI *PFNDllRegisterServer)();
typedef HRESULT (WINAPI *PFNDllUnregisterServer)();

// Function pointer types for the original IME functions
typedef BOOL (WINAPI *PFNImeInquire)(IMEINFO* lpIMEInfo, LPWSTR lpszUIClass, DWORD dwSystemInfoFlags);
typedef BOOL (WINAPI *PFNImeConfigure)(HKL hKL, HWND hWnd, DWORD dwMode, LPVOID lpData);
typedef DWORD (WINAPI *PFNImeConversionList)(HIMC hIMC, LPCTSTR lpSource, LPCANDIDATELIST lpCandList, DWORD dwBufLen, UINT uFlag);
typedef BOOL (WINAPI *PFNImeDestroy)(UINT uForce);
typedef LRESULT (WINAPI *PFNImeEscape)(HIMC hIMC, UINT uSubFunc, LPVOID lpData);
typedef BOOL (WINAPI *PFNImeProcessKey)(HIMC hIMC, UINT vKey, LPARAM lKeyData, const LPBYTE lpbKeyState);
typedef BOOL (WINAPI *PFNImeSelect)(HIMC hIMC, BOOL fSelect);
typedef BOOL (WINAPI *PFNImeSetActiveContext)(HIMC hIMC, BOOL fFocus);
typedef UINT (WINAPI *PFNImeToAsciiEx)(UINT uVKey, UINT uScanCode, CONST LPBYTE lpbKeyState, LPDWORD lpdwTransKey, UINT fuState, HIMC hIMC);
typedef BOOL (WINAPI *PFNNotifyIME)(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue);
typedef BOOL (WINAPI *PFNImeRegisterWord)(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr);
typedef BOOL (WINAPI *PFNImeUnregisterWord)(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr);
typedef UINT (WINAPI *PFNImeGetRegisterWordStyle)(UINT nItem, LPSTYLEBUF lp);
typedef UINT (WINAPI *PFNImeEnumRegisterWord)(REGISTERWORDENUMPROC lpfn, LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr, LPVOID lpData);
typedef BOOL (WINAPI *PFNImeSetCompositionString)(HIMC hIMC, DWORD dwIndex, LPCVOID lpComp, DWORD dwComp, LPCVOID lpRead, DWORD dwRead);


// Global handles for the loaded DLLs
HMODULE g_hWeaselTSFX64 = NULL;
HMODULE g_hWeaselTSFARM64 = NULL;
HMODULE g_hWeaselIMEX64 = NULL;
HMODULE g_hWeaselIMEARM64 = NULL;

// Helper function to load the correct TSF DLL
HMODULE LoadWeaselTSFDll() {
    SYSTEM_INFO sysInfo;
    GetNativeSystemInfo(&sysInfo);

    if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64) {
        if (g_hWeaselTSFARM64 == NULL) {
            g_hWeaselTSFARM64 = LoadLibrary(L"weaselARM64.dll"); // Assuming weaselARM64.dll is the ARM64 version
        }
        return g_hWeaselTSFARM64;
    } else { // Assuming x64
        if (g_hWeaselTSFX64 == NULL) {
            g_hWeaselTSFX64 = LoadLibrary(L"weaselx64.dll"); // Assuming weaselx64.dll is the x64 version
        }
        return g_hWeaselTSFX64;
    }
}

// Helper function to load the correct IME DLL
HMODULE LoadWeaselIMEDll() {
    SYSTEM_INFO sysInfo;
    GetNativeSystemInfo(&sysInfo);

    if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64) {
        if (g_hWeaselIMEARM64 == NULL) {
            g_hWeaselIMEARM64 = LoadLibrary(L"WeaselIME_ARM64.ime");
        }
        return g_hWeaselIMEARM64;
    } else { // Assuming x64
        if (g_hWeaselIMEX64 == NULL) {
            g_hWeaselIMEX64 = LoadLibrary(L"WeaselIME_x64.ime");
        }
        return g_hWeaselIMEX64;
    }
}


// Forwarding function for DllCanUnloadNow
HRESULT WINAPI DllCanUnloadNow() {
    HMODULE hDll = LoadWeaselTSFDll();
    if (hDll != NULL) {
        PFNDllCanUnloadNow pfn = (PFNDllCanUnloadNow)GetProcAddress(hDll, "DllCanUnloadNow");
        if (pfn != NULL) {
            return pfn();
        }
    }
    return E_FAIL;
}

// Forwarding function for DllGetClassObject
HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppvObject) {
    HMODULE hDll = LoadWeaselTSFDll();
    if (hDll != NULL) {
        PFNDllGetClassObject pfn = (PFNDllGetClassObject)GetProcAddress(hDll, "DllGetClassObject");
        if (pfn != NULL) {
            return pfn(rclsid, riid, ppvObject);
        }
    }
    return E_FAIL;
}

// Forwarding function for DllRegisterServer
HRESULT WINAPI DllRegisterServer() {
    HMODULE hDll = LoadWeaselTSFDll();
    if (hDll != NULL) {
        PFNDllRegisterServer pfn = (PFNDllRegisterServer)GetProcAddress(hDll, "DllRegisterServer");
        if (pfn != NULL) {
            return pfn();
        }
    }
    return E_FAIL;
}

// Forwarding function for DllUnregisterServer
HRESULT WINAPI DllUnregisterServer() {
    HMODULE hDll = LoadWeaselTSFDll();
    if (hDll != NULL) {
        PFNDllUnregisterServer pfn = (PFNDllUnregisterServer)GetProcAddress(hDll, "DllUnregisterServer");
        if (pfn != NULL) {
            return pfn();
        }
    }
    return E_FAIL;
}


// Forwarding function for ImeInquire
BOOL WINAPI ImeInquire(IMEINFO* lpIMEInfo, LPWSTR lpszUIClass, DWORD dwSystemInfoFlags) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeInquire pfn = (PFNImeInquire)GetProcAddress(hDll, "ImeInquire");
        if (pfn != NULL) {
            return pfn(lpIMEInfo, lpszUIClass, dwSystemInfoFlags);
        }
    }
    return FALSE;
}

// Forwarding function for ImeConfigure
BOOL WINAPI ImeConfigure(HKL hKL, HWND hWnd, DWORD dwMode, LPVOID lpData) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeConfigure pfn = (PFNImeConfigure)GetProcAddress(hDll, "ImeConfigure");
        if (pfn != NULL) {
            return pfn(hKL, hWnd, dwMode, lpData);
        }
    }
    return FALSE;
}

// Forwarding function for ImeConversionList
DWORD WINAPI ImeConversionList(HIMC hIMC, LPCTSTR lpSource, LPCANDIDATELIST lpCandList, DWORD dwBufLen, UINT uFlag) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeConversionList pfn = (PFNImeConversionList)GetProcAddress(hDll, "ImeConversionList");
        if (pfn != NULL) {
            return pfn(hIMC, lpSource, lpCandList, dwBufLen, uFlag);
        }
    }
    return 0;
}

// Forwarding function for ImeDestroy
BOOL WINAPI ImeDestroy(UINT uForce) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeDestroy pfn = (PFNImeDestroy)GetProcAddress(hDll, "ImeDestroy");
        if (pfn != NULL) {
            return pfn(uForce);
        }
    }
    return FALSE;
}

// Forwarding function for ImeEscape
LRESULT WINAPI ImeEscape(HIMC hIMC, UINT uSubFunc, LPVOID lpData) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeEscape pfn = (PFNImeEscape)GetProcAddress(hDll, "ImeEscape");
        if (pfn != NULL) {
            return pfn(hIMC, uSubFunc, lpData);
        }
    }
    return 0;
}

// Forwarding function for ImeProcessKey
BOOL WINAPI ImeProcessKey(HIMC hIMC, UINT vKey, LPARAM lKeyData, const LPBYTE lpbKeyState) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeProcessKey pfn = (PFNImeProcessKey)GetProcAddress(hDll, "ImeProcessKey");
        if (pfn != NULL) {
            return pfn(hIMC, vKey, lKeyData, lpbKeyState);
        }
    }
    return FALSE;
}

// Forwarding function for ImeSelect
BOOL WINAPI ImeSelect(HIMC hIMC, BOOL fSelect) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeSelect pfn = (PFNImeSelect)GetProcAddress(hDll, "ImeSelect");
        if (pfn != NULL) {
            return pfn(hIMC, fSelect);
        }
    }
    return FALSE;
}

// Forwarding function for ImeSetActiveContext
BOOL WINAPI ImeSetActiveContext(HIMC hIMC, BOOL fFocus) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeSetActiveContext pfn = (PFNImeSetActiveContext)GetProcAddress(hDll, "ImeSetActiveContext");
        if (pfn != NULL) {
            return pfn(hIMC, fFocus);
        }
    }
    return FALSE;
}

// Forwarding function for ImeToAsciiEx
UINT WINAPI ImeToAsciiEx(UINT uVKey, UINT uScanCode, CONST LPBYTE lpbKeyState, LPDWORD lpdwTransKey, UINT fuState, HIMC hIMC) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeToAsciiEx pfn = (PFNImeToAsciiEx)GetProcAddress(hDll, "ImeToAsciiEx");
        if (pfn != NULL) {
            return pfn(uVKey, uScanCode, lpbKeyState, lpdwTransKey, fuState, hIMC);
        }
    }
    return 0;
}

// Forwarding function for NotifyIME
BOOL WINAPI NotifyIME(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNNotifyIME pfn = (PFNNotifyIME)GetProcAddress(hDll, "NotifyIME");
        if (pfn != NULL) {
            return pfn(hIMC, dwAction, dwIndex, dwValue);
        }
    }
    return FALSE;
}

// Forwarding function for ImeRegisterWord
BOOL WINAPI ImeRegisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeRegisterWord pfn = (PFNImeRegisterWord)GetProcAddress(hDll, "ImeRegisterWord");
        if (pfn != NULL) {
            return pfn(lpRead, dw, lpStr);
        }
    }
    return FALSE;
}

// Forwarding function for ImeUnregisterWord
BOOL WINAPI ImeUnregisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeUnregisterWord pfn = (PFNImeUnregisterWord)GetProcAddress(hDll, "ImeUnregisterWord");
        if (pfn != NULL) {
            return pfn(lpRead, dw, lpStr);
        }
    }
    return FALSE;
}

// Forwarding function for ImeGetRegisterWordStyle
UINT WINAPI ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF lp) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeGetRegisterWordStyle pfn = (PFNImeGetRegisterWordStyle)GetProcAddress(hDll, "ImeGetRegisterWordStyle");
        if (pfn != NULL) {
            return pfn(nItem, lp);
        }
    }
    return 0;
}

// Forwarding function for ImeEnumRegisterWord
UINT WINAPI ImeEnumRegisterWord(REGISTERWORDENUMPROC lpfn, LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr, LPVOID lpData) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeEnumRegisterWord pfn = (PFNImeEnumRegisterWord)GetProcAddress(hDll, "ImeEnumRegisterWord");
        if (pfn != NULL) {
            return pfn(lpfn, lpRead, dw, lpStr, lpData);
        }
    }
    return 0;
}

// Forwarding function for ImeSetCompositionString
BOOL WINAPI ImeSetCompositionString(HIMC hIMC, DWORD dwIndex, LPCVOID lpComp, DWORD dwComp, LPCVOID lpRead, DWORD dwRead) {
    HMODULE hDll = LoadWeaselIMEDll();
    if (hDll != NULL) {
        PFNImeSetCompositionString pfn = (PFNImeSetCompositionString)GetProcAddress(hDll, "ImeSetCompositionString");
        if (pfn != NULL) {
            return pfn(hIMC, dwIndex, lpComp, dwComp, lpRead, dwRead);
        }
    }
    return FALSE;
}


// DllMain for cleanup
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_DETACH:
            if (g_hWeaselTSFX64 != NULL) {
                FreeLibrary(g_hWeaselTSFX64);
                g_hWeaselTSFX64 = NULL;
            }
            if (g_hWeaselTSFARM64 != NULL) {
                FreeLibrary(g_hWeaselTSFARM64);
                g_hWeaselTSFARM64 = NULL;
            }
            if (g_hWeaselIMEX64 != NULL) {
                FreeLibrary(g_hWeaselIMEX64);
                g_hWeaselIMEX64 = NULL;
            }
            if (g_hWeaselIMEARM64 != NULL) {
                FreeLibrary(g_hWeaselIMEARM64);
                g_hWeaselIMEARM64 = NULL;
            }
            break;
    }
    return TRUE;
}
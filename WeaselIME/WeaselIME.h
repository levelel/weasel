#pragma once

#ifdef WEASELIME_EXPORTS
#define WEASELIME_API __declspec(dllexport)
#else
#define WEASELIME_API __declspec(dllimport)
#endif

#include <WeaselIPC.h>
#include <KeyEvent.h>

#define MAX_COMPOSITION_SIZE 256

struct CompositionInfo {
  COMPOSITIONSTRING cs;
  WCHAR szCompStr[MAX_COMPOSITION_SIZE];
  WCHAR szResultStr[MAX_COMPOSITION_SIZE];
  void Reset() {
    memset(this, 0, sizeof(*this));
    cs.dwSize = sizeof(*this);
    cs.dwCompStrOffset = (DWORD)((ptrdiff_t)&szCompStr - (ptrdiff_t)this);
    cs.dwResultStrOffset = (DWORD)((ptrdiff_t)&szResultStr - (ptrdiff_t)this);
  }
};

class WeaselIME;

class HIMCMap : public std::map<HIMC, std::shared_ptr<WeaselIME> > {
 public:
  HIMCMap() : m_valid(true) {}
  ~HIMCMap() { m_valid = false; }
  std::mutex& get_mutex() { return m_mutex; }
  bool is_valid() const { return m_valid; }

 private:
  bool m_valid;
  std::mutex m_mutex;
};

class WeaselIME {
 public:
  static HINSTANCE GetModuleInstance();
  static void SetModuleInstance(HINSTANCE hModule);
  static HRESULT RegisterUIClass();
  static HRESULT UnregisterUIClass();
  static LPCWSTR GetUIClassName();
  static LRESULT WINAPI UIWndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);
  static BOOL IsIMEMessage(UINT uMsg);
  static std::shared_ptr<WeaselIME> GetInstance(HIMC hIMC);
  static void Cleanup();

  WEASELIME_API BOOL WINAPI ImeInquire(IMEINFO* lpIMEInfo,
                       LPWSTR lpszUIClass,
                       DWORD dwSystemInfoFlags);
  WEASELIME_API BOOL WINAPI ImeConfigure(HKL hKL, HWND hWnd, DWORD dwMode, LPVOID lpData);
  WEASELIME_API DWORD WINAPI ImeConversionList(HIMC hIMC,
                               LPCTSTR lpSource,
                               LPCANDIDATELIST lpCandList,
                               DWORD dwBufLen,
                               UINT uFlag);
  WEASELIME_API BOOL WINAPI ImeDestroy(UINT uForce);
  WEASELIME_API LRESULT WINAPI ImeEscape(HIMC hIMC, UINT uSubFunc, LPVOID lpData);
  WEASELIME_API BOOL WINAPI ImeProcessKey(HIMC hIMC,
                          UINT vKey,
                          LPARAM lKeyData,
                          const LPBYTE lpbKeyState);
  WEASELIME_API BOOL WINAPI ImeSelect(HIMC hIMC, BOOL fSelect);
  WEASELIME_API BOOL WINAPI ImeSetActiveContext(HIMC hIMC, BOOL fFocus);
  WEASELIME_API UINT WINAPI ImeToAsciiEx(UINT uVKey,
                         UINT uScanCode,
                         CONST LPBYTE lpbKeyState,
                         LPDWORD lpdwTransKey,
                         UINT fuState,
                         HIMC hIMC);
  WEASELIME_API BOOL WINAPI NotifyIME(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue);
  WEASELIME_API BOOL WINAPI ImeRegisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr);
  WEASELIME_API BOOL WINAPI ImeUnregisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr);
  WEASELIME_API UINT WINAPI ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF lp);
  WEASELIME_API UINT WINAPI ImeEnumRegisterWord(REGISTERWORDENUMPROC lpfn,
                                LPCTSTR lpRead,
                                DWORD dw,
                                LPCTSTR lpStr,
                                LPVOID lpData);
  WEASELIME_API BOOL WINAPI ImeSetCompositionString(HIMC hIMC,
                                    DWORD dwIndex,
                                    LPCVOID lpComp,
                                    DWORD dwComp,
                                    LPCVOID lpRead,
                                    DWORD dwRead);

  WeaselIME(HIMC hIMC);
  LRESULT OnIMESelect(BOOL fSelect);
  LRESULT OnIMEFocus(BOOL fFocus);
  LRESULT OnUIMessage(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);
  BOOL ProcessKeyEvent(UINT vKey, KeyInfo kinfo, const LPBYTE lpbKeyState);

 private:
  HRESULT _Initialize();
  HRESULT _Finalize();
  LRESULT _OnIMENotify(LPINPUTCONTEXT lpIMC, WPARAM wp, LPARAM lp);
  HRESULT _StartComposition();
  HRESULT _EndComposition(LPCWSTR composition);
  HRESULT _AddIMEMessage(UINT msg, WPARAM wp, LPARAM lp);
  void _SetCandidatePos(LPINPUTCONTEXT lpIMC);
  void _SetCompositionWindow(LPINPUTCONTEXT lpIMC);
  void _UpdateInputPosition(LPINPUTCONTEXT lpIMC, POINT pt);

 private:
  static HINSTANCE s_hModule;
  static HIMCMap s_instances;
  HIMC m_hIMC;
  bool m_composing;
  bool m_preferCandidatePos;
  weasel::Client m_client;
};

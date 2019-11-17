#include <windows.h>
#include <stdio.h>
#include <string>

#include "getpid.h"

void Debug(std::string) {}

int GetMemoryStatus(HANDLE hProc) {

   HINSTANCE hDll;

   PROCESS_MEMORY_COUNTERS pmc;
   pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS);

   hDll = LoadLibrary(TEXT("PSAPI.DLL"));
   if (hDll) {
     LPPMI GetProcessMemoryInfo = (LPPMI) GetProcAddress(hDll, "GetProcessMemoryInfo");
     if (GetProcessMemoryInfo) {
       if (hProc) {
         GetProcessMemoryInfo(hProc, &pmc, pmc.cb);
       }
       else
         Debug("NOPE: OpenProcess");
      }
      else
        Debug("NOPE: GetProcAddress");

      FreeLibrary(hDll);
   }
  else
    Debug("NOPE: LoadLibrary");

   return (int)pmc.WorkingSetSize;

}


int getpid(const char* name, DWORD *pids) {

  // Win/NT or 2000 or XP 
 
  // Load library and get the procedures explicitly. We do 
  // this so that we don't have to worry about modules using 
  // this code failing to load under Windows 9x, because 
  // it can't resolve references to the PSAPI.DLL. 

  HINSTANCE hInstLib;
  int iLenP, indx;
  char szName[MAX_PATH], szToTermUpper[MAX_PATH];
  BOOL bResult;
  DWORD aiPID[1000], iCb=1000, iNumProc;
  DWORD iCbneeded, i;
  HANDLE hProc;
  HMODULE hMod;
  int retPID=0;

  // PSAPI Function Pointers
  BOOL (WINAPI *lpfEnumProcesses)(DWORD*, DWORD cb, DWORD*);
  BOOL (WINAPI *lpfEnumProcessModules)(HANDLE, HMODULE*,  DWORD, LPDWORD);
  DWORD (WINAPI *lpfGetModuleBaseName)(HANDLE, HMODULE,  LPTSTR, DWORD); 

  // Transfer Process name into "szToTermUpper" and convert it to upper case 
  iLenP=strlen(name);
  if (iLenP<1 || iLenP>MAX_PATH) return 632;
  for (indx=0 ; indx < iLenP ; indx++)
    szToTermUpper[indx]=toupper(name[indx]);

  szToTermUpper[iLenP]=0;

  hInstLib = LoadLibraryA("PSAPI.DLL");
  if (hInstLib == NULL) return 0;
 
  // Get procedure addresses.
  lpfEnumProcesses = (BOOL(WINAPI*)(DWORD*, DWORD, DWORD*)) GetProcAddress(hInstLib, "EnumProcesses");
  lpfEnumProcessModules = (BOOL(WINAPI*)(HANDLE, HMODULE*, DWORD, LPDWORD)) GetProcAddress(hInstLib, "EnumProcessModules");
  lpfGetModuleBaseName =(DWORD (WINAPI*)(HANDLE, HMODULE, LPTSTR, DWORD )) GetProcAddress(hInstLib, "GetModuleBaseNameA");
 
  if (lpfEnumProcesses == NULL || lpfEnumProcessModules == NULL || lpfGetModuleBaseName == NULL) {
    FreeLibrary(hInstLib);
    return 0;
  }

  bResult=lpfEnumProcesses(aiPID, iCb, &iCbneeded);
  if (!bResult) {
    // Unable to get process list, EnumProcesses failed
    FreeLibrary(hInstLib);
    return 0;
  }

  // How many processes are there?
  //int maxmem = 0;

  iNumProc = iCbneeded / sizeof(DWORD);
  for (i = 0 ; i < iNumProc ; i++) {
    // Get the (module) name for this process

    strcpy(szName,"Unknown");
    // First, get a handle to the process
    hProc = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, aiPID[i]);
    // Now, get the process name
    if (hProc) {
      if (lpfEnumProcessModules(hProc,&hMod,sizeof(hMod),&iCbneeded)) {
        lpfGetModuleBaseName(hProc, hMod, szName, MAX_PATH);
      }
    }
    else
      continue;
      // We will match regardless of lower or upper case

    printf("PROCESS: %s\n", szName);

    if (strcmp(strupr(szName),szToTermUpper) == 0) {
      if (retPID < 100) {
        pids[retPID] = aiPID[i];
        ++retPID;
      }
    }
    CloseHandle(hProc);
  }

  return retPID;

}

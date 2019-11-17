#include <windows.h>
#include <stdio.h>

typedef struct _PROCESS_MEMORY_COUNTERS {
   DWORD cb;
   DWORD PageFaultCount;
   SIZE_T PeakWorkingSetSize;
   SIZE_T WorkingSetSize;
   SIZE_T QuotaPeakPagedPoolUsage;
   SIZE_T QuotaPagedPoolUsage;
   SIZE_T QuotaPeakNonPagedPoolUsage;
   SIZE_T QuotaNonPagedPoolUsage;
   SIZE_T PagefileUsage;
   SIZE_T PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS, *PPROCESS_MEMORY_COUNTERS;

typedef BOOL (WINAPI *LPPMI) (HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD);

int GetMemoryStatus(HANDLE hProc);
int getpid(const char* name, DWORD *pids);

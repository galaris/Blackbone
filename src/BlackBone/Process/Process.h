#pragma once

#include "ProcessCore.h"
#include "ProcessMemory.h"
#include "ProcessModules.h"
#include "Threads/Threads.h"
#include "RPC/RemoteExec.h"
#include "RPC/RemoteHook.h"
#include "../ManualMap/Native/NtLoader.h"
#include "../ManualMap/MMap.h"

#include "../Include/NativeStructures.h"

#include <string>
#include <list>

namespace blackbone
{

#define DEFAULT_ACCESS_P  PROCESS_QUERY_INFORMATION | \
                          PROCESS_VM_READ           | \
                          PROCESS_VM_WRITE          | \
                          PROCESS_VM_OPERATION      | \
                          PROCESS_CREATE_THREAD     | \
                          PROCESS_SET_QUOTA         | \
                          PROCESS_TERMINATE         | \
                          PROCESS_DUP_HANDLE
class Process
{
public:
    BLACKBONE_API Process();
    BLACKBONE_API ~Process(void);

    /// <summary>
    /// Attach to existing process
    /// </summary>
    /// <param name="pid">Process ID</param>
    /// <param name="access">Access mask</param>
    /// <returns>Status</returns>
    BLACKBONE_API NTSTATUS Attach( DWORD pid, DWORD access = DEFAULT_ACCESS_P );

    /// <summary>
    /// Get process ID
    /// </summary>
    /// <returns>Process ID</returns>
    BLACKBONE_API inline DWORD pid() const { return _core.pid(); }

    /// <summary>
    /// Checks if process still exists
    /// </summary>
    /// <returns></returns>
    BLACKBONE_API bool valid();

    /// <summary>
    /// Search for process by executable name
    /// </summary>
    /// <param name="name">Process name. If empty - function will retrieve all existing processes</param>
    /// <param name="found">Found processses</param>
    BLACKBONE_API static void EnumByName( const std::wstring& name, std::vector<DWORD>& found );

    //
    // Subroutines
    //
    BLACKBONE_API inline ProcessCore&    core()      { return _core; }       // Core routines and native 
    BLACKBONE_API inline ProcessMemory&  memory()    { return _memory; }     // Memory manipulations
    BLACKBONE_API inline ProcessModules& modules()   { return _modules; }    // Module management
    BLACKBONE_API inline ProcessThreads& threads()   { return _threads; }    // Threads
    BLACKBONE_API inline RemoteHook&     hooks()     { return _hooks; }      // Hooking code remotely
    BLACKBONE_API inline RemoteExec&     remote()    { return _remote; }     // Remote code execution
    BLACKBONE_API inline MMap&           mmap()      { return _mmap; }       // Manual module mapping
    BLACKBONE_API inline NtLdr&          nativeLdr() { return _nativeLdr; }  // Native loader routines

private:
    NTSTATUS GrantPriviledge( const std::basic_string<TCHAR>& name );

    Process(const Process&) = delete;
    Process& operator =(const Process&) = delete;

private:
    ProcessCore    _core;       // Core routines and native subsystem
    ProcessModules _modules;    // Module management
    ProcessMemory  _memory;     // Memory manipulations
    ProcessThreads _threads;    // Threads
    RemoteHook     _hooks;      // Hooking code remotely
    RemoteExec     _remote;     // Remote code execution
    MMap           _mmap;       // Manual module mapping
    NtLdr          _nativeLdr;  // Native loader routines
};

}

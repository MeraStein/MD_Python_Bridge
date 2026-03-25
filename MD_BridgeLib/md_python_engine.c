#include <Python.h>
#include <stdio.h>
#include <shlwapi.h>
#include <direct.h> 
#include "md_python_engine.h"
#pragma comment(lib, "shlwapi.lib")

MD_API void RunPythonScript(const char* scriptPath) {
    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

    // --- 1. Path Calculations ---
    wchar_t fullPath[MAX_PATH];
    GetModuleFileNameW(NULL, fullPath, MAX_PATH);
    PathRemoveFileSpecW(fullPath);

    wchar_t wSubPath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, scriptPath, -1, wSubPath, MAX_PATH);
    PathAppendW(fullPath, wSubPath);

    wchar_t scriptDir[MAX_PATH];
    wcscpy_s(scriptDir, MAX_PATH, fullPath);
    PathRemoveFileSpecW(scriptDir);

    //Clean and Reset Python Environment (Fixing Redefinition)
    PyObject* sysPathObj = PySys_GetObject("path"); // Renamed to avoid confusion
    if (sysPathObj) {
        // Clear all previous paths
        PyList_SetSlice(sysPathObj, 0, PyList_Size(sysPathObj), NULL);
        // Add current script directory
        PyList_Append(sysPathObj, PyUnicode_FromWideChar(scriptDir, -1));
    }

    // Force Python to forget previously loaded modules
    PyRun_SimpleString(
        "import sys\n"
        "for m in list(sys.modules.keys()):\n"
        "    if m not in sys.builtin_module_names and m != 'sys':\n"
        "        del sys.modules[m]\n"
    );

    // Execution
    if (_wchdir(scriptDir) != 0) {
        printf("Error: Cant reach folder %ls\n", scriptDir);
    }

    FILE* fp = NULL;
    _wfopen_s(&fp, fullPath, L"r");
    if (fp) {
        PyRun_SimpleFile(fp, scriptPath);
        fclose(fp);
    }
    else {
        printf("Error: File not found at %ls\n", fullPath);
    }
}
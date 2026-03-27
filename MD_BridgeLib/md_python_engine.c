#define _CRT_SECURE_NO_WARNINGS
#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <shlwapi.h>
#include <direct.h>
#include "md_python_engine.h"
#pragma comment(lib, "shlwapi.lib")

// running python script by path
MD_API void RunPythonScript(const char* scriptPath) {
    wchar_t dllFolderPath[MAX_PATH];
    HMODULE hModule = GetModuleHandleW(L"MD_BridgeLib.dll");
    if (hModule != NULL) {
        GetModuleFileNameW(hModule, dllFolderPath, MAX_PATH);
        PathRemoveFileSpecW(dllFolderPath);
        SetDllDirectoryW(dllFolderPath);
    }

    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

    // Path Calculations
    wchar_t rootPath[MAX_PATH];
    GetModuleFileNameW(NULL, rootPath, MAX_PATH);

    int found = 0;
    while (PathRemoveFileSpecW(rootPath)) { 
        wchar_t checkPath[MAX_PATH];
        wcscpy_s(checkPath, MAX_PATH, rootPath);
        PathAppendW(checkPath, L"python_projects"); 

        if (PathFileExistsW(checkPath)) { 
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Error: Could not find 'python_projects' folder in any parent directory.\n");
        return;
    }

    wchar_t wSubPath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, scriptPath, -1, wSubPath, MAX_PATH);

    wchar_t fullPath[MAX_PATH];
    wcscpy_s(fullPath, MAX_PATH, rootPath);
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


//running python script in python progects by word
MD_API void runPythonInProjectsByWord(const char* word) {
    char wordArr[50] = {0};

    for (int i = 0; word[i] != '\0'; i++) {
        wordArr[i] = (char)tolower((unsigned char)word[i]);
    }

    char scriptPath[260] = { 0 };
    sprintf_s(scriptPath, sizeof(scriptPath), "python_projects\\python_%s\\main.py", wordArr);
    RunPythonScript(scriptPath);
}


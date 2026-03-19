#include <Python.h>
#include <stdio.h>
#include <shlwapi.h>
#include <direct.h> // for _wchdir
#include "md_python_engine.h"
#pragma comment(lib, "shlwapi.lib")

MD_API void RunPythonScript(const char* scriptPath) {
    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

    PyObject* sysPath = PySys_GetObject("path");
    if (!sysPath) return;

    // 1. for the path of EXE
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    PathRemoveFileSpecW(exePath);
    PyList_Append(sysPath, PyUnicode_FromWideChar(exePath, -1));

    // 2. treat path of rhe script and exchange job folder
    wchar_t wScriptPath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, scriptPath, -1, wScriptPath, MAX_PATH);

    wchar_t scriptDir[MAX_PATH];
    wcscpy_s(scriptDir, MAX_PATH, wScriptPath);
    PathRemoveFileSpecW(scriptDir);

    PyList_Append(sysPath, PyUnicode_FromWideChar(scriptDir, -1));

    if (_wchdir(scriptDir) != 0) {
        printf("Error: Failed to change working directory to %ls\n", scriptDir);
    }


    // 3. running the python file
    FILE* fp = NULL;
    fopen_s(&fp, scriptPath, "r");
    if (fp) {
        PyRun_SimpleFile(fp, scriptPath);
        fclose(fp);
    }
    else {
        printf("Error: Could not find %s\n", scriptPath);
    }
}
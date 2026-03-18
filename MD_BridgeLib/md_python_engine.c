#include <Python.h>
#include <stdio.h>
#include <shlwapi.h>
#include "md_python_engine.h"
#pragma comment(lib, "shlwapi.lib")

MD_API void RunPythonScript(const char* scriptPath) {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    PathRemoveFileSpecW(path);

    Py_Initialize();

    PyObject* sysPath = PySys_GetObject("path");
    PyList_Append(sysPath, PyUnicode_FromWideChar(path, -1));

    FILE* fp = NULL;
    fopen_s(&fp, scriptPath, "r");
    if (fp) {
        PyRun_SimpleFile(fp, scriptPath);
        fclose(fp);
    }
    else {
        printf("Error: Could not find %s\n", scriptPath);
    }

    Py_Finalize();
}
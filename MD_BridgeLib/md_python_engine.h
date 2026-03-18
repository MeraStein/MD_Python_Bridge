#pragma once

#ifdef MDPYTHONRUNNER_EXPORTS
#define MD_API __declspec(dllexport)
#else
#define MD_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    MD_API void RunPythonScript(const char* scriptPath);

#ifdef __cplusplus
}
#endif
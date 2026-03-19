#include <stdio.h>

// Import the script execution function from the external Bridge DLL
__declspec(dllimport) void RunPythonScript(const char* scriptPath);

int main() {
    printf("--- MagicData Tester (C Mode) --- \n");
    RunPythonScript("C:\\Users\\meroo\\Downloads\\python\\main.py");
    printf("\n--- Execution Finished. Press Enter to exit ---\n");

    int c = getchar(); //waiting for the user to press to exit
    return 0;
}
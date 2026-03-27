#include "MD_Runner.h"

int main() {
    char codeWord[50];
    /*char scriptPath[260];*/
    int isExit = 0;
    char tempChar;

    while (!isExit) {
        system("cls");
        memset(codeWord, 0, sizeof(codeWord));
       /* memset(scriptPath, 0, sizeof(scriptPath));*/
        fflush(stdin);

        printf("\n========================================\n");
        printf("   MagicData Python Runner - Console    \n");
        printf("========================================\n");
        printf("Enter project code (or 'EXIT' to quit): ");

        if (gets_s(codeWord, sizeof(codeWord)) == NULL) {
            printf("Error reading input.\n");
            break;
        }

        for (int i = 0; codeWord[i] != '\0'; i++) {
            codeWord[i] = (char)tolower((unsigned char)codeWord[i]);
        }

        if (!strcmp(codeWord, "exit")) {
            printf("\n- - - exiting system - - -\n");
            isExit = 1;
            break;
        }

        runPythonInProjectsByWord(codeWord);
        /*sprintf_s(scriptPath, sizeof(scriptPath), "python_projects\\python_%s\\main.py", codeWord);*/

        printf("\n>>> STARTING EXECUTION: %s\n", codeWord);
        printf("----------------------------------------\n");

        runPythonInProjectsByWord(codeWord);
        /*RunPythonScript(scriptPath);*/

        printf("----------------------------------------\n");
        printf(">>> EXECUTION FINISHED.\n");
        printf("\nPress any key to return to menu...");
        fflush(stdin);
        tempChar = _getch();
    }

    return 0;
}
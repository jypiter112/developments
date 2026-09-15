#include <windows.h>
#include <stdio.h>

void executeCommand(const char* command) {
    // Create pipes for the child process's input and output
    HANDLE hReadPipe, hWritePipe, hReadOutput, hWriteOutput;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    // Create the pipe for output
    if (!CreatePipe(&hReadOutput, &hWriteOutput, &sa, 0)) {
        fprintf(stderr, "Error creating output pipe.\n");
        return;
    }

    // Create the pipe for input
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        fprintf(stderr, "Error creating input pipe.\n");
        CloseHandle(hReadOutput);
        return;
    }

    // Set the read end of the output pipe to be inherited by the child process
    SetHandleInformation(hReadOutput, HANDLE_FLAG_INHERIT, 0);

    // Prepare the STARTUPINFO structure
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = hWriteOutput; // Redirect standard output to the output pipe
    si.hStdError = hWriteOutput;  // Redirect standard error to the output pipe
    si.hStdInput = hReadPipe;     // Redirect standard input from the input pipe
    si.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process
    if (!CreateProcess(NULL, (LPSTR)command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Error creating process: %d\n", GetLastError());
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        CloseHandle(hReadOutput);
        CloseHandle(hWriteOutput);
        return;
    }

    // Close unnecessary handles in the parent process
    CloseHandle(hWriteOutput);
    CloseHandle(hReadPipe);

    // Example: Write to the child process
    const char* input = "echo Writing to child process\n";
    DWORD bytesWritten;
    WriteFile(hWritePipe, input, strlen(input), &bytesWritten, NULL);

    // Read output from the child process
    char buffer[128];
    DWORD bytesRead;
    while (ReadFile(hReadOutput, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("%s", buffer);     // Print the output
    }

    // Cleanup
    CloseHandle(hWritePipe);
    CloseHandle(hReadOutput);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    const char* command = "cmd.exe"; // Run command prompt
    executeCommand(command);
    return 0;
}

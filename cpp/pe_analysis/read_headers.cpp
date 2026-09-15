#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string, std::ifstream, std::ios, std::vector, std::cout;

string readFile(const string &fileName)
{
    ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);

    vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return string(bytes.data(), fileSize);
}
int readPEFile(const string& fileName){
    ifstream exeFile(fileName.c_str(), ios::binary);
    if(!exeFile){
        std::cerr << "Failed to open file\n";
        return 1;
    }

    // Read dos headers
    IMAGE_DOS_HEADER dosHeader;
    exeFile.read(reinterpret_cast<char*>(&dosHeader), sizeof(dosHeader));
    if(dosHeader.e_magic != IMAGE_DOS_SIGNATURE){
        std::cerr << "Net PE file\n";
        return 1;
    }
    cout << "Dos addr: 0x" << (DWORD)&dosHeader << "\n";

    // Read NT Headers
    IMAGE_NT_HEADERS ntHeaders;
    exeFile.seekg(dosHeader.e_lfanew, std::ios::beg);
    exeFile.read(reinterpret_cast<char*>(&ntHeaders), sizeof(ntHeaders));
    if(ntHeaders.Signature != IMAGE_NT_SIGNATURE){
        std::cerr << "Not NT sign\n";
        return 1;
    }
    cout << "NT addr: 0x" << (DWORD)&ntHeaders << "\n";

    // Read section headers
    IMAGE_SECTION_HEADER sectHeader;
    vector<IMAGE_SECTION_HEADER> sections(ntHeaders.FileHeader.NumberOfSections);
    cout << "Sect head count: " << sections.size() << "\n";

    exeFile.seekg(dosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS), std::ios::beg);
    exeFile.read(reinterpret_cast<char*>(sections.data()), sizeof(IMAGE_SECTION_HEADER) * ntHeaders.FileHeader.NumberOfSections);
    // Loop through SECTIONs
    for (const auto& section : sections){
        string sectName((char*)(section.Name));
        cout << sectName << "\n";
    }

    exeFile.close();
    return 0;
}

int main(int argc, char** argv) {
    readPEFile("hack.exe");
    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

using std::string;
using std::ifstream;
using std::ios;
using std::vector;
using std::cout;

void printDOSHeader(const IMAGE_DOS_HEADER& dosHeader) {
    cout << "\n=== DOS Header Information ===\n";
    cout << "Magic number: 0x" << std::hex << dosHeader.e_magic << "\n";
    cout << "Bytes on last page: " << std::dec << dosHeader.e_cblp << "\n";
    cout << "Pages in file: " << dosHeader.e_cp << "\n";
    cout << "Relocations: " << dosHeader.e_crlc << "\n";
    cout << "Size of header in paragraphs: " << dosHeader.e_cparhdr << "\n";
    cout << "Minimum extra paragraphs needed: " << dosHeader.e_minalloc << "\n";
    cout << "Maximum extra paragraphs needed: " << dosHeader.e_maxalloc << "\n";
    cout << "Initial (relative) SS value: 0x" << std::hex << dosHeader.e_ss << "\n";
    cout << "Initial SP value: 0x" << dosHeader.e_sp << "\n";
    cout << "Checksum: 0x" << dosHeader.e_csum << "\n";
    cout << "Initial IP value: 0x" << dosHeader.e_ip << "\n";
    cout << "Initial (relative) CS value: 0x" << dosHeader.e_cs << "\n";
    cout << "File address of relocation table: 0x" << dosHeader.e_lfarlc << "\n";
    cout << "Overlay number: " << std::dec << dosHeader.e_ovno << "\n";
    cout << "Reserved words: " << dosHeader.e_res[0] << " " << dosHeader.e_res[1] << " " 
         << dosHeader.e_res[2] << " " << dosHeader.e_res[3] << "\n";
    cout << "OEM identifier: 0x" << std::hex << dosHeader.e_oemid << "\n";
    cout << "OEM information: 0x" << dosHeader.e_oeminfo << "\n";
    cout << "Reserved words: " << dosHeader.e_res2[0] << " " << dosHeader.e_res2[1] << " " 
         << dosHeader.e_res2[2] << " " << dosHeader.e_res2[3] << " " 
         << dosHeader.e_res2[4] << " " << dosHeader.e_res2[5] << " " 
         << dosHeader.e_res2[6] << " " << dosHeader.e_res2[7] << " " 
         << dosHeader.e_res2[8] << " " << dosHeader.e_res2[9] << "\n";
    cout << "File address of new exe header: 0x" << dosHeader.e_lfanew << "\n";
}

void printNTHeaders(const IMAGE_NT_HEADERS64& ntHeaders) {
    cout << "\n=== NT Headers Information ===\n";
    cout << "Signature: 0x" << std::hex << ntHeaders.Signature << "\n";
    
    cout << "\nFile Header:\n";
    cout << "Machine: 0x" << std::hex << ntHeaders.FileHeader.Machine << "\n";
    cout << "Number of sections: " << std::dec << ntHeaders.FileHeader.NumberOfSections << "\n";
    cout << "Time date stamp: " << ntHeaders.FileHeader.TimeDateStamp << "\n";
    cout << "Pointer to symbol table: 0x" << std::hex << ntHeaders.FileHeader.PointerToSymbolTable << "\n";
    cout << "Number of symbols: " << std::dec << ntHeaders.FileHeader.NumberOfSymbols << "\n";
    cout << "Size of optional header: " << ntHeaders.FileHeader.SizeOfOptionalHeader << "\n";
    cout << "Characteristics: 0x" << std::hex << ntHeaders.FileHeader.Characteristics << "\n";
    
    cout << "\nOptional Header:\n";
    cout << "Magic: 0x" << std::hex << ntHeaders.OptionalHeader.Magic << "\n";
    cout << "Major linker version: " << std::dec << ntHeaders.OptionalHeader.MajorLinkerVersion << "\n";
    cout << "Minor linker version: " << ntHeaders.OptionalHeader.MinorLinkerVersion << "\n";
    cout << "Size of code: " << ntHeaders.OptionalHeader.SizeOfCode << "\n";
    cout << "Size of initialized data: " << ntHeaders.OptionalHeader.SizeOfInitializedData << "\n";
    cout << "Size of uninitialized data: " << ntHeaders.OptionalHeader.SizeOfUninitializedData << "\n";
    cout << "Address of entry point: 0x" << std::hex << ntHeaders.OptionalHeader.AddressOfEntryPoint << "\n";
    cout << "Base of code: 0x" << ntHeaders.OptionalHeader.BaseOfCode << "\n";
    cout << "Image base: 0x" << ntHeaders.OptionalHeader.ImageBase << "\n";
    cout << "Section alignment: " << std::dec << ntHeaders.OptionalHeader.SectionAlignment << "\n";
    cout << "File alignment: " << ntHeaders.OptionalHeader.FileAlignment << "\n";
    cout << "Major operating system version: " << ntHeaders.OptionalHeader.MajorOperatingSystemVersion << "\n";
    cout << "Minor operating system version: " << ntHeaders.OptionalHeader.MinorOperatingSystemVersion << "\n";
    cout << "Major image version: " << ntHeaders.OptionalHeader.MajorImageVersion << "\n";
    cout << "Minor image version: " << ntHeaders.OptionalHeader.MinorImageVersion << "\n";
    cout << "Major subsystem version: " << ntHeaders.OptionalHeader.MajorSubsystemVersion << "\n";
    cout << "Minor subsystem version: " << ntHeaders.OptionalHeader.MinorSubsystemVersion << "\n";
    cout << "Win32 version value: " << ntHeaders.OptionalHeader.Win32VersionValue << "\n";
    cout << "Size of image: " << ntHeaders.OptionalHeader.SizeOfImage << "\n";
    cout << "Size of headers: " << ntHeaders.OptionalHeader.SizeOfHeaders << "\n";
    cout << "Check sum: 0x" << std::hex << ntHeaders.OptionalHeader.CheckSum << "\n";
    cout << "Subsystem: 0x" << ntHeaders.OptionalHeader.Subsystem << "\n";
    cout << "DLL characteristics: 0x" << ntHeaders.OptionalHeader.DllCharacteristics << "\n";
    cout << "Size of stack reserve: 0x" << ntHeaders.OptionalHeader.SizeOfStackReserve << "\n";
    cout << "Size of stack commit: 0x" << ntHeaders.OptionalHeader.SizeOfStackCommit << "\n";
    cout << "Size of heap reserve: 0x" << ntHeaders.OptionalHeader.SizeOfHeapReserve << "\n";
    cout << "Size of heap commit: 0x" << ntHeaders.OptionalHeader.SizeOfHeapCommit << "\n";
    cout << "Loader flags: 0x" << ntHeaders.OptionalHeader.LoaderFlags << "\n";
    cout << "Number of RVA and sizes: " << std::dec << ntHeaders.OptionalHeader.NumberOfRvaAndSizes << "\n";
}

void printSectionHeaders(const vector<IMAGE_SECTION_HEADER>& sections) {
    cout << "\n=== Section Headers ===\n";
    cout << "Number of sections: " << sections.size() << "\n\n";
    
    for (const auto& section : sections) {
        string sectName((char*)(section.Name));
        cout << "Section: " << sectName << "\n";
        cout << "Virtual address: 0x" << std::hex << section.VirtualAddress << "\n";
        cout << "Virtual size: 0x" << section.Misc.VirtualSize << "\n";
        cout << "Raw data pointer: 0x" << section.PointerToRawData << "\n";
        cout << "Raw data size: 0x" << section.SizeOfRawData << "\n";
        cout << "Relocation pointer: 0x" << section.PointerToRelocations << "\n";
        cout << "Relocation count: " << std::dec << section.NumberOfRelocations << "\n";
        cout << "Line number pointer: 0x" << std::hex << section.PointerToLinenumbers << "\n";
        cout << "Line number count: " << std::dec << section.NumberOfLinenumbers << "\n";
        cout << "Characteristics: 0x" << std::hex << section.Characteristics << "\n\n";
    }
}

void printImportDirectory(ifstream& exeFile, const vector<IMAGE_SECTION_HEADER>& sections, 
                         DWORD importDirRVA, DWORD importDirSize) {
    if (importDirRVA && importDirSize) {
        cout << "\nImport Directory Table:\n";
        
        DWORD importDirOffset = 0;
        for (const auto& section : sections) {
            if (importDirRVA >= section.VirtualAddress && 
                importDirRVA < (section.VirtualAddress + section.Misc.VirtualSize)) {
                importDirOffset = section.PointerToRawData + (importDirRVA - section.VirtualAddress);
                break;
            }
        }

        if (importDirOffset) {
            exeFile.seekg(importDirOffset, std::ios::beg);
            IMAGE_IMPORT_DESCRIPTOR importDesc;
            
            while (true) {
                exeFile.read(reinterpret_cast<char*>(&importDesc), sizeof(importDesc));
                if (!importDesc.Name) break;

                DWORD nameOffset = importDesc.Name;
                for (const auto& section : sections) {
                    if (nameOffset >= section.VirtualAddress && 
                        nameOffset < (section.VirtualAddress + section.Misc.VirtualSize)) {
                        DWORD nameFileOffset = section.PointerToRawData + (nameOffset - section.VirtualAddress);
                        exeFile.seekg(nameFileOffset, std::ios::beg);
                        char dllName[256];
                        exeFile.getline(dllName, sizeof(dllName), '\0');
                        cout << "DLL: " << dllName << "\n";
                        break;
                    }
                }
            }
        }
    }
}

int readPEFile(const string& fileName) {
    IMAGE_DOS_HEADER dosHeader;
    IMAGE_NT_HEADERS64 ntHeaders;
    vector<IMAGE_SECTION_HEADER> sections(0);
    DWORD importDirRVA = NULL;
    DWORD importDirSize = NULL;

    ifstream exeFile(fileName.c_str(), ios::binary);
    if(!exeFile) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    // Read dos headers
    exeFile.read(reinterpret_cast<char*>(&dosHeader), sizeof(dosHeader));
    if(dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
        std::cerr << "Not PE file\n";
        return 1;
    }
    printDOSHeader(dosHeader);

    // Read NT Headers
    exeFile.seekg(dosHeader.e_lfanew, ios::beg);
    exeFile.read(reinterpret_cast<char*>(&ntHeaders), sizeof(ntHeaders));
    if(ntHeaders.Signature != IMAGE_NT_SIGNATURE) {
        std::cerr << "Not NT signature\n";
        return 1;
    }
    printNTHeaders(ntHeaders);

    // Read section headers
    sections.resize(ntHeaders.FileHeader.NumberOfSections);
    exeFile.seekg(dosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS64), ios::beg);
    exeFile.read(reinterpret_cast<char*>(sections.data()), sizeof(IMAGE_SECTION_HEADER) * ntHeaders.FileHeader.NumberOfSections);
    printSectionHeaders(sections);

    // Read Import Directory Table
    importDirRVA = ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    importDirSize = ntHeaders.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
    printImportDirectory(exeFile, sections, importDirRVA, importDirSize);

    exeFile.close();
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <exe_file_path>\n";
        return 1;
    }

    char currentDir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDir);
    std::cout << "Current working directory: " << currentDir << "\n";
    
    if (readPEFile(argv[1]) != 0) {
        std::cerr << "Failed to process file: " << argv[1] << "\n";
        return 1;
    }
    return 0;
}

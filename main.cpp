#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>
#include <string>
#include <Psapi.h>
#include <sstream>

struct Signature {
    std::string pattern;
    uint32_t offset;
    uint32_t extra;

    Signature(const std::string& pat, uint32_t off, uint32_t ext)
        : pattern(pat), offset(off), extra(ext) {}

    std::vector<uint8_t> parse_pattern() const {
        std::vector<uint8_t> bytes;
        std::istringstream patternStream(pattern);
        std::string byteStr;

        while (patternStream >> byteStr) {
            if (byteStr == "?" || byteStr == "??") {
                bytes.push_back(0);
            }
            else {
                bytes.push_back(static_cast<uint8_t>(strtol(byteStr.c_str(), nullptr, 16)));
            }
        }
        return bytes;
    }

    void find(const std::vector<uint8_t>& memory, HANDLE processHandle, uintptr_t moduleBase) const {
        std::vector<uint8_t> pattern = parse_pattern();
        for (size_t i = 1000000; i < memory.size(); ++i) {
            bool patternMatch = true;
            for (size_t j = 0; j < pattern.size(); ++j) {
                if (pattern[j] != 0 && memory[i + j] != pattern[j]) {
                    patternMatch = false;
                    break;
                }
            }
            if (patternMatch) {
                uintptr_t patternAddress = moduleBase + i;
                int32_t of;
                ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(patternAddress + offset), &of, sizeof(of), nullptr);
                uintptr_t result = patternAddress + of + extra;
                std::cout << "+ 0x" << std::hex << (result - moduleBase) << std::endl;
            }
        }
    }
};

// Helper function to convert TCHAR to std::string or std::wstring
#ifdef UNICODE
std::wstring toString(const TCHAR* tcharArray) {
    return std::wstring(tcharArray);
}
#else
std::string toString(const TCHAR* tcharArray) {
    return std::string(tcharArray);
}
#endif

std::string wstringToString(const std::wstring& wstr) {
    std::string str(wstr.begin(), wstr.end());
    return str;
}

HANDLE getProcessHandle(const std::string& processName) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &processEntry)) {
        do {
            std::string exeFileName = wstringToString(processEntry.szExeFile);
            if (processName == exeFileName) {
                CloseHandle(snapshot);
                return OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processEntry.th32ProcessID);
            }
        } while (Process32Next(snapshot, &processEntry));
    }
    CloseHandle(snapshot);
    return nullptr;
}


MODULEINFO getModuleInfo(HANDLE processHandle, const std::string& moduleName) {
    HMODULE hMods[1024];
    DWORD cbNeeded;
    MODULEINFO modInfo = { 0 };

    if (EnumProcessModules(processHandle, hMods, sizeof(hMods), &cbNeeded)) {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            char szModName[MAX_PATH];
            if (GetModuleBaseNameA(processHandle, hMods[i], szModName, sizeof(szModName) / sizeof(char))) {
                if (moduleName == szModName) {
                    GetModuleInformation(processHandle, hMods[i], &modInfo, sizeof(modInfo));
                    break;
                }
            }
        }
    }
    return modInfo;
}

std::vector<uint8_t> readMemoryBytes(HANDLE processHandle, uintptr_t address, size_t size) {
    std::vector<uint8_t> buffer(size);
    ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), buffer.data(), size, nullptr);
    return buffer;
}

int main() {
    std::cerr << "By 5komar (Catrine)\n" << std::endl;
    std::cerr << "Discord Server: https://discord.gg/tcnksFMCR9\n" << std::endl;

    Signature localPlayerSig("48 8B 0D ? ? ? ? 48 85 C9 74 65 83 FF FF", 3, 7);
    Signature viewMatrixSig("48 8D ? ? ? ? ? 48 C1 E0 06 48 03 C1 C3", 3, 7);
    Signature entityListSig("48 8B 0D ? ? ? ? 8B C5 48 C1 E8", 3, 7);
    Signature CCameraManagerSig("48 8D 3D ? ? ? ? 8B D9", 3, 7);
    Signature gameEntitySystemSig("48 8B 1D ? ? ? ? 48 89 1D", 3, 7);
    Signature viewRenderSig("48 89 05 ? ? ? ? 48 8B C8 48 85 C0", 3, 7);
    Signature GameTraceManageSig("48 8B 0D ? ? ? ? 48 8D 45 ? 48 89 44 24 ? 4C 8D 44 24 ? 4C 8B CF", 3, 7);


    std::string processName = "project8.exe";
    HANDLE processHandle = getProcessHandle(processName);
    if (!processHandle) {
        std::cerr << "Game process not found!" << std::endl;
        return 1;
    }

    MODULEINFO moduleInfo = getModuleInfo(processHandle, "client.dll");

    if (!moduleInfo.lpBaseOfDll) {
        std::cerr << "client.dll not found!" << std::endl;
        return 1;
    }

    std::vector<uint8_t> memory = readMemoryBytes(processHandle, reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll), moduleInfo.SizeOfImage);

    std::cout << "dwLocalPlayerController:" << std::endl;
    localPlayerSig.find(memory, processHandle, reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll));
    std::cout << "dwViewMatrix:" << std::endl;
    viewMatrixSig.find(memory, processHandle, reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll));
    std::cout << "dwEntityList:" << std::endl;
    entityListSig.find(memory, processHandle, reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll));
    std::cout << "dwGameTraceManage:" << std::endl;
    GameTraceManageSig.find(memory, processHandle, reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll));
    std::cout << "dwViewRender:" << std::endl;
    viewRenderSig.find(memory, processHandle, reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll));
    std::cout << "dwGameEntitySystem:" << std::endl;
    gameEntitySystemSig.find(memory, processHandle, reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll));
    std::cout << "CCameraManager:" << std::endl;
    CCameraManagerSig.find(memory, processHandle, reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll));

    CloseHandle(processHandle);
    return 0;
}

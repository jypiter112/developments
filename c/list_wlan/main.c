#include <stdio.h>
#include <windows.h>
#include <wlanapi.h>
#include <string.h>

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

CHAR* wchar_to_cstring(const WCHAR* wstr)
{
    if(!wstr){
        return NULL;
    }
    int wstr_len = (int) wcslen(wstr) + 1;
    int num_chars = WideCharToMultiByte(CP_UTF8, 0, wstr, wstr_len, NULL, 0, NULL, NULL);
    CHAR* strTo = (CHAR*) malloc((num_chars + 1) * sizeof(CHAR));
    if (strTo)
    {
        WideCharToMultiByte(CP_UTF8, 0, wstr, wstr_len, strTo, num_chars, NULL, NULL);
        strTo[num_chars] = '\0';
    } else{
        free(strTo);
    }
    return strTo;
}
void print_interface_state(WLAN_INTERFACE_STATE isState){
    switch(isState){
    case wlan_interface_state_not_ready:
        printf("Not ready\n");
        break;
    case wlan_interface_state_connected:
        printf("Connected\n");
        break;
    case wlan_interface_state_disconnected:
        printf("Disconnected\n");
        break;
    default:
        printf("Unknown state\n");
        break;
    }
}
void print_network_info(PWLAN_AVAILABLE_NETWORK pNetworkEntry, unsigned int j){
    wprintf(L"\n\nNetwork name [%d]: %s\n", j, pNetworkEntry->strProfileName);
    printf("Network type [%d] %d\n", j, pNetworkEntry->dot11BssType);
    printf("Connectable [%d] ", j);

    if(pNetworkEntry->bNetworkConnectable){
        printf("Yes\n");
    } else {
        printf("No\n");
        printf("Reason: %lu\n", pNetworkEntry->wlanNotConnectableReason);
    }

    printf("Connection quality: [%d] %lu\n", j, pNetworkEntry->wlanSignalQuality);
    
    printf("Security enabled [%d] ", j);
    if(pNetworkEntry->bSecurityEnabled){
        printf("Yes\n");
    } else {
        printf("No\n");
    }

    printf("Flags [%d] 0x%lx\n", j, pNetworkEntry->dwFlags);
    if(pNetworkEntry->dwFlags == 0x03){
        printf("Currently connected [%d]\n", j);
    }
}
void print_interface_info(PWLAN_INTERFACE_INFO pInterfaceInfo, int* pinterfaceRet, WCHAR *GuidString, unsigned int i){
    printf("Interface Index: %d\n", i);
    *pinterfaceRet = StringFromGUID2(&pInterfaceInfo->InterfaceGuid, (LPOLESTR)GuidString,(sizeof(GuidString)/sizeof(*GuidString)));
    if(pinterfaceRet != 0){
        printf("IntefaceGUID [%d] %s\n", i, wchar_to_cstring(GuidString));
    }
    printf("Interface description: [%d] %s\n", i, wchar_to_cstring(pInterfaceInfo->strInterfaceDescription));
    printf("Interface state[%d] ", i);
    print_interface_state(pInterfaceInfo->isState);
}
int main(void){
    DWORD lastError;
    DWORD version = 0;
    HANDLE client;

    PWLAN_INTERFACE_INFO_LIST ppInterfaceList = {0};
    PWLAN_INTERFACE_INFO pInterfaceInfo;

    PWLAN_AVAILABLE_NETWORK_LIST ppNetworkList;
    PWLAN_AVAILABLE_NETWORK pNetworkEntry;

    WCHAR GuidString[39] = {0};
    int interfaceRet = 0;

    // Open handle to WLAN
    lastError = WlanOpenHandle(1, 0, &version, &client);
    if(lastError != ERROR_SUCCESS){
        printf("Error wlanh failed code: 0x%lx", lastError);
        return 1;
    }
    
    lastError = WlanEnumInterfaces(client, 0, &ppInterfaceList);
    if(lastError != ERROR_SUCCESS){
        printf("Error getenuminterfaces failed code: 0x%lx", lastError);
        return 1;
    } else {
        printf("Entries: %lu\n", ppInterfaceList->dwNumberOfItems);
        for(unsigned int i = 0; i < (int)ppInterfaceList->dwNumberOfItems; i++){
            // Get Current index of interface info
            pInterfaceInfo = (PWLAN_INTERFACE_INFO) &ppInterfaceList->InterfaceInfo[i];
            print_interface_info(pInterfaceInfo, &interfaceRet, &GuidString, i);

            lastError = WlanGetAvailableNetworkList(client, &pInterfaceInfo->InterfaceGuid, 0, NULL, &ppNetworkList);
            if(lastError != ERROR_SUCCESS){
                printf("Get wlanlist failed with code, %lx\n", lastError);
            }
            else {
                printf("\n\n");
                printf("Available network list for this interface\n");
                printf("Num of entries %lu\n\n", ppNetworkList->dwNumberOfItems);
                for(unsigned int j = 0; j < ppNetworkList->dwNumberOfItems; j++){
                    pNetworkEntry = (WLAN_AVAILABLE_NETWORK *)&ppNetworkList->Network[j];
                    print_network_info(pNetworkEntry, j);
                }
            }
        }
    }
    if(ppNetworkList != 0){
        WlanFreeMemory(ppNetworkList);
    }
    if(ppInterfaceList != 0){
        WlanFreeMemory(ppInterfaceList);
    }
    return 0;
}

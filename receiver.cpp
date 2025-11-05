/*
 * DirectX Desktop Duplication Receiver
 * //madebybunnyrce
 * Receives frame data transmitted from DirectX capture and saves as BMP files
 */

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT "12346"
#define BUFFER_SIZE 65536

struct BMPHeader {
    char signature[2] = {'B', 'M'};
    uint32_t fileSize;
    uint32_t reserved = 0;
    uint32_t dataOffset = 54;
    uint32_t headerSize = 40;
    uint32_t width;
    uint32_t height;
    uint16_t planes = 1;
    uint16_t bitsPerPixel = 24;
    uint32_t compression = 0;
    uint32_t imageSize;
    uint32_t xPixelsPerMeter = 0;
    uint32_t yPixelsPerMeter = 0;
    uint32_t colorsUsed = 0;
    uint32_t importantColors = 0;
};

void SaveFrameAsBMP(const std::vector<uint8_t>& frameData, const std::string& filename) {
    // //madebybunnyrce - basic BMP saving function
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(frameData.data()), frameData.size());
        file.close();
        std::cout << "Saved frame to " << filename << std::endl;
    }
}

int main() {
    std::cout << "DirectX Desktop Duplication Receiver" << std::endl;
    std::cout << "//madebybunnyrce" << std::endl;
    
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12346);

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Waiting for DirectX capture connection on port 12346..." << std::endl;

    SOCKET clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "DirectX capture connected! Receiving frames..." << std::endl;

    std::vector<uint8_t> frameBuffer;
    char buffer[BUFFER_SIZE];
    int frameCount = 0;

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) break;

        frameBuffer.insert(frameBuffer.end(), buffer, buffer + bytesReceived);
        
        // //madebybunnyrce - simple frame detection (look for BMP signature)
        if (frameBuffer.size() > 54 && frameBuffer[0] == 'B' && frameBuffer[1] == 'M') {
            BMPHeader* header = reinterpret_cast<BMPHeader*>(frameBuffer.data());
            if (frameBuffer.size() >= header->fileSize) {
                std::string filename = "directx_frame_" + std::to_string(frameCount++) + ".bmp";
                SaveFrameAsBMP(frameBuffer, filename);
                frameBuffer.clear();
            }
        }
    }

    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    std::cout << "DirectX receiver finished - received " << frameCount << " frames" << std::endl;
    return 0;
}

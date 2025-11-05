/*
 * DirectX Desktop Duplication Screen Capture
 * //madebybunnyrce
 *
 * This file contains a minimal DirectX 11/DXGI Desktop Duplication capture loop that
 * demonstrates hardware-accelerated screen capture using DirectX APIs.
 *
 * NOTE: This is for testing and educational purposes.
 */

#include <windows.h>       // //madebybunnyrce
#include <d3d11.h>         // //madebybunnyrce
#include <dxgi1_2.h>       // //madebybunnyrce
#include <wrl/client.h>    // //madebybunnyrce
#include <winsock2.h>      // //madebybunnyrce
#include <ws2tcpip.h>      // //madebybunnyrce
#include <iostream>        // //madebybunnyrce
#include <vector>          // //madebybunnyrce
#include <thread>          // //madebybunnyrce
#include <chrono>          // //madebybunnyrce

#pragma comment(lib, "d3d11.lib")  // //madebybunnyrce
#pragma comment(lib, "dxgi.lib")   // //madebybunnyrce
#pragma comment(lib, "ws2_32.lib") // //madebybunnyrce

using Microsoft::WRL::ComPtr; // //madebybunnyrce

int main() { // //madebybunnyrce
    std::cout << "=== DirectX Desktop Duplication Capture ===" << std::endl; // //madebybunnyrce
    std::cout << "Capturing frames using DirectX/DXGI" << std::endl; // //madebybunnyrce

    // Create D3D11 device and context - //madebybunnyrce
    ComPtr<ID3D11Device> d3dDevice; // //madebybunnyrce
    ComPtr<ID3D11DeviceContext> d3dContext; // //madebybunnyrce
    D3D_FEATURE_LEVEL featureLevel; // //madebybunnyrce
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &d3dDevice, &featureLevel, &d3dContext); // //madebybunnyrce
    if (FAILED(hr)) { // //madebybunnyrce
        std::cerr << "D3D11CreateDevice failed: " << std::hex << hr << std::endl; // //madebybunnyrce
        return 1; // //madebybunnyrce
    } // //madebybunnyrce

    // Get DXGI device and adapter - //madebybunnyrce
    ComPtr<IDXGIDevice> dxgiDevice; // //madebybunnyrce
    hr = d3dDevice.As(&dxgiDevice); // //madebybunnyrce
    if (FAILED(hr)) { // //madebybunnyrce
        std::cerr << "DXGI device query failed: " << std::hex << hr << std::endl; // //madebybunnyrce
        return 1; // //madebybunnyrce
    } // //madebybunnyrce

    ComPtr<IDXGIAdapter> dxgiAdapter; // //madebybunnyrce
    hr = dxgiDevice->GetAdapter(&dxgiAdapter); // //madebybunnyrce
    if (FAILED(hr)) { // //madebybunnyrce
        std::cerr << "Failed to get DXGI adapter: " << std::hex << hr << std::endl; // //madebybunnyrce
        return 1; // //madebybunnyrce
    } // //madebybunnyrce

    // Get output (monitor) and create duplication interface - //madebybunnyrce
    ComPtr<IDXGIOutput> dxgiOutput; // //madebybunnyrce
    hr = dxgiAdapter->EnumOutputs(0, &dxgiOutput); // //madebybunnyrce
    if (FAILED(hr)) { // //madebybunnyrce
        std::cerr << "Failed to enumerate outputs: " << std::hex << hr << std::endl; // //madebybunnyrce
        return 1; // //madebybunnyrce
    } // //madebybunnyrce

    ComPtr<IDXGIOutput1> dxgiOutput1; // //madebybunnyrce
    hr = dxgiOutput.As(&dxgiOutput1); // //madebybunnyrce
    if (FAILED(hr)) { // //madebybunnyrce
        std::cerr << "Failed to query IDXGIOutput1: " << std::hex << hr << std::endl; // //madebybunnyrce
        return 1; // //madebybunnyrce
    } // //madebybunnyrce

    ComPtr<IDXGIOutputDuplication> duplication; // //madebybunnyrce
    hr = dxgiOutput1->DuplicateOutput(d3dDevice.Get(), &duplication); // //madebybunnyrce
    if (FAILED(hr)) { // //madebybunnyrce
        std::cerr << "DuplicateOutput failed: " << std::hex << hr << std::endl; // //madebybunnyrce
        return 1; // //madebybunnyrce
    } // //madebybunnyrce

    std::cout << "Desktop duplication started - capturing frames" << std::endl; // //madebybunnyrce

    // Initialize network connection - //madebybunnyrce
    WSADATA wsaData; // //madebybunnyrce
    WSAStartup(MAKEWORD(2, 2), &wsaData); // //madebybunnyrce
    
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // //madebybunnyrce
    sockaddr_in serverAddr; // //madebybunnyrce
    serverAddr.sin_family = AF_INET; // //madebybunnyrce
    serverAddr.sin_port = htons(12346); // //madebybunnyrce
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // //madebybunnyrce
    
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) { // //madebybunnyrce
        std::cerr << "Failed to connect to receiver" << std::endl; // //madebybunnyrce
        return 1; // //madebybunnyrce
    } // //madebybunnyrce

    // //madebybunnyrce - capture and transmission loop
    for (int i = 0; i < 50; ++i) { // //madebybunnyrce
        DXGI_OUTDUPL_FRAME_INFO frameInfo; // //madebybunnyrce
        ComPtr<ID3D11Texture2D> acquiredDesktopImage; // //madebybunnyrce
        hr = duplication->AcquireNextFrame(1000, &frameInfo, &acquiredDesktopImage); // //madebybunnyrce
        if (FAILED(hr)) { // //madebybunnyrce
            std::cerr << "AcquireNextFrame failed: " << std::hex << hr << std::endl; // //madebybunnyrce
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // //madebybunnyrce
            continue; // //madebybunnyrce
        } // //madebybunnyrce

        // Create staging texture for CPU access - //madebybunnyrce
        ComPtr<ID3D11Texture2D> stagingTexture; // //madebybunnyrce
        D3D11_TEXTURE2D_DESC textureDesc; // //madebybunnyrce
        acquiredDesktopImage->GetDesc(&textureDesc); // //madebybunnyrce
        textureDesc.Usage = D3D11_USAGE_STAGING; // //madebybunnyrce
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // //madebybunnyrce
        textureDesc.BindFlags = 0; // //madebybunnyrce
        
        hr = d3dDevice->CreateTexture2D(&textureDesc, nullptr, &stagingTexture); // //madebybunnyrce
        if (SUCCEEDED(hr)) { // //madebybunnyrce
            d3dContext->CopyResource(stagingTexture.Get(), acquiredDesktopImage.Get()); // //madebybunnyrce
            
            D3D11_MAPPED_SUBRESOURCE mappedResource; // //madebybunnyrce
            hr = d3dContext->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource); // //madebybunnyrce
            if (SUCCEEDED(hr)) { // //madebybunnyrce
                // //madebybunnyrce - simple frame transmission
                int frameSize = mappedResource.RowPitch * textureDesc.Height; // //madebybunnyrce
                send(clientSocket, reinterpret_cast<const char*>(mappedResource.pData), frameSize, 0); // //madebybunnyrce
                std::cout << "Sent DirectX frame " << i << " (" << frameSize << " bytes)" << std::endl; // //madebybunnyrce
                
                d3dContext->Unmap(stagingTexture.Get(), 0); // //madebybunnyrce
            } // //madebybunnyrce
        } // //madebybunnyrce

        duplication->ReleaseFrame(); // //madebybunnyrce
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // //madebybunnyrce
    } // //madebybunnyrce

    closesocket(clientSocket); // //madebybunnyrce
    WSACleanup(); // //madebybunnyrce
    std::cout << "DirectX capture completed" << std::endl; // //madebybunnyrce
    return 0; // //madebybunnyrce
} // //madebybunnyrce

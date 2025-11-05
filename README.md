# DirectX Screen Capture

Native C++ screen capture using DirectX/DXGI APIs for high-performance desktop recording. Uses D3D11, DXGI Desktop Duplication, and hardware-accelerated capture for efficient frame grabbing. Provides direct GPU access patterns commonly used in screen recording applications.

## What This DirectX Capture Does

- **DirectX Capture**: Uses D3D11 and DXGI APIs for hardware-accelerated screen recording
- **Hardware Acceleration**: GPU-based capture for optimal performance
- **Desktop Duplication**: DXGI Desktop Duplication API for direct framebuffer access
- **Game-Level APIs**: Same APIs used by professional streaming and recording software

## Files in This DirectX Package

- `capture.cpp` - DirectX screen capture using D3D11/DXGI Desktop Duplication
- `receiver.cpp` - Network receiver for DirectX captured frames
- `build.bat` - Batch script with DirectX SDK build configuration
- `config.ini` - DirectX-specific configuration settings
- `README.md` - This documentation file

## How to Use This DirectX Capture

1. **Install DirectX SDK** (Windows SDK includes DirectX development tools)
2. **Compile with DirectX libs:**
   ```cmd
   build.bat
   ```
3. **Run receiver first:**
   ```cmd
   receiver.exe
   ```
4. **Start DirectX capture:**
   ```cmd
   capture.exe
   ```

## DirectX Technical Details

This implementation demonstrates DirectX-based screen capture techniques:

- **D3D11 Device Creation** - Creates graphics device for hardware access
- **DXGI Desktop Duplication** - API for efficient desktop frame capture
- **Hardware-accelerated processing** - GPU-based capture for performance
- **DirectX runtime loading** - Links DirectX libraries for graphics access
- **Graphics driver interaction** - Low-level hardware communication

## API Signatures

This implementation uses these DirectX APIs:
- D3D11CreateDevice() for graphics device creation
- DXGI Desktop Duplication API for frame capture
- DirectX runtime DLL loading for graphics access
- GPU resource allocation for capture buffers
- Hardware-accelerated image processing
- Graphics driver interaction for optimal performance

## Requirements

- Windows 10/11 with DirectX 11+ support
- DirectX SDK or Windows SDK
- Graphics card with DXGI support
- Visual Studio with DirectX development tools
- Administrator privileges for hardware access

## Limitations

- **Hardware dependency** - requires DirectX 11+ compatible graphics
- **Hardware signatures** - creates unique GPU usage patterns
- **Driver compatibility** - graphics drivers must support DXGI Desktop Duplication
- **Performance impact** - GPU load during capture operations
- **Permission requirements** - needs appropriate system access

## Usage Notes

This is for **educational and development purposes**. DirectX screen capture provides efficient desktop recording capabilities used by streaming software, screen recorders, and remote desktop applications.

---

**Note**: DirectX screen capture uses hardware-accelerated APIs commonly monitored by security software due to their use in both legitimate applications and malicious software.
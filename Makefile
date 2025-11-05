# DirectX Desktop Duplication Makefile
# //madebybunnyrce

CC=cl
CFLAGS=/EHsc /W3 /MD
LIBS=d3d11.lib dxgi.lib ws2_32.lib
SRCDIR=.
BINDIR=bin

# Default target
all: $(BINDIR) capture receiver

# Create bin directory
$(BINDIR):
	if not exist $(BINDIR) mkdir $(BINDIR)

# Compile capture executable
capture: $(BINDIR)/capture.exe

$(BINDIR)/capture.exe: $(SRCDIR)/capture.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/capture.cpp /Fe:$(BINDIR)/capture.exe /link $(LIBS)

# Compile receiver executable  
receiver: $(BINDIR)/receiver.exe

$(BINDIR)/receiver.exe: $(SRCDIR)/receiver.cpp
	$(CC) $(CFLAGS) $(SRCDIR)/receiver.cpp /Fe:$(BINDIR)/receiver.exe /link ws2_32.lib

# Clean build artifacts
clean:
	if exist $(BINDIR) rmdir /s /q $(BINDIR)
	if exist *.obj del *.obj
	if exist *.pdb del *.pdb

# Run the capture demo
run: all
	start $(BINDIR)/receiver.exe
	timeout /t 2
	$(BINDIR)/capture.exe

# Install dependencies (placeholder)
install:
	@echo DirectX SDK and Visual Studio required
	@echo Run from Developer Command Prompt

.PHONY: all clean run install capture receiver
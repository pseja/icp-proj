# Setup
## Prerequisites
The following are the minimum tested versions for the required tools:
- **GNU Make**: 5.4 or higher
- **CMake**: 3.16 or higher
- **G++**: 11.5.0 or higher
- **Qt**: 5.9.2 or higher

### Ubuntu
To install the required dependencies on Ubuntu, run the following commands:
```sh
sudo apt install -y make cmake g++        # Basic build tools
sudo apt install -y qt5-qmake qtbase5-dev # Core Qt development tools
sudo apt install -y qtdeclarative5-dev    # Qt Quick (QML) support
sudo apt install -y libqt5scxml5-dev      # Qt SCXML module (for state machines)
```

### Arch linux
To install the required dependencies on Arch Linux, run the following commands:
```sh
sudo pacman -S make cmake g++   # Basic build tools
sudo pacman -S qt5-base qt5-doc # Core Qt development tools & documentation
sudo pacman -S qt5-declarative  # Qt Quick (QML) support
sudo pacman -S qt5-scxml        # Qt SCXML module (for state machines)
```

### Other platforms
For other platforms, refer to the official documentation for installing the required tools:
- [CMake](https://cmake.org/getting-started/)
- [Qt5](https://doc.qt.io/qt-5/get-and-install-qt.html)

## Building the project
1. Clone the repository:
```sh
git clone git@github.com:pseja/icp-proj.git
cd icp-proj
```

2. Run the following command to build and run the project:
```sh
make run
```

## Debugging
If you encounter the following error:
```
qt.qpa.screen: QXcbConnection: Could not connect to display
Could not connect to any X display.
```
This typically occurs when running the application on a remote server without a display. To resolve this, ensure you are connected over `ssh` with the `-X` flag:
```sh
ssh -X xlogin00@merlin.fit.vutbr.cz
```

If the issue persists, you may need to install and configure an X server on your local machine (e.g., [Xming](https://en.wikipedia.org/wiki/Xming)).

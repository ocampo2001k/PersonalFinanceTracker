# Build Instructions for Personal Finance Tracker

## Prerequisites

### 1. Visual Studio Community Edition
- Download and install [Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/)
- During installation, make sure to include:
  - **Desktop development with C++** workload
  - **CMake tools for Visual Studio**
  - **Git for Windows** (if not already installed)

### 2. wxWidgets Setup

#### Option A: Download Pre-built Binaries (Recommended)
1. Download wxWidgets 3.2.x from [wxWidgets Downloads](https://www.wxwidgets.org/downloads/)
2. Extract to `C:\wxWidgets-3.2.x`
3. Set environment variable `WXWIN` to `C:\wxWidgets-3.2.x`

#### Option B: Build from Source
1. Download wxWidgets source code
2. Open `build\msw\wx_vc16.sln` in Visual Studio
3. Build both Debug and Release configurations for x64

### 3. SQLite3 Setup

#### Option A: Using vcpkg (Recommended)
```bash
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install SQLite3
.\vcpkg install sqlite3:x64-windows
```

#### Option B: Manual Installation
1. Download SQLite amalgamation from [SQLite Downloads](https://www.sqlite.org/download.html)
2. Download precompiled binaries for Windows
3. Create a folder `C:\sqlite3` and extract files
4. Add `C:\sqlite3` to your PATH environment variable

## Building the Project

### Method 1: Using CMake (Recommended)

1. **Open Command Prompt or PowerShell**
2. **Navigate to project directory**
   ```bash
   cd C:\Users\%USERNAME%\Documents\PersonalFinanceTracker
   ```

3. **Create build directory**
   ```bash
   mkdir build
   cd build
   ```

4. **Configure with CMake**
   ```bash
   # If using vcpkg
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   
   # If wxWidgets is in standard location
   cmake .. -DwxWidgets_ROOT_DIR=C:/wxWidgets-3.2.x
   ```

5. **Build the project**
   ```bash
   cmake --build . --config Release
   ```

6. **Run the application**
   ```bash
   .\bin\Release\PersonalFinanceTracker.exe
   ```

### Method 2: Using Visual Studio Directly

1. **Open Visual Studio**
2. **File → Open → CMake...** and select `CMakeLists.txt`
3. **Configure CMake settings** if needed:
   - Go to **Project → CMake Settings**
   - Add toolchain file path if using vcpkg
4. **Build → Build All** (Ctrl+Shift+B)
5. **Debug → Start Without Debugging** (Ctrl+F5)

### Method 3: Using Visual Studio Project File

1. **Set up environment variables:**
   - `WXWIN` = `C:\wxWidgets-3.2.x`
   - Add SQLite3 path to system PATH

2. **Open Visual Studio**
3. **File → Open → Project/Solution** and select `PersonalFinanceTracker.vcxproj`
4. **Build → Build Solution** (Ctrl+Shift+B)
5. **Debug → Start Without Debugging** (Ctrl+F5)

## Troubleshooting

### Common Issues

#### 1. wxWidgets not found
- Ensure `WXWIN` environment variable is set correctly
- Restart Visual Studio after setting environment variables
- Check that wxWidgets libraries are built for the correct architecture (x64)

#### 2. SQLite3 linking errors
- If using vcpkg, ensure integration is properly set up
- If manual installation, verify library paths in project settings
- Make sure you're linking against the correct architecture (x64)

#### 3. Build errors related to C++ standard
- Ensure project is set to use C++17 or later
- Check compiler toolset version in project properties

#### 4. Runtime errors
- Ensure all required DLLs are in the same directory as the executable
- For wxWidgets, you might need to copy DLLs from the lib folder
- SQLite3.dll should be accessible if using dynamic linking

### Environment Variables Setup

1. **Open System Properties**
   - Right-click "This PC" → Properties → Advanced system settings
   - Click "Environment Variables"

2. **Add/Edit System Variables:**
   - `WXWIN` = `C:\wxWidgets-3.2.x`
   - Add to `PATH`: `C:\sqlite3` (if using manual SQLite installation)

3. **Restart Visual Studio** after making changes

## Project Structure Verification

After successful build, your project should look like this:

```
PersonalFinanceTracker/
├── Model/
│   ├── Transaction.h
│   └── Transaction.cpp
├── ViewModel/
│   ├── TransactionManager.h
│   └── TransactionManager.cpp
├── View/
│   ├── MainWindow.h
│   └── MainWindow.cpp
├── Database/
│   ├── DatabaseHandler.h
│   └── DatabaseHandler.cpp
├── build/                    # CMake build directory
├── bin/                      # Output executables
├── CMakeLists.txt
├── PersonalFinanceTracker.vcxproj
├── main.cpp
├── README.md
└── .gitignore
```

## Next Steps

1. **Test the application** by adding some sample transactions
2. **Commit your changes** to Git
3. **Push to GitHub** for version control
4. **Consider adding unit tests** for robust development
5. **Implement additional features** like data export, charts, etc.

## Getting Help

- **wxWidgets Documentation**: https://docs.wxwidgets.org/
- **SQLite Documentation**: https://www.sqlite.org/docs.html
- **CMake Documentation**: https://cmake.org/documentation/
- **Visual Studio C++ Documentation**: https://docs.microsoft.com/en-us/cpp/ 
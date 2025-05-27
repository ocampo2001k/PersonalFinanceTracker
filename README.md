# Personal Finance Tracker

A modern C++17 desktop application for tracking personal finances, built using MVVM architecture, wxWidgets GUI, and SQLite database.

## 🚀 Features

- ✅ Add, edit, delete income and expense transactions
- 📊 Categorize transactions (rent, salary, groceries, etc.)
- 📈 Monthly summary and basic analytics
- 💾 Local SQLite database for data persistence
- 🎨 Modern and intuitive user interface
- 🏗️ Clean MVVM architecture

## 🛠️ Tech Stack

- **Language**: C++17
- **GUI Framework**: wxWidgets
- **Database**: SQLite3
- **Architecture**: MVVM (Model-View-ViewModel)
- **IDE**: Visual Studio Community Edition
- **Build System**: CMake (optional) / Visual Studio Projects

## 📁 Project Structure

```
PersonalFinanceTracker/
├── Model/              # Data models (Transaction, etc.)
├── ViewModel/          # Business logic and data management
├── View/              # User interface components
├── Database/          # SQLite database handler
├── Utils/             # Helper functions and utilities
├── resources/         # Icons, images, and other assets
├── CMakeLists.txt     # CMake build configuration
├── README.md          # This file
└── main.cpp          # Application entry point
```

## 🔧 Prerequisites

Before building this project, ensure you have:

1. **Visual Studio Community Edition** (2019 or later)
2. **wxWidgets** library
3. **SQLite3** library
4. **CMake** (optional, for CMake builds)
5. **Git** for version control

## 📦 Installation & Setup

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/PersonalFinanceTracker.git
cd PersonalFinanceTracker
```

### 2. Install Dependencies

#### wxWidgets
1. Download wxWidgets from [official website](https://www.wxwidgets.org/downloads/)
2. Follow the [Visual Studio setup guide](https://docs.wxwidgets.org/3.2/overview_vc.html)

#### SQLite3
Option A: Using vcpkg (recommended)
```bash
vcpkg install sqlite3:x64-windows
```

Option B: Manual installation
1. Download SQLite amalgamation from [SQLite website](https://www.sqlite.org/download.html)
2. Add to your project

### 3. Build the Project

#### Using CMake
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

#### Using Visual Studio
1. Open Visual Studio
2. File → Open → CMake... → Select CMakeLists.txt
3. Build → Build All

## 🎯 Usage

1. Launch the application
2. Add your first transaction using the "Add Transaction" button
3. Categorize your income and expenses
4. View monthly summaries and analytics
5. Edit or delete transactions as needed

## 🏗️ Architecture Overview

This project follows the **MVVM (Model-View-ViewModel)** pattern:

- **Model**: Defines data structures (`Transaction`)
- **View**: Handles UI presentation (`MainWindow`)
- **ViewModel**: Manages business logic (`TransactionManager`)
- **Database**: Handles data persistence (`DatabaseHandler`)

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- wxWidgets team for the excellent GUI framework
- SQLite team for the lightweight database engine
- C++ community for best practices and patterns

## 📞 Contact

Your Name - your.email@example.com

Project Link: [https://github.com/yourusername/PersonalFinanceTracker](https://github.com/yourusername/PersonalFinanceTracker) 
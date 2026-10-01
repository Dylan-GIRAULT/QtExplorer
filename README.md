# QtExplorer

QtExplorer is a desktop file explorer developed in C++ using Qt 6 and Qt Widgets. The project aims to put into practice the key mechanisms of a Qt application: interface design with Qt Designer, signals and slots, custom widgets, Qt resources, and file system access.

## Features

- Folder navigation from a local path;
- Display of files and folders with their name, size, and modification date;
- Backward and forward navigation through folder history;
- Opening a file with the system's associated application;
- Creation of files and folders via the context menu;
- Renaming and deletion of files or folders;
- Opening the current folder in the system file explorer;
- Resizable interface based on Qt layouts.

## Technical Overview

The project is organized around two main classes:

- `QtExplorer` : main window, navigation, filtering, history and operations on the files;
- `FileWidget` : custom widget representing a file or a folder.

The Qt elements used include, in particular:

- `QMainWindow`, `QScrollArea`, `QLineEdit`, `QToolButton`, and layouts;
- the meta-object system with `Q_OBJECT`, signals, and slots;
- `QDir`, `QFileInfo`, `QFile`, and `QDesktopServices`;
- context menus with `QMenu` and `QAction`;
- Qt Designer via `qtexplorer.ui`;
- the Qt resource system via `resource/icon.qrc`.

## Dependencies

- Qt 6 (the project was developed and verified with Qt 6.11.2);
- a C++17 compiler, for example, the 64-bit MinGW provided with Qt;
- Qt Creator or the `qmake` and `make` tools.

## Compiling with Qt Creator

1. Open `QtExplorer.pro` in Qt Creator.
2. Select a Qt 6 kit compatible with the installed compiler.
3. Configure the project, then start the build.
4. Run the generated application.

## Command-line compilation

From a terminal configured for Qt:

```bash
qmake QtExplorer.pro
mingw32-make
```

The compilation command depends on the compiler used. With MSVC, replace `mingw32-make` with the corresponding build tool.

## Usage

Upon startup, QtExplorer opens the user's home folder. The current path can be entered directly into the top bar. Clicking a folder opens it; clicking a file opens it using the system's default application.

The filter field narrows down the displayed list by searching for the entered text within item names. The context menu allows you to create, rename, delete, or open the displayed items.

## Project Structure

```text
QtExplorer/
├── filewidget.cpp       # Implementation of the file widget
├── filewidget.h
├── main.cpp             # Application entry point
├── qtexplorer.cpp       # Main window logic
├── qtexplorer.h
├── qtexplorer.ui        # Interface created with Qt Designer
├── QtExplorer.pro       # qmake configuration
└── resource/
	└── icon.qrc         # Embedded resources
```

## Current Limitations

The project is primarily a demonstration of Qt Widgets. It does not yet include automated test suites, user preference persistence, translation with Qt Linguist, or a model/view based on `QFileSystemModel`.

These points constitute natural evolution paths to improve the robustness, performance, and functional coverage of the project.

## Evolution Paths

- Replace the list of pre-allocated widgets with `QFileSystemModel` and `QTreeView` or `QListView`;
- Add Qt Test tests for filtering, history, and size formatting;
- Use `QSettings` to save the window size and the last opened folder;
- Add keyboard shortcuts and more detailed access error handling;
- Migrate the build configuration to CMake;

## Project Purpose

This project serves as a practical demonstration of creating a desktop application using Qt 6. It focuses on inter-component communication, interface composition with Qt Widgets, and interaction with the local file system.

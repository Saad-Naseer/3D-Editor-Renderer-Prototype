# 3D Editor Renderer Prototype

A working prototype demonstrating the core features of a 3D road modeling application, including the user interface, user management (authentication & storage), and the 3D road editor.

## Features

This prototype implements the following core functionalities:

### 1. User Management & Storage
- Secure signup and login for new and existing users.

### 2. Credit System
- A functional system for tracking credits per user, which are required for exporting a road model.

### 3. Interactive 3D Editor
- A user interface for creating road geometry.

### 4. Real-time Preview
- A low-fidelity 3D renderer that provides instant visual feedback to the user.

### 5. Secure API
- All communication between the Qt frontend and the backend is handled through a secure REST API.
- Events are managed using Qt's signals and slots.

### 6. Asynchronous Export
- The system initiates an export process that runs in the background, preventing the application from freezing during long-running tasks.

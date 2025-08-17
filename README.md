# 3D Editor Renderer Prototype
 A working prototype demonstrating the core features of a 3D road modeling application, including the user interface, user management (authentication & storage), and the 3D road editor.
# Features
This prototype implements the following core functionalities:

User Management: Secure signup and login for new and existing users.

Credit System: A functional system for tracking credits per user, which are a prerequisite for exporting a road model.

Interactive 3D Editor: A user interface for creating and modifying road geometry.

Real-time Preview: A low-fidelity 3D renderer that provides instant visual feedback to the user.

Secure API: All communication between the frontend and backend is handled through a secure REST API.

Asynchronous Export: The system initiates an export process that is handled in the background, preventing the application from freezing during long-running tasks.
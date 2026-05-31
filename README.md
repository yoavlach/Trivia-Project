# 🧠 Trivia Multi-Client System (v3.0.0)

## 📖 Overview

The **Trivia Multi-Client System** is a robust, multi-participant trivia game built on a custom client-server architecture. The system utilizes low-level socket programming for real-time network communication and features a multi-threaded C++ backend capable of efficiently handling concurrent client requests. The frontend is a rich Graphical User Interface (GUI) developed in C#.

Currently in **Version 3.0.0**, the project supports full user authentication, real-time multiplayer room management, and dynamic statistics tracking.

## ✨ Key Features (Up to v3.0.0)

* **Multi-Threaded C++ Server:** Engineered to support multiple concurrent users without bottlenecking, allocating independent threads for active client socket sessions.
* **C# Graphical User Interface (GUI):** A dynamic .NET client application featuring real-time auto-refresh mechanisms (via background threads) to update active rooms and player lists every 3 seconds.
* **Custom Communication Protocol:** Implements a strict binary-to-JSON protocol. Every message contains a 1-byte request code, a 4-byte message length header, and a serialized JSON payload.
* **Advanced State Machine Architecture:** Request handling is managed via a `RequestHandlerFactory`, smoothly transitioning users between states like `LoginRequestHandler`, `MenuRequestHandler`, `RoomAdminRequestHandler`, and `RoomMemberRequestHandler`.
* **Secure Data Persistence:** Integrates an **SQLite** database to securely store user credentials, trivia questions, and player statistics (average answer time, correct answers, total games).

## 🛠️ Technologies & Tools

* **Backend Server:** C++ (Sockets, Multi-threading, OOP, State Machine)
* **Frontend Client:** C# / .NET (WPF / WinForms)
* **Database:** SQLite3
* **Data Formatting:** JSON (Serialization/Deserialization)
* **Version Control:** Git & GitLab (Continuous feature branching, merging, and MR reviews)

## 🏗️ System Architecture Highlights

* **Serialization & Deserialization:** The `JsonResponsePacketSerializer` and `JsonRequestPacketDeserializer` classes strictly parse incoming raw byte streams from the socket into workable C++ structs, and vice versa for outgoing server responses.
* **Manager Classes:** The backend utilizes centralized managers (`LoginManager`, `RoomManager`, `StatisticsManager`) to interface securely with the database and maintain active memory states.
* **Admin vs. Member Logic:** Room interactions are strictly validated. Only Room Admins can close rooms or initiate a game, immediately broadcasting state changes to Room Members.

## 🚀 Getting Started

### Prerequisites

To compile and run this project, you will need:

* A C++ compiler (GCC / MinGW / MSVC)
* SQLite3 installed and configured
* Visual Studio (for running the C# .NET Client)

### Installation & Setup

1. **Clone the repository:**
```bash
git clone https://github.com/yoavlach/Trivia-Project
cd Trivia_Project

```


2. **Compile and Run the Server (C++):**
* Build the project using your preferred IDE or Makefile. Ensure the `sqlite3` library is linked correctly.
* Run the compiled Server executable. It will listen on the designated port.


3. **Launch the Client (C#):**
* Create a project in Visual Studio.
* Run the application to connect to the server.



## 👥 Contributors

* **Yoav Lach** - Core Developer (Backend Architecture, Sockets, State Machine, DB Integration)
* **Yonatan** - Core Developer (Frontend GUI, Protocol Serialization, Room Management Logic)

## Introduction
This is a simple HTTP server implemented in C, designed to serve static web content to clients. An HTTP server is a fundamental component of web technology, responsible for handling incoming HTTP requests and serving the requested resources to clients, such as web browsers. This server is intended for educational purposes and provides a basic understanding of how web servers work at a low level.

## Usage
To use this HTTP server, follow these steps:

Compile the Code: Compile the code using a C compiler (e.g., gcc):
Run the Server: Start the server by running the compiled binary:
Access Served Files: The server will start listening on port 8080. You can access the served files by opening a web browser and navigating to URLs like http://localhost:8080/index.html

## Safety Concerns

Security: The server lacks essential security features like input validation, security mechanisms, and access controls. It should not be used in production environments without significant improvements.

Memory Management: Memory management is not safeguarded in this code, potentially leading to memory leaks or crashes upon memory allocation failures.

Concurrency: This server is single-threaded and can handle only one connection at a time. For real-world scenarios, you should consider implementing multi-threading or an event-driven approach to manage multiple connections concurrently.


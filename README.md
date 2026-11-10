# AI Agent Framework in C++

## 1. Introduction

AI Agent Framework is a C++17 project developed as part of an Object-Oriented Programming (OOP) course.

The project aims to build a simplified AI Agent framework from scratch, inspired by existing frameworks such as LangChain and OpenClaw.

It connects to locally hosted Large Language Models (LLMs) through the Ollama API and provides an extensible tool system that allows an AI Agent to interact with external functionalities.

Unlike a traditional chatbot that only generates text, an AI Agent can identify when a tool is needed, request its execution, and use the returned results to generate a response.

The main objective is to understand the fundamental architecture and implementation of an AI Agent using C++ and Object-Oriented Programming principles.

## 2. Features

The project is being developed incrementally. The current implementation includes the following components:

- **LLM Client:** Communicate with locally hosted language models through the Ollama API using HTTP requests.
- **Tool Interface:** Define a common interface for different tools, allowing them to be implemented and executed through a unified structure.
- **Calculator Tool:** Evaluate mathematical expressions.
- **Tool Registry:** Register and retrieve available tools by their names.
- **JSON Processing:** Parse API requests and responses using nlohmann/json.
- **Error Handling:** Handle API connection errors, invalid JSON responses, and tool execution errors.

## In Progress

The following features and improvements are currently under development:

- **Agent Loop Improvements**
  - Improve error handling for invalid LLM responses and failed tool executions.
  - Handle unknown tools gracefully without prematurely terminating the agent loop.
  - Improve conversation history management for tool calls and tool responses.
  - Ensure reliable execution of multiple tool calls within a single interaction.

- **Loop Detection**
  - Refine loop detection logic to identify repetitive tool calls.
  - Implement and validate both generic repeat and ping-pong loop detection.
  - Improve loop detection thresholds and termination handling.

- **Additional Tools**
  - Implement additional tools to expand the agent's capabilities.
  - Ensure at least five functional tools are available through the Tool Registry.

- **Skill System**
  - Implement `SkillLoader` to load Markdown skill files from the `skills/` directory.
  - Add keyword-based skill selection.
  - Inject relevant skill instructions into the system prompt before each agent run.
  - Develop at least three custom skill files.

- **Harness & Evaluation**
  - Implement `HarnessRunner` to manage agent execution and evaluation.
  - Add trajectory recording for agent execution steps.
  - Implement `KeywordEvaluator` and `FunctionalEvaluator`.
  - Develop benchmark tasks and support batch evaluation with JSON output.

- **Testing & Reliability**
  - Expand unit and integration tests.
  - Improve HTTP error handling, timeout configuration, and JSON response validation in `OllamaClient`.
  - Test agent behavior across normal execution, tool failures, invalid responses, and loop detection scenarios.
  
## 3. Technologies

| Technology | Purpose |
|---|---|
| C++17 | Main programming language |
| Ollama | Local LLM runtime |
| llama3.2 | Language model |
| libcurl | HTTP communication with Ollama |
| nlohmann/json | JSON parsing and manipulation |
| CMake | Build system |
| vcpkg | C++ dependency management |
| Visual Studio Build Tools | MSVC compiler |
| Visual Studio Code | Development environment |

## 4. Project Structure

The project is organized into separate modules to improve maintainability and extensibility.

```text
Agent_25120015/
│
├── src/
│   ├── client/
│   │   ├── ollama_client.h
│   │   └── ollama_client.cpp
│   │
│   ├── tools/
│   │   ├── tool.h
│   │   ├── calc_tool.h
│   │   ├── calc_tool.cpp
│   │   ├── tool_registry.h
│   │   └── tool_registry.cpp
│   │
│   ├── core/
│   │   └── (Agent implementation)
│   │
│   └── main.cpp
│
├── tests/
│
├── docs/
│
├── .vscode/
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

*Note: This is a logical representation of the project architecture. Update the tree to match the actual file locations in the repository.*

### Main Components

- **OllamaClient:** Responsible for sending requests to the Ollama API and receiving model responses.
- **Tool:** Defines the common interface that all tools must implement.
- **Calculator:** Implements mathematical expression evaluation.
- **ToolRegistry:** Manages available tools and provides access to them.
- **Agent:** Coordinates communication between the LLM and available tools through the Agent Loop.

## 5. Requirements

Before building the project, make sure the following software is installed:

- Windows 10 or later.
- Visual Studio 2022 Build Tools with the C++ workload.
- CMake 3.20 or later.
- Git.
- vcpkg.
- Ollama.
- A compatible LLM model.

## 6. Installation and Setup

### Step 1: Clone the repository

```bash
git clone <YOUR_REPOSITORY_URL>
cd Agent_25120015
```

### Step 2: Install dependencies

This project uses vcpkg to manage C++ dependencies.

Install the required libraries:

```powershell
vcpkg install curl:x64-windows
vcpkg install nlohmann-json:x64-windows
```

Make sure that vcpkg is installed and available at:

```text
C:/vcpkg
```

If vcpkg is installed in another location, adjust the paths in the following commands accordingly.

### Step 3: Install and configure Ollama

Download and install Ollama from:

https://ollama.com/

Download the model:

```bash
ollama pull llama3.2
```

Start the Ollama service:

```bash
ollama serve
```

The default API endpoint is:

```text
http://localhost:11434
```

Make sure Ollama is running before launching the application.

### Step 4: Configure the project using CMake

Open a terminal in the project root directory and run:

```powershell
cmake -S . -B build-vcpkg `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows
```

This command configures the project using CMake and tells it to locate dependencies through vcpkg.

### Step 5: Build the project

Compile the project in Debug mode:

```powershell
cmake --build build-vcpkg --config Debug
```

After a successful build, the executable will be generated at:

```text
build-vcpkg/Debug/OllamaTest.exe
```

### Step 6: Run the application

Run the executable:

```powershell
.\build-vcpkg\Debug\OllamaTest.exe
```

## 7. Usage

After launching the application, users can interact with the locally hosted LLM through the terminal.

The framework is designed to allow the Agent to use registered tools when necessary.

### Example: Calculator Tool

User input:

```text
Calculate 123 * 456
```

Expected tool call from the LLM:

```json
{
    "name": "Calculator",
    "arguments": {
        "expression": "123*456"
    }
}
```
Expected calculation result:

```text
56088
```

*Note: The LLM tool-call request and Calculator implementation are available. The complete Agent Loop, which executes the requested tool and sends its result back to the LLM for a final response, is still under development.*

## 8. Object-Oriented Programming Concepts

This project applies the following OOP principles:

### Encapsulation

Each component encapsulates its own data and operations.

For example, `OllamaClient` handles API communication, while `Calculator` handles mathematical expression evaluation.

### Inheritance

Different tools inherit from a common `Tool` interface, allowing them to share a consistent structure.

### Polymorphism

Different tool implementations can be accessed and executed through a common interface.

For example, the Agent can work with a `Tool` pointer without needing to know the concrete implementation of each tool.

### Abstraction

The framework separates the main responsibilities into independent components:

- LLM communication.
- Tool definitions and implementations.
- Tool management.
- Agent execution and decision-making.

This separation makes the framework easier to maintain and extend.

## 9. Future Improvements

The following features are planned for future development:

- Complete the Agent Loop and tool execution workflow.
- Support multiple tool calls in a single interaction.
- Implement additional tools such as web search and file operations.
- Improve conversation history management.
- Support multimodal inputs, including images.
- Improve error handling and logging.
- Add unit tests and integration tests.
- Improve the Agent's ability to handle complex tasks.

## 10. Author

**Name:** Nguyễn Thành An

**Student ID:** 25120015

## 11. License

This project was developed for educational purposes as part of an Object-Oriented Programming course.
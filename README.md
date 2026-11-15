# AI Agent Framework in C++

## 1. Introduction

AI Agent Framework is a C++17 project developed as part of an Object-Oriented Programming (OOP) course.

The project aims to build a simplified AI Agent framework from scratch, inspired by frameworks such as LangChain and OpenClaw.

It connects to locally hosted Large Language Models (LLMs) through the Ollama API. The framework also provides a tool system that allows an AI Agent to request and execute external functions.

Unlike a traditional chatbot that only generates text, an AI Agent can decide when a tool is needed, request its execution, receive the result, and use that result to continue the conversation.

The main objective is to understand the architecture and implementation of an AI Agent using C++ and Object-Oriented Programming principles.

## 2. Features

### Implemented

- **LLM Client:** Communicates with locally hosted language models through the Ollama API.
- **Tool Interface:** Defines a common interface for tools.
- **Tool Registry:** Registers tools and retrieves them by name.
- **Calculator Tool:** Evaluates arithmetic expressions.
- **Agent Loop:** Coordinates communication between the LLM and registered tools.
- **Multiple Tool Calls:** Processes multiple tool calls returned in one LLM response.
- **Loop Detection:** Detects repeated calls with the same tool name and arguments.
- **System Prompt:** Loads the assistant's base instructions from an external text file.
- **Skill Loader:** Loads Markdown-based Skill files and provides keyword-based Skill selection.
- **Skill Injection:** Adds the selected Skill's instructions to the system prompt before sending a request to the LLM.

### In Progress

- **Automatic Error Recovery**
  - Detect tool execution errors.
  - Activate the Error Recovery Skill automatically.
  - Allow the Agent to analyze errors and decide whether recovery is possible.

- **Tool System**
  - Implement additional tools.
  - Improve argument validation and tool error reporting.

- **Agent Reliability**
  - Improve loop detection.
  - Improve JSON response validation.
  - Handle failures and edge cases in the Agent Loop.

- **Harness and Evaluation**
  - Complete `HarnessRunner`.
  - Implement trajectory recording and evaluators.
  - Run benchmark tasks and produce evaluation results.

- **Testing**
  - Add unit and integration tests.
  - Test normal execution, tool failures, invalid responses, and recovery behavior.

## 3. Technologies

| Technology | Purpose |
|---|---|
| C++17 | Main programming language |
| Ollama | Local LLM runtime |
| Qwen | Language model |
| libcurl | HTTP communication |
| nlohmann/json | JSON processing |
| CMake | Build system |
| vcpkg | Windows dependency management |
| Visual Studio Build Tools | MSVC compiler |

## 4. Project Structure

```text
Agent_25120015/
│
├── src/
│   ├── agent/
│   │   ├── agent_loop.h
│   │   ├── agent_loop.cpp
│   │   ├── loop_detector.h
│   │   ├── loop_detector.cpp
│   │   ├── skill_loader.h
│   │   └── skill_loader.cpp
│   │
│   ├── client/
│   │   ├── llm_client.h
│   │   ├── ollama_client.h
│   │   └── ollama_client.cpp
│   │
│   ├── tools/
│   │   ├── tool.h
│   │   ├── tool_registry.h
│   │   ├── tool_registry.cpp
│   │   ├── calc_tool.h
│   │   └── calc_tool.cpp
│   │
│   ├── harness/
│   │   ├── harness_runner.h
│   │   ├── harness_runner.cpp
│   │   ├── evaluator.h
│   │   ├── placeholder_evaluator.cpp
│   │   ├── trajectory.h
│   │   └── trajectory.cpp
│   │
│   └── main.cpp
│
├── skills/
│   ├── error_recovery.md
│   ├── task_planner.md
│   └── placeholder.md
│
├── benchmark/
│   ├── run_eval.cpp
│   └── tasks.json
│
├── system_prompt.txt
├── skill_format_instructions.txt
├── CMakeLists.txt
├── README.md
└── .gitignore
```

### Main Components

- **OllamaClient:** Sends chat requests to Ollama and receives model responses.
- **Tool:** Abstract interface implemented by individual tools.
- **ToolRegistry:** Owns registered tools and provides tool definitions to the LLM.
- **AgentLoop:** Manages the interaction cycle between the user, LLM, and tools.
- **LoopDetector:** Detects repeated tool calls.
- **SkillLoader:** Loads Skill files and selects a Skill based on the user's request.
- **Harness:** Intended to run benchmark tasks and evaluate Agent behavior.

## 5. Requirements

- Windows 10 or later, or a compatible Linux environment.
- C++17 compiler.
- CMake 3.20 or later.
- Git.
- Ollama.
- A compatible Ollama model.
- libcurl.
- nlohmann/json.

## 6. Installation and Setup

### Step 1: Clone the repository

```bash
git clone <YOUR_REPOSITORY_URL>
cd Agent_25120015
```

### Step 2: Install dependencies

#### Windows

Install Visual Studio 2022 Build Tools with the C++ workload, CMake, and vcpkg.

Install the required libraries:

```powershell
vcpkg install curl:x64-windows
vcpkg install nlohmann-json:x64-windows
```

The current Windows CMake configuration expects vcpkg at:

```text
C:/vcpkg
```

If vcpkg is installed elsewhere, update the toolchain path in `CMakeLists.txt`.

#### Linux / Kaggle

Install the required packages:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libcurl4-openssl-dev nlohmann-json3-dev
```

### Step 3: Install and configure Ollama

Install Ollama from:

https://ollama.com/

The default model is selected in `OllamaClient` according to the operating system:

- Windows: `qwen2.5:3b`
- Linux: `qwen3.8:27b`

Pull the model configured for your environment. For example:

```bash
ollama pull qwen2.5:3b
```

Or, on Linux:

```bash
ollama pull qwen3.8:27b
```

Start the Ollama service if it is not already running:

```bash
ollama serve
```

The default API endpoint is:

```text
http://localhost:11434
```

Make sure the Ollama service is available before running the application.

### Step 4: Configure and build

#### Windows

```powershell
cmake -S . -B build-vcpkg -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build-vcpkg --config Debug
```

The executable is generated at:

```text
build-vcpkg/Debug/OllamaTest.exe
```

#### Linux / Kaggle

```bash
cmake -S . -B build
cmake --build build --parallel 2
```

The executable is generated at:

```text
build/OllamaTest
```

### Step 5: Run the application

Run the executable from the project root directory. This is important because the application loads `system_prompt.txt` and the `skills/` directory using relative paths.

Windows:

```powershell
.\build-vcpkg\Debug\OllamaTest.exe
```

Linux:

```bash
./build/OllamaTest
```

## 7. Usage

After launching the application, select an option from the terminal menu:

```text
1. Trò chuyện với AI
2. Thoát
```

Enter a message when prompted. The Agent sends the request to the LLM and executes registered tools when requested.

### Example: Calculator

User input:

```text
Calculate 123 * 456
```

The LLM may request the Calculator tool with arguments similar to:

```json
{
    "expression": "123*456"
}
```

The Agent executes the tool and sends its result back to the LLM so it can generate a final response.

### Example: Skill Selection

The project stores Skills as Markdown files in the `skills/` directory.

Each Skill contains metadata and instructions:

```markdown
---
name: Task Planner
keywords: plan, planning, task, organize
---

Instructions
- Understand the user's goal.
- Break complex tasks into manageable steps.
- Use tools only when necessary.
```

The Skill Loader uses the metadata to select a relevant Skill. The selected instructions are added to the system prompt before the LLM request.

Automatic activation of Error Recovery after a tool execution error is currently under development.

## 8. Object-Oriented Programming Concepts

### Encapsulation

Each component manages its own data and operations. For example, `OllamaClient` handles API communication, while `Calculator` handles arithmetic evaluation.

### Inheritance

Concrete tools inherit from the abstract `Tool` interface.

### Polymorphism

The `ToolRegistry` stores tools through `unique_ptr<Tool>`. The Agent can execute different tools through the common interface without depending on their concrete implementations.

### Abstraction

The project separates responsibilities into independent components:

- LLM communication.
- Tool definitions and execution.
- Tool registration.
- Agent orchestration.
- Skill loading and selection.
- Evaluation infrastructure.

## 9. Current Limitations and Future Improvements

- Add more functional tools.
- Implement automatic Error Recovery activation.
- Improve Skill selection and matching.
- Improve loop detection and Agent step management.
- Strengthen HTTP and JSON error handling.
- Complete Harness and Evaluation.
- Add unit and integration tests.
- Improve conversation history management.
- Explore additional capabilities such as file operations and web search.

## 10. License

This project was developed for educational purposes as part of an Object-Oriented Programming course.
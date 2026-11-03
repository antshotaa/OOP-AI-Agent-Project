# AI Agent Framework in C++

## 1. Introduction

This project is an AI Agent Framework developed in C++17 as an Object-Oriented Programming (OOP) course project.

The framework connects to a locally hosted Large Language Model (LLM) through the Ollama API and provides an extensible tool system that allows the AI Agent to interact with external functionalities.

The main goal is to understand and implement the fundamental architecture of an AI Agent framework from scratch using C++.

## 2. Features

- **LLM Client:** Communicate with locally hosted language models through the Ollama API.
- **Tool System:** Provide an extensible architecture for defining and executing tools.
- **Calculator Tool:** Perform mathematical calculations through the Agent.
- **Tool Registry:** Manage and retrieve registered tools.
- **Error Handling:** Handle API connection errors, invalid responses, and tool execution errors.

*(Update this list according to the features actually implemented.)*

## 3. Technologies

| Technology | Purpose |
|---|---|
| C++17 | Main programming language |
| Ollama | Local LLM runtime |
| libcurl | HTTP communication |
| nlohmann/json | JSON parsing and manipulation |
| Visual Studio Code | Development environment |

## 4. Project Structure

```text
Agent/
├── src/
│   ├── client/       # Ollama API communication
│   ├── tools/        # Tool implementations
│   ├── core/         # Agent core logic
│   └── main.cpp      # Program entry point
├── include/          # Header files
├── tests/            # Unit tests
├── docs/             # Project documentation
├── build/            # Build output
└── README.md
```

*(Adjust the directory structure to match the actual project.)*

## 5. Requirements

Before running the project, make sure you have:

- A C++17 compatible compiler.
- [Ollama](https://ollama.com/) installed.
- libcurl.
- nlohmann/json.
- At least one compatible LLM model downloaded.

## 6. Installation and Setup

### Step 1: Install dependencies

Install Ollama, libcurl, and nlohmann/json.

### Step 2: Download an LLM model

For example:

```bash
ollama pull llama3.2
```

### Step 3: Start Ollama

```bash
ollama serve
```

Make sure the Ollama API is accessible at:

```text
http://localhost:11434
```

### Step 4: Build the project

Compile the source files using a C++17 compatible compiler and link the required dependencies.

### Step 5: Run

Execute the generated program.

## 7. Usage

Launch the application and follow the instructions displayed in the terminal.

The user can interact with the AI Agent through natural language.

When a supported tool is available, the Agent can use it to perform the corresponding operation.

Example:

```text
User: What is 25 * 4 + 10?

Agent: 110
```

## 8. OOP Concepts

This project applies several Object-Oriented Programming concepts:

- **Encapsulation:** Organize data and related operations within classes.
- **Inheritance:** Define specialized tools based on a common Tool interface.
- **Polymorphism:** Allow different tools to be executed through a common interface.
- **Abstraction:** Separate the Agent, LLM Client, and Tool implementation layers.

## 9. Future Improvements

- Support more tools.
- Improve tool selection and execution.
- Support multimodal inputs.
- Improve error handling and logging.
- Add more comprehensive unit tests.

## 10. Authors

- Student ID: [Your Student ID]
- Name: [Your Name]

## 11. License

This project was developed for educational purposes as part of an OOP course.
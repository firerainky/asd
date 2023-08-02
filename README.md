# fhe_lib

A foundational computation layer of the full homomorphic encryption algorithm library, developed by Zhejiang Lab.

fhe_lib can be built and executed using gcc/clang compilers, but it has not been tested with the MSVC compiler yet.

## Build

```bash
cmake -S . -B build
```

There are 3 executable targets in fhe_lib at the current stage:

- **zj_fhe_lib**: main app, just a usage case for using MatchFunctions lib.
- **unittests**: unit tests for MathFunctions lib.
- **fhe_benchmark**: benchmarks for MathFunctions, not really in use now.

## Development tools

### Sanitizer

Sanitizer is used for detecting memory error during runtime.

You can turn on/off sanitizer in `./CMakeLists.txt`:

```CMake
option(ENABLE_SANITIZE_ADDR "Enable address sanitize." ON)
option(ENABLE_SANITIZE_UNDEF "Enable undefined sanitize." ON)
option(ENABLE_SANITIZE_LEAK "Enable leak sanitize (Gcc/Clang only)." ON)
option(ENABLE_SANITIZE_THREAD "Enable thread sanitize (Gcc/Clang only)." OFF)
```

### Code format

fhe_lib uses clang-format for C/CPP code formatting and cmake-format for CMake file formatting.

You can format your code in CMake build target "run-clang-format" / "run-cmake-format".

#### Install clang-format / cmake-format

Ubuntu: `sudo apt-get install -y clang-format cmake-format`

macOS:

```bash
brew install clang-format

# cmake-format 通过 pip 来安装，没有装 pip 的话要先装 pip: 
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
python3 get-pip.py

# 通过 pip 来安装 cmake-format
pip install cmakelang
```

If you are using vscode, you can install Xaver's Clang-Format extension and add the following config in settings.json:

```json
"[cpp]": {
  "editor.defaultFormatter": "xaver.clang-format",
  "editor.formatOnSave": true,
},
"clang-format.language.cpp.enable": true,
```

### Static analysis

fhe_lib uses the linting tool clang-tidy for static analysis.

You can run clang-tidy with CMake build target: `MathFunctions_clangtidy`

#### Install clang-tidy

Ubuntu: `sudo apt-get install -y clang-tidy`

macOS:

```bash
brew install llvm
sudo ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy"
```

## log
宏变量定义在 util/logger.h中
#define CONSOLE_LOG  控制是否在控制台打印日志

#define LOG_FILE_NAME "zj_fhe_log.log" 日志文件名称

#define LOG_FILE_PATH "logs/" 日志文件保存路径

#define CONSOLE_LOG_LEVEL spdlog::level::debug  控制台日志显示等级
#define FILE_LOG_LEVEL spdlog::level::info 文件日志显示等级

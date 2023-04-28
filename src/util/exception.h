//
// Created by 赵启明 on 2023/4/27.
//

#ifndef ZJ_FHE_LIB_EXCEPTION_H
#define ZJ_FHE_LIB_EXCEPTION_H
#include <exception>
#include <stdexcept>
#include <string>

namespace zhejiangfhe {
    class ZJException : public std::runtime_error {
        std::string fileName;
        int lineNum;
        std::string message;

    public:
        ZJException(const std::string& file, int line, const std::string& what)
            : std::runtime_error(what), fileName(file), lineNum(line) {
            message = fileName + ":" + std::to_string(lineNum) + " " + what;
        }

        const char* what() const throw() {
            return message.c_str();
        }

        const std::string& GetFileName() const {
            return fileName;
        }
        int GetLineNum() const {
            return lineNum;
        }
    };



    class ConfigException : public ZJException {
    public:
        ConfigException(const std::string& file, int line, const std::string& what) : ZJException(file, line, what) {}
    };

    class MathException : public ZJException {
    public:
        MathException(const std::string& file, int line, const std::string& what) : ZJException(file, line, what) {}
    };

    class NotImplementedException : public ZJException {
    public:
        NotImplementedException(const std::string& file, int line, const std::string& what)
            : ZJException(file, line, what) {}
    };

    class NotAvailableError : public ZJException {
    public:
        NotAvailableError(const std::string& file, int line, const std::string& what) : ZJException(file, line, what) {}
    };

    class TypeException : public ZJException {
    public:
        TypeException(const std::string& file, int line, const std::string& what) : ZJException(file, line, what) {}
    };


    class SerializeError : public ZJException {
    public:
        SerializeError(const std::string& file, int line, const std::string& what) : ZJException(file, line, what) {}
    };


    class DeserializeError : public ZJException {
    public:
        DeserializeError(const std::string& file, int line, const std::string& what) : ZJException(file, line, what) {}
    };

#define ZJFHE_THROW(exc, expr) throw exc(__FILE__, __LINE__, (expr))
}


#endif//ZJ_FHE_LIB_EXCEPTION_H

#pragma once
#include <string>

class ILogger
{
public:
	virtual void Write(const std::string& message) = 0;
	virtual ~ILogger() {};
};

class Logger : public ILogger
{
public:
	void Write(const std::string& message) override {
		m_message = message;
		std::cout << m_message << std::endl;
	};
	~Logger() {};
private:
	std::string m_message;
};
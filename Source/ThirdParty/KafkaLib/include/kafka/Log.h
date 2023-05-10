#pragma once

#include "Project.h"

#include "Utility.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>

#define ENABLE_UE_LOGS 1

namespace KAFKA_API {

	DEFINE_LOG_CATEGORY_STATIC(LogKafka, Log, All);

	struct Log
	{
		enum Level
		{
			Emerg = 0,
			Alert = 1,
			Crit = 2,
			Err = 3,
			Warning = 4,
			Notice = 5,
			Info = 6,
			Debug = 7
		};

		static const std::string& levelString(std::size_t level)
		{
			static const std::vector<std::string> levelNames = { "EMERG", "ALERT", "CRIT", "ERR", "WARNING", "NOTICE", "INFO", "DEBUG", "INVALID" };
			static const std::size_t              maxIndex = levelNames.size() - 1;

			return levelNames[std::min(level, maxIndex)];
		}
	};

	template <std::size_t MAX_CAPACITY>
	class LogBuffer
	{
	public:
		LogBuffer() :_wptr(_buf.data()) { _buf[0] = 0; } // NOLINT

		LogBuffer& clear()
		{
			_wptr = _buf.data();
			_buf[0] = 0;
			return *this;
		}

		template<class ...Args>
		LogBuffer& print(const char* format, Args... args)
		{
			assert(!(_buf[0] != 0 && _wptr == _buf.data())); // means it has already been used as a plain buffer (with `str()`)

			auto cnt = std::snprintf(_wptr, capacity(), format, args...); // returns number of characters written if successful (not including '\0')
			if (cnt > 0)
			{
				_wptr = std::min(_wptr + cnt, _buf.data() + MAX_CAPACITY - 1);
			}
			return *this;
		}
		LogBuffer& print(const char* format) { return print("%s", format); }

		std::size_t capacity() const { return static_cast<size_t>(_buf.data() + MAX_CAPACITY - _wptr); }
		char* str() { return _buf.data(); }
		const char* c_str() const { return _buf.data(); }

	private:
		std::array<char, MAX_CAPACITY> _buf;
		char* _wptr;
	};

	using Logger = std::function<void(int, const char*, int, const char* msg)>;

	inline void UELogger(int level, const char* msg)
	{
		auto levelLog = Log::levelString(static_cast<std::size_t>(level));
		auto levelStr = UTF8_TO_TCHAR(std::move(levelLog).c_str());

		UE_LOG(LogKafka, Warning, TEXT("LogKafka: %s"), *FString(msg));
	}

	inline void DefaultLogger(int level, const char* /*filename*/, int /*lineno*/, const char* msg)
	{
		#if ENABLE_UE_LOGS
			UELogger(level, msg);
		#else
			std::cout << "[" << utility::getCurrentTime() << "]" << Log::levelString(static_cast<std::size_t>(level)) << " " << msg;
			std::cout << std::endl;
		#endif // ENABLE_UE_LOGS
	}

	inline void NullLogger(int /*level*/, const char* /*filename*/, int /*lineno*/, const char* /*msg*/)
	{
	}

} // end of KAFKA_API


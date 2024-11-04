#pragma once
#include <chrono>
#include <string>
namespace yang {
	namespace time {
		class Time : public std::chrono::nanoseconds {
		public:
			std::string toStdString(int precesion = 0);
		};
		/// <summary>
/// 计时器，用于统计程序耗时
/// </summary>
		class TimeCounter {
		private:
			/// <summary>
			/// 开始的时刻
			/// </summary>
			std::chrono::steady_clock::time_point StartTime;
			/// <summary>
			/// 结束的时刻
			/// </summary>
			std::chrono::steady_clock::time_point EndTime;
			/// <summary>
			/// 消耗的时间
			/// </summary>
			std::chrono::nanoseconds UseTime;
			/// <summary>
			/// 计时器是否工作
			/// </summary>
			bool IsStart = false;
		public:
			/// <summary>
			/// 初始化计时器
			/// </summary>
			TimeCounter();
			/// <summary>
			/// 开启计时器
			/// </summary>
			void Start();
			/// <summary>
			/// 关闭计时器
			/// </summary>
			void End();
			/// <summary>
			/// 重置计时器
			/// </summary>
			void Reset();
			/// <summary>
			/// 显示时间
			/// </summary>
			Time GetTime();
		};
	}
}

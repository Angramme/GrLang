#include "Logger.h"

#include <iostream>

#include "grlangConfig.h"

#ifdef PLATFORM_WIN32
#include <windows.h>
#endif


namespace grComp {
	namespace Log {
		static enum COLOR {
			red=0, green=1, blue, yellow, magenta, cyan, black, white
		};

		static void LOG(std::string msg, COLOR clr) {
#if defined PLATFORM_UNIX
			const short linux_colors[] = { 31, 32, 34, 33, 35, 36, 30, 37 };
			printf("\033[%dm%s", linux_colors[clr], msg.c_str());

#elif defined PLATFORM_WIN32
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			auto CL = 0;
			if(clr==red || clr == yellow || clr == magenta)
				CL |= FOREGROUND_RED;
			if (clr == green || clr == yellow || clr == cyan)
				CL |= FOREGROUND_GREEN;
			if (clr == blue || clr == magenta || clr == cyan)
				CL |= FOREGROUND_BLUE;

			SetConsoleTextAttribute(hConsole,
				CL);
			printf("%s\n", msg.c_str());
#endif

		}
		void InternalRuntimeError(std::string msg) {
			LOG(
				"Internal Compiler Runtime Error: \n"
				+ msg +
				"\n This is abnormal behaviour. Please report this error to the GrLang project maintainer.",
				magenta
			);
		}
		void CompiletimeError(std::string msg) {
			LOG(
				"Compile Error: " + msg,
				red
			);
		}
		void CompiletimeInfo(std::string msg) {
			LOG(
				"Compile info: " + msg,
				blue
			);
		}
	}
}
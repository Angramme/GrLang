#include "Logger.h"

#include <iostream>
#include <stdio.h>

#include "grlangConfig.h"

#ifdef PLATFORM_WIN32
#include <windows.h>
#endif


namespace grComp {
	namespace Log {

		static WORD _Default_Color_Attributes = 0;

		void PRINT_COLOR(const std::string& str, const COLOR& color, FILE* STDOUT) {
			return PRINT_COLOR(str.c_str(), color, STDOUT);
		}

		void PRINT_COLOR(const char* msg, const COLOR& clr, FILE* STDOUT) {
#if defined PLATFORM_UNIX
			if (clr != COLOR::DEFAULT) {
				const short linux_colors[] = { 31, 32, 34, 33, 35, 36, 30, 37 };
				fprintf(STDOUT, "\033[%dm%s\033[39m", linux_colors[clr-1], msg);
			}
			else {
				fprintf(STDOUT, "\033[39m%s", msg);
			}

#elif defined PLATFORM_WIN32
			if (clr != DEFAULT) {
				if (!_Default_Color_Attributes) {
					CONSOLE_SCREEN_BUFFER_INFO Info;
					GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
					_Default_Color_Attributes = Info.wAttributes;
				}

				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				auto CL = FOREGROUND_INTENSITY;
				if(clr==red || clr == yellow || clr == magenta)
					CL |= FOREGROUND_RED;
				if (clr == green || clr == yellow || clr == cyan)
					CL |= FOREGROUND_GREEN;
				if (clr == blue || clr == magenta || clr == cyan)
					CL |= FOREGROUND_BLUE;

				SetConsoleTextAttribute(hConsole, CL);
				fprintf(STDOUT, "%s\n", msg);
				SetConsoleTextAttribute(hConsole, _Default_Color_Attributes);
			}
			else {
				fprintf(STDOUT, "%s\n", msg);
			}
#endif

		}
		void InternalRuntimeError(std::string msg, const char* in_file, int on_line, const char* in_function) {
			PRINT_COLOR("\n!!==!!==!!==!!==!!==!!\n!!==!!==!!==!!==!!==!!", yellow, stderr);
			PRINT_COLOR(
				(std::string)"Internal Compiler Runtime Error: \n" +
				"\tin file: " + in_file +
				"\n\ton line: " + std::to_string(on_line) +
				"\n\tin function: " + in_function + "\n" +
				msg +
				"\n This is abnormal behaviour. Please report this error to the GrLang project maintainer.",
				magenta, stderr
			);
			PRINT_COLOR("!!==!!==!!==!!==!!==!!\n!!==!!==!!==!!==!!==!!\n", yellow, stderr);
		}
		void CompiletimeError(std::string msg) {
			PRINT_COLOR(
				msg,
				red
			);
		}
		void CompiletimeInfo(std::string msg) {
			PRINT_COLOR(
				msg,
				blue
			);
		}
	}
}
#pragma once
#include <string>

namespace Utils {
	namespace math {
		float round2dp(float num) {
			num *= 100;
			int tmp = (int)num + .5;
			return(tmp / 100000);
		}
	}

	std::string readFileAsString(const std::string& path) {
		struct stat sb {};
		std::string res;

		FILE* input_file = fopen(path.c_str(), "r");
		if (input_file == nullptr) {
			perror("fopen");
		}

		stat(path.c_str(), &sb);
		res.resize(sb.st_size);
		fread(const_cast<char*>(res.data()), sb.st_size, 1, input_file);
		fclose(input_file);

		return res;
	}

	std::string GetStdoutFromCommand(std::string cmd) {

		std::string data;
		FILE* stream;
		const int max_buffer = 256;
		char buffer[max_buffer];
		cmd.append(" 2>&1");

		stream = _popen(cmd.c_str(), "r");

		if (stream) {
			while (!feof(stream))
				if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
			_pclose(stream);
		}
		return data;
	}

	std::string getServerPing(std::string hostname) {
		std::string tmp1 = GetStdoutFromCommand("ping " + hostname + " -n 1");;

		std::string key = "Average =";

		int cnt = 0;
		std::string fnd = "";
		for (int i = 0; i < tmp1.length(); i++) {
			if (tmp1.at(i) == key.at(cnt)) {
				cnt++;
				fnd += tmp1.at(i);
			}
			else {
				cnt = 0;
				fnd = "";
			}
			if (cnt >= key.size()) {
				//std::cout << "FOUND" << std::endl;
				//std::cout << fnd << std::endl;
				while (1) {
					int contentBase = i + 1 + 1;
					int e = contentBase;
					std::string tempStr = "";
					while (tmp1.at(e) != 'm') {
						if (e >= tmp1.length()) {
							return "";
						}
						tempStr += tmp1.at(e);
						e++;
					}
					//std::cout << "DONE KEY IS" << std::endl;
					//std::cout << tempStr << std::endl;
					return tempStr;
				}
				clog::log("Could not find Average Ping", "getServerPing 1");
				return "";
			}
		}
		clog::log("Could not find Average Ping", "getServerPing 2");
		return "";

		return tmp1;

	}
}
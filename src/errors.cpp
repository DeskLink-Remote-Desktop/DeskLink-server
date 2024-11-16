#include <iostream>

#include "errors.hpp"


void error(const std::string &error_text, const ErrorLevel error_level, const std::string &ctx) {
	const std::string error_level_str = error_level_names.find(error_level)->second;

	std::cerr << ctx << error_level_str << ": " << error_text << std::endl;

	if (error_level >= ErrorLevel::FatalError) {
		std::exit(EXIT_FAILURE);
	}
}
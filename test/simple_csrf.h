#pragma once
#include <string>
 
std::string generateCSRFToken();
bool validateCSRFToken(const std::string& token); 
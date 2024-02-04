#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define UFM_CLIENT_EXPORT __declspec(dllexport)
#else
  #define UFM_CLIENT_EXPORT
#endif

UFM_CLIENT_EXPORT void ufm_client();
UFM_CLIENT_EXPORT void ufm_client_print_vector(const std::vector<std::string> &strings);

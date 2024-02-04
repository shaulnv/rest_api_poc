#include "ufm_client.h"
#include <vector>
#include <string>

int main() {
    ufm_client();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    ufm_client_print_vector(vec);
}

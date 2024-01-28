#include <doctest/doctest.h>
#include <ufm_client/greeter.h>
#include <ufm_client/version.h>

#include <string>

TEST_CASE("ufm_client") {
  using namespace greeter;

  ufm_client greeter("Tests");

  CHECK(greeter.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(greeter.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(greeter.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(greeter.greet(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("ufm_client version") {
  CHECK(std::string(UFM_CLIENT_VERSION) == std::string("1.0"));
}

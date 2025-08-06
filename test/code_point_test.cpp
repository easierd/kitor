#include<cassert>
#include<iostream>

#include "../src/code_point.h"

class CodePointTest {
    public:
        static void run_all_tests() {}
    private:
        static void test_constructor() {
            std::string empty {};

            UTF8CodePoint ucp {empty};

            assert(empty == ucp.to_string());
        }

        static void test_constructor_string() {
            std::string s{};
            s.push_back('h');
            s.push_back('\xff');

            UTF8CodePoint ucp {s};

            assert(s == ucp.to_string());
        }
};


int main(void) {
    try {
        CodePointTest :: run_all_tests();
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Test failed with unknown exception." << std::endl;
        return EXIT_FAILURE;
    }

    std :: cout << "All tests passed!" << std :: endl;

    return EXIT_SUCCESS;
}
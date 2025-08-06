#include<cassert>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include "../src/utf8_reader.h"

class UTF8ReaderTest {
    public:
        static void run_all_tests() {
            test_single_byte();
            test_eof();
            test_two_bytes();
            test_three_bytes();
            test_four_bytes();
        }
    private:
        static void test_single_byte() {
            int pipefd[2];

            assert(pipe(pipefd) != -1);

            int original_stdin = dup(STDIN_FILENO);

            dup2(pipefd[0], STDIN_FILENO);

            char c[] = {'\033', '[', 'D'};

            write(pipefd[1], c, 3);
            close(pipefd[1]);

            UTF8Reader reader;

            auto input = reader.next_input();
            assert(input.to_string() == "\033");

            input = reader.next_input();
            assert(input.to_string() == "[");

            input = reader.next_input();
            assert(input.to_string() == "D");

            dup2(original_stdin, STDIN_FILENO);
            close(original_stdin);
            close(pipefd[0]);
        }

        static void test_eof() {
            int orig_stdin = dup(STDIN_FILENO);

            int dummy_fd = open("/dev/null", O_RDWR);
            dup2(dummy_fd, STDIN_FILENO);

            UTF8Reader reader;
            auto input = reader.next_input();

            assert(input.to_string().empty());

            dup2(orig_stdin, STDIN_FILENO);
            close(orig_stdin);
            close(dummy_fd);
        }

        static void test_two_bytes() {
            int pipefd[2];
            assert(pipe(pipefd) != -1);
            int orig_stdin = dup(STDIN_FILENO);
            dup2(pipefd[0], STDIN_FILENO);

            char c[] = {'\xd7', '\xff'};
            write(pipefd[1], c, 2);
            close(pipefd[1]);
            UTF8Reader reader;

            auto input {reader.next_input()};
            std::string expected {c};

            assert(input.to_string() == expected);

            dup2(orig_stdin, STDIN_FILENO);
            close(orig_stdin);
            close(pipefd[0]);
        }

        static void test_three_bytes() {
            int pipefd[2];
            assert(pipe(pipefd) != -1);
            int orig_stdin = dup(STDIN_FILENO);
            dup2(pipefd[0], STDIN_FILENO);

            char c[] = {'\xe0','\xff','\x0a'};
            write(pipefd[1], c, 3);
            close(pipefd[1]);
            UTF8Reader reader;

            auto input {reader.next_input()};
            std::string expected {c};

            assert(input.to_string() == expected);

            dup2(orig_stdin, STDIN_FILENO);
            close(orig_stdin);
            close(pipefd[0]);
        }

        static void test_four_bytes() {
            int pipefd[2];
            assert(pipe(pipefd) != -1);
            int orig_stdin = dup(STDIN_FILENO);
            dup2(pipefd[0], STDIN_FILENO);

            char c[] = {'\xf7', '\x06', '\x0a', '\xdd'};
            write(pipefd[1], c, 4);
            close(pipefd[1]);
            UTF8Reader reader;

            auto input {reader.next_input()};
            std::string expected {c};

            assert(input.to_string() == expected);

            dup2(orig_stdin, STDIN_FILENO);
            close(orig_stdin);
            close(pipefd[0]);
        }
};


int main(void) {
    try {
        UTF8ReaderTest :: run_all_tests();
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
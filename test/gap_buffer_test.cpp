#include <cassert>
#include <iostream>

#include "../src/gap_buffer.h"

class GapBufferTest {
    public:
        static void run_all_test() {
            test_insertion();
            test_deletion();
            test_deletion_empty_buffer();
            test_left();
            test_left_start_of_buffer();
            test_right();
            test_right_end_of_buffer();
            test_expand();
        }
    private:
        static void test_insertion() {
            GapBuffer buffer;
            assert(buffer.to_string() == "");
            buffer.insert('A');
            assert(buffer.to_string() == "A");
            buffer.insert('C');
            assert(buffer.to_string() == "AC");
        }

        static void test_deletion() {
            GapBuffer buffer;
            buffer.insert('A');
            buffer.insert('B');
            buffer.del();
            assert(buffer.to_string() == "A");
        }

        static void test_deletion_empty_buffer() {
            GapBuffer buffer;
            buffer.del();
            assert(buffer.to_string().empty());
        }

        static void test_left() {
            GapBuffer buffer;
            buffer.insert('A');
            buffer.left();
            buffer.insert('B');
            assert(buffer.to_string() == "BA");
        }

        static void test_left_start_of_buffer() {
            GapBuffer buffer;
            buffer.left();
            buffer.insert('A');
            assert(buffer.to_string() == "A");
        }

        static void test_right() {
            GapBuffer buffer;
            buffer.insert('A');
            buffer.left();
            buffer.insert('B');
            buffer.right();
            buffer.insert('C');
            assert(buffer.to_string() == "BAC");
        }

        static void test_right_end_of_buffer() {
            GapBuffer buffer;
            buffer.insert('A');
            buffer.right();
            buffer.insert('B');
            assert(buffer.to_string() == "AB");
        }

        static void test_expand() {
            GapBuffer buffer{2};
            buffer.insert('a');
            buffer.insert('b');
            buffer.insert('c');
            buffer.insert('d');
            buffer.insert('e');
            assert(buffer.size() == 8);
            assert(buffer.to_string() == "abcde");
        }


};

int main(void) {
    try {
        GapBufferTest :: run_all_test();
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Test failed with unknown exception." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#include <algorithm>
#include <cstdio>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>
#include <fstream>

using std::string, std::vector;

struct Vec2 {
    int y, x;

    void add(const Vec2& operand) {
        y += operand.y;
        x += operand.x;
    }

    bool operator==(const Vec2& other) {
        return y == other.y && x == other.x;
    }
};

namespace Direction {
    const Vec2 LEFT = {0, -1};
    const Vec2 RIGHT = {0, 1};
    const Vec2 UP = {-1, 0};
    const Vec2 DOWN = {1, 0};
}

class Stack : std::stack<int> {
    public:
        int pop() {
            int a = top();
            std::stack<int>::pop();
            return a;
        }
        void push(int a) { std::stack<int>::push(a); }
        bool isEmpty() { return std::stack<int>::empty(); }
};

void crash(const char* error, const string& token, const Vec2& location) {
    int y = location.y + 1;
    int x = location.x + 1;
    const char* t = token.c_str();
    printf("`%s` at line %d column %d caused the following error:\n\t%s\n", t, y, x, error);
    exit(1);
}

void testError(bool is_error, const char* message) {
    if (is_error) {
        printf("%s\n", message);
        exit(1);
    }
}

void interpretProgram(const vector<string>& program) {
    std::unordered_map<string, Vec2> proc_locs; // location of procedures
    
    // Find the procedures
    for (int h = 0; h < (int) program.size(); h++) {
        const string& line = program[h];
        int line_width = line.size();
        for (int i = 0; i < line_width; i++) {
            char c = line[i];
            if (c == '}' && i != 0 && line[i - 1] != ' ') {
                string buffer;
                int j = 1;
                while (i - j >= 0 && line[i - j] != ' ') {
                    buffer += line[i - j];
                    j++;
                }
                std::reverse(buffer.begin(), buffer.end());
                proc_locs[buffer] = {h, i};
            } else if (c == '{' && line[i + 1] != ' ') {
                string buffer;
                int j = 1;
                while (i + j < line_width && line[i + j] != ' ') {
                    buffer += line[i + j];
                    j++;
                }
                proc_locs[buffer] = {h, i};
            }
        }
    }

    testError(!proc_locs.contains("main"), "No main procedure");
    
    Vec2 location = proc_locs["main"];
    Vec2 direction = program[location.y][location.x] == '}' ? Direction::RIGHT : Direction::LEFT;
    location.add(direction);
    Stack stack;
    string buffer;
    
    while (true) {
        char c = program[location.y][location.x];

        switch (c) {
            case '/':
            case '\\':
            case '-':
            case '|':
                if (buffer.size() == 0) break;

                if (direction == Direction::LEFT || direction == Direction::UP) {
                    std::reverse(buffer.begin(), buffer.end());
                }

                if (buffer == "add") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(a + b);
                } else if (buffer == "*") {
                    // printing
                    printf("%d\n", stack.pop());
                } else {
                    // number stuff
                    int num;
                    try {
                        num = std::stoi(buffer);
                    } catch (const std::invalid_argument&) {
                        crash("Unknown word", buffer, location);
                    }
                    stack.push(num);
                }
                buffer = "";
                break;
            case '}':
            case '{':
                if (stack.isEmpty()) crash("No exit code on stack", string(1, c), location);
                exit(stack.pop());
            default:
                buffer += c;
        }

        if (c == '\\') {
            if (direction == Direction::RIGHT) {
                direction = Direction::DOWN;
            } else if (direction == Direction::DOWN) {
                direction = Direction::RIGHT;
            } else if (direction == Direction::LEFT) {
                direction = Direction::UP;
            } else if (direction == Direction::UP) {
                direction = Direction::LEFT;
            }
        } else if (c == '/') {
            if (direction == Direction::DOWN) {
                direction = Direction::LEFT;
            } else if (direction == Direction::LEFT) {
                direction = Direction::DOWN;
            } else if (direction == Direction::RIGHT) {
                direction = Direction::UP;
            } else if (direction == Direction::UP) {
                direction = Direction::RIGHT;
            }
        }
        
        location.add(direction);
    }
}

int main(int argc, char **argv) {
    testError(argc < 2, "You must include an input file");

    vector<string> program;
    
    // Read File
    std::ifstream file(argv[1]);
    testError(file.fail(), "File not found");
    string input_line;
    while (std::getline(file, input_line)) {
        program.push_back(input_line);
    }
    file.close();

    interpretProgram(program);
}

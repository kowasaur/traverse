#include <algorithm>
#include <cstdio>
#include <tuple>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string>
#include <fstream>

using std::string, std::vector;

struct Vec2 {
    int y, x;

    // This mutates the original Vec2
    void add(const Vec2& operand) {
        y += operand.y;
        x += operand.x;
    }

    // This does not mutate anything
    Vec2 addCopy(const Vec2& operand) const {
        Vec2 newVec2 = {y + operand.y, x + operand.x};
        return newVec2;
    }

    bool operator==(const Vec2& other) const {
        return y == other.y && x == other.x;
    }
};

namespace Direction {
    const Vec2 LEFT = {0, -1};
    const Vec2 RIGHT = {0, 1};
    const Vec2 UP = {-1, 0};
    const Vec2 DOWN = {1, 0};
    const Vec2 ALL[4] = {LEFT, UP, RIGHT, DOWN};

    const Vec2 opposite(const Vec2& dir) {
        return {dir.y * -1, dir.x * -1};
    }

    const Vec2 rotated(const Vec2& dir, char c) {
        if (c == '\\') {
            if (dir == Direction::RIGHT) {
                return Direction::DOWN;
            } else if (dir == Direction::DOWN) {
                return Direction::RIGHT;
            } else if (dir == Direction::LEFT) {
                return Direction::UP;
            } else if (dir == Direction::UP) {
                return Direction::LEFT;
            }
        } else if (c == '/') {
            if (dir == Direction::DOWN) {
                return Direction::LEFT;
            } else if (dir == Direction::LEFT) {
                return Direction::DOWN;
            } else if (dir == Direction::RIGHT) {
                return Direction::UP;
            } else if (dir == Direction::UP) {
                return Direction::RIGHT;
            }
        }
        return dir;
    }
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

class ReturnStack {
    private:
        std::stack<Vec2> locations;
        std::stack<Vec2> directions;

    public:
        void push(const Vec2 location, const Vec2 direction) {
            locations.push(location);
            directions.push(direction);
        }

        std::tuple<Vec2, Vec2> pop() {
            Vec2 loc = locations.top();
            Vec2 dir = directions.top();
            locations.pop();
            directions.pop();
            return std::make_tuple(loc, dir);
        }

        bool isEmpty() {
            return locations.empty();
        }
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
    ReturnStack return_stack;
    string buffer;
    
    while (true) {
        char c = program[location.y][location.x];

        // Character literals
        if (buffer.size() > 0 && buffer[0] == '\'') {
            if (buffer.size() > 1 && c == '\'') {
                char new_c;
                int i1 = 1;
                int i2 = 2;
                if (direction == Direction::LEFT || direction == Direction::UP) {
                    i1 = 2;
                    i2 = 1;
                }

                if (buffer.size() == 2) new_c = buffer[1];
                else if (buffer.size() == 3 && buffer[i1] == '\\') {
                    if (buffer[i2] == 'n') new_c = '\n';
                    else crash("unsupported escape character", buffer, location);
                }
                else crash("character must be singular", buffer, location);

                stack.push(new_c);
                buffer = "";
            } else {
                buffer += c;
            }
            location.add(direction);
            continue;
        }

        switch (c) {
            case '/':
            case '\\':
            case '-':
            case '|':
                if (buffer.size() == 0) break;

                if (direction == Direction::LEFT || direction == Direction::UP) {
                    std::reverse(buffer.begin(), buffer.end());
                }

                // Addition
                if (buffer == "add") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(a + b);
                // Subtraction
                } else if (buffer == "sub") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(b - a);
                // Multiplication
                } else if (buffer == "mul") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(a * b);
                // Division
                } else if (buffer == "div") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(b / a);
                // Modulo
                } else if (buffer == "mod") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(b % a);
                // Equal
                } else if (buffer == "=") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(a == b);
                // Not Equal
                } else if (buffer == "!=") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(a != b);
                // Greater than
                } else if (buffer == ">") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(b > a);
                // Less than
                } else if (buffer == "<") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(b < a);
                // Greater than or Equal
                } else if (buffer == ">=") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(b >= a);
                // Less than or Equal
                } else if (buffer == "<=") {
                    int a = stack.pop();
                    int b = stack.pop();
                    stack.push(b <= a);
                // Printing number
                } else if (buffer == "*") {
                    printf("%d\n", stack.pop());
                // Printing ascii character
                } else if (buffer == "&") {
                    printf("%c", stack.pop());
                // Duplication
                } else if (buffer == "%") {
                    int a = stack.pop();
                    stack.push(a);
                    stack.push(a);
                // Procedures
                } else if (proc_locs.contains(buffer)) {
                    return_stack.push(location, direction);
                    location = proc_locs[buffer];
                    direction = program[location.y][location.x] == '}' ? Direction::RIGHT : Direction::LEFT;
                // Integer literals
                } else {
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
            case '+': {
                int start_index;
                for (int i = 0; i < 4; i++) {
                    if (Direction::ALL[i] == Direction::opposite(direction)) {
                        start_index = i;
                        break;
                    }
                }

                vector<Vec2> paths;
                for (int i = 1; i < 4; i++) {
                    int dir_index = (start_index + i) % 4;
                    const Vec2& dir = Direction::ALL[dir_index];
                    Vec2 path = location.addCopy(dir);

                    // TODO: Maybe add support for going from + to \ or /
                    if (dir == Direction::LEFT || dir == Direction::RIGHT ) {
                        const string& line = program[path.y];
                        if (path.x < 0 || path.x >= (int) line.size() || line[path.x] != '-')
                            continue;
                    } else if (dir == Direction::UP || dir == Direction::DOWN) {
                        int lines = program.size();
                        if (path.y < 0 || path.y >= lines || program[path.y][path.x] != '|')
                            continue;
                    }
                    // Path is fine
                    paths.push_back(dir);
                }
                
                if (paths.size() == 2) {
                    if (stack.pop()) direction = paths[0];
                    else direction = paths[1];
                } else if (paths.size() == 3) {
                    if (stack.pop()) {
                        direction = paths[0];
                        stack.pop();
                    }
                    else if (stack.pop()) direction = paths[1];
                    else direction = paths[2];
                } else {
                    crash("must have at least two exit paths", string(1, c), location);
                }

                break;
            }
            case '}':
            case '{':
                if (return_stack.isEmpty()) {
                    if (stack.isEmpty()) crash("No exit code on stack", string(1, c), location);
                    exit(stack.pop());
                }
                std::tie(location, direction) = return_stack.pop();
                break;
            default:
                buffer += c;
        }

        direction = Direction::rotated(direction, c);

        location.add(direction);

        if (program[location.y][location.x] == ' ') {
            Vec2 additional = Direction::rotated(direction, c);
            if (additional != direction) location.add(additional);
        }
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

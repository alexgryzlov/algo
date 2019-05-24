/*Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
 * [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку.
 * Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
*/


#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

const unsigned long long Masks[] = {
        0x000000000000000F,
        0x00000000000000F0,
        0x0000000000000F00,
        0x000000000000F000,
        0x00000000000F0000,
        0x0000000000F00000,
        0x000000000F000000,
        0x00000000F0000000,
        0x0000000F00000000,
        0x000000F000000000,
        0x00000F0000000000,
        0x0000F00000000000,
        0x000F000000000000,
        0x00F0000000000000,
        0x0F00000000000000,
        0xF000000000000000,
};

const unsigned long long AntiMasks[] = {
        0xFFFFFFFFFFFFFFF0,
        0xFFFFFFFFFFFFFF0F,
        0xFFFFFFFFFFFFF0FF,
        0xFFFFFFFFFFFF0FFF,
        0xFFFFFFFFFFF0FFFF,
        0xFFFFFFFFFF0FFFFF,
        0xFFFFFFFFF0FFFFFF,
        0xFFFFFFFF0FFFFFFF,
        0xFFFFFFF0FFFFFFFF,
        0xFFFFFF0FFFFFFFFF,
        0xFFFFF0FFFFFFFFFF,
        0xFFFF0FFFFFFFFFFF,
        0xFFF0FFFFFFFFFFFF,
        0xFF0FFFFFFFFFFFFF,
        0xF0FFFFFFFFFFFFFF,
        0x0FFFFFFFFFFFFFFF
};

struct Configuration {
    Configuration() {}

    Configuration(const Configuration& configuration) : data(configuration.data),
                                                        null_position(configuration.null_position),
                                                        father(configuration.father),
                                                        manhattan_distance(configuration.manhattan_distance), depth(configuration.depth) {}

    Configuration(const std::vector<unsigned char>&);

    int null_position;
    int manhattan_distance;
    int depth;
    unsigned long long data;
    unsigned long long father;


    void set_at(int position, unsigned char value);
    unsigned char get_at(int position) const;

    Configuration up() const;
    Configuration down() const;
    Configuration left() const;
    Configuration right() const;
    void calculate_manhattan_distance();
    void calculate_manhattan_distance1();
    bool solvable() const;
};


void Configuration::set_at(int position, unsigned char value) {
    data = (data & AntiMasks[position]) | (static_cast<unsigned long long>(value) << (position << 2));

}

unsigned char Configuration::get_at(int position) const {
    return static_cast<unsigned char>((data & Masks[position]) >> (position << 2));
}

Configuration::Configuration(const std::vector<unsigned char>& input) {
    data = 0;
    depth = 0;
    father = -1;
    manhattan_distance = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        set_at(i, input[i]);
        if (input[i] == 0) {
            null_position = i;
        }
    }
    calculate_manhattan_distance();
}

Configuration Configuration::up() const {
    Configuration result(*this);
    result.set_at(null_position - 4, 0);
    result.set_at(null_position, get_at(null_position - 4));
    result.null_position -= 4;
    result.father = data;
    result.depth++;
    result.calculate_manhattan_distance();

    return result;
}

Configuration Configuration::down() const {
    Configuration result(*this);
    result.set_at(null_position + 4, 0);
    result.set_at(null_position, get_at(null_position + 4));
    result.null_position += 4;
    result.father = data;
    result.depth++;
    result.calculate_manhattan_distance();

    return result;
}

Configuration Configuration::left() const {
    Configuration result(*this);
    result.set_at(null_position - 1, 0);
    result.set_at(null_position, get_at(null_position - 1));
    result.null_position -= 1;
    result.father = data;
    result.depth++;
    result.calculate_manhattan_distance();

    return result;
}

Configuration Configuration::right() const {
    Configuration result(*this);
    result.set_at(null_position + 1, 0);
    result.set_at(null_position, get_at(null_position + 1));
    result.null_position += 1;
    result.father = data;
    result.depth++;
    result.calculate_manhattan_distance();

    return result;
}

void Configuration::calculate_manhattan_distance() {
    manhattan_distance = 0;
    for (int i = 0; i < 16; ++i) {
        if (get_at(i) == 0) {
            continue;
        }
        int current_x = i % 4;
        int current_y = i / 4;
        int correct_x = (get_at(i) - 1) % 4;
        int correct_y = (get_at(i)  - 1) / 4;
        manhattan_distance += abs(current_x - correct_x) + abs(current_y - correct_y);
    }
}

bool Configuration::solvable() const {
    int permutation_parity = 0;
    for (int i = 0; i < 16; ++i) {
        for (int j = i + 1; j < 16; ++j) {
            if (get_at(j) < get_at(i) && get_at(i) != 0 && get_at(j) != 0) {
                permutation_parity++;
            }
        }
    }
    return (permutation_parity + (null_position / 4) + 1) % 2 == 0;
}

void solve(const Configuration &initial_configuration) {
    if (!initial_configuration.solvable()) {
        std::cout << -1;
        return;
    }

    std::set<std::pair<unsigned long long, unsigned long long>> q;
    std::unordered_map<unsigned long long, Configuration> used;
    std::unordered_map<unsigned long long, unsigned long long> distances;
    q.insert(std::make_pair(initial_configuration.manhattan_distance, initial_configuration.data));
    used[initial_configuration.data] = initial_configuration;
    distances[initial_configuration.data] = 0;
    while (!q.empty()) {
        Configuration current_configuration = used[q.begin()->second]; q.erase(q.begin());
        if (current_configuration.data == 1147797409030816545) {
            std::vector<char> move_sequence;
            int moves_count = 0;
            while (current_configuration.father != -1) {
                ++moves_count;
                if (current_configuration.left().data == current_configuration.father) {
                    move_sequence.push_back('L');
                }
                else if (current_configuration.right().data == current_configuration.father) {
                    move_sequence.push_back('R');
                }
                else if (current_configuration.up().data == current_configuration.father) {
                    move_sequence.push_back('U');
                }
                else if (current_configuration.down().data == current_configuration.father) {
                    move_sequence.push_back('D');
                }
                current_configuration = used[current_configuration.father];
            }
            std::cout << moves_count << std::endl;
            for (int i = 0; i < moves_count; ++i) {
                std::cout << move_sequence[moves_count - i - 1];
            }
            std::cout << std::endl;
            return;
        }

        if (current_configuration.null_position % 4 != 0) {
            Configuration new_configuration = current_configuration.left();
            if (used.find(new_configuration.data) == used.end()) {
                q.erase(std::make_pair(new_configuration.manhattan_distance, new_configuration.data));
                used[new_configuration.data] = new_configuration;
                q.insert(std::make_pair(new_configuration.manhattan_distance, new_configuration.data));
            }
        }

        if (current_configuration.null_position % 4 != 3) {
            Configuration new_configuration = current_configuration.right();
            if (used.find(new_configuration.data) == used.end()) {
                q.erase(std::make_pair(new_configuration.manhattan_distance, new_configuration.data));
                used[new_configuration.data] = new_configuration;
                distances[new_configuration.data] = distances[current_configuration.data] + 1;
                q.insert(std::make_pair(new_configuration.manhattan_distance, new_configuration.data));
            }
        }

        if (current_configuration.null_position < 12) {
            Configuration new_configuration = current_configuration.down();
            if (used.find(new_configuration.data) == used.end()) {
                q.erase(std::make_pair(new_configuration.manhattan_distance, new_configuration.data));
                used[new_configuration.data] = new_configuration;
                q.insert(std::make_pair(new_configuration.manhattan_distance, new_configuration.data));
            }
        }

        if (current_configuration.null_position > 3 ) {
            Configuration new_configuration = current_configuration.up();
            if (used.find(new_configuration.data) == used.end()) {
                q.erase(std::make_pair(new_configuration.manhattan_distance, new_configuration.data));
                used[new_configuration.data] = new_configuration;
                q.insert(std::make_pair(new_configuration.manhattan_distance, new_configuration.data));
            }
        }
    }

}

int main() {
    std::vector<unsigned char> input;
    for (int i = 0; i < 16; ++i) {
        int value = 0; std::cin >> value;
        input.push_back(value);
    }
    Configuration a = input;
    solve(a);
    std::cout << std::endl;
    return 0;
}

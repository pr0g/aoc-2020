#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <array>
#include <numeric>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <functional>
#include <queue>
#include <list>

// 11562782
// 18108497

std::unordered_map<int64_t, int64_t> cache;

int64_t transform(int64_t loop_size, int64_t subject_number) {
    
    int64_t i = 0;
    int64_t key = 1;
    if (auto c = cache.find(loop_size - 1);
        c != cache.end() && loop_size != 1) {
        key = c->second;
        i = loop_size - 1;
    }
    
    for (; i < loop_size; ++i) {
        key *= subject_number;
        key %= 20201227;
    }

    cache[loop_size] = key;
    return key;
}

int main(int, char**)
{
    // std::cout << transform(8, 7) << '\n';
    // std::cout << transform(11, 7) << '\n';

    // std::cout << transform(8, 17807724) << '\n';
    // std::cout << transform(11, 5764801) << '\n';

    // int64_t loop_size = 1;
    // for (bool found = false; !found; ++loop_size) {
    //     if (transform(loop_size, 7) == 11562782) {
    //         found = true;
    //         break;
    //     }
    // }
    // std::cout << loop_size << '\n';

    // 19976408
    // 17580934

    std::cout << transform(19976408, 7) << '\n';
    std::cout << transform(17580934, 7) << '\n';

    std::cout << transform(19976408, 11562782) << '\n';
    std::cout << transform(17580934, 18108497) << '\n';

    return 0;
}

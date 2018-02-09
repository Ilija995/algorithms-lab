#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> state_sum;

std::vector< std::pair<state_sum, int> >* get_state_sums(std::vector< std::vector<int> > *switches)
{
    int n_switches = switches->size();
    int n_rooms = (*switches)[0].size() / 2;

    std::vector< std::pair<state_sum, int> > *state_sums = new std::vector< std::pair<state_sum, int> >;

    for (int perm = 0; perm < 1 << n_switches; ++perm)
    {
        state_sum turned_on(n_rooms);
        for (int i_room = 0; i_room < n_rooms; ++i_room) turned_on[i_room] = 0;

        int curr_pressed = 0;
        for (int i_switch = 0; i_switch < n_switches; ++i_switch)
        {
            bool press_switch = (perm & 1 << i_switch) >> i_switch;
            curr_pressed += press_switch;
            for (int i_room = press_switch; i_room < 2 * n_rooms; i_room += 2)
                turned_on[i_room / 2] += (*switches)[i_switch][i_room];
        }

        state_sums->emplace_back(std::make_pair(turned_on, curr_pressed));
    }

    return state_sums;
}

void testcase()
{
    int n_switches, n_rooms;
    std::cin >> n_switches >> n_rooms;

    std::vector<int> room_target(n_rooms);
    for (int i_room = 0; i_room < n_rooms; ++i_room)
        std::cin >> room_target[i_room];

    int n_switches_1 = n_switches / 2;
    std::vector< std::vector<int> > switches_1(n_switches_1);
    for (int i_switch = 0; i_switch < n_switches_1; ++i_switch)
    {
        switches_1[i_switch].reserve(2 * n_rooms);
        for (int i = 0; i < 2 * n_rooms; ++i)
        {
            switches_1[i_switch].emplace_back();
            std::cin >> switches_1[i_switch].back();
        }
    }

    auto comparator = [](const std::pair<state_sum, int> &lhs, const std::pair<state_sum, int> &rhs) {
        int i = 0;
        while (i < lhs.first.size() && lhs.first[i] == rhs.first[i]) ++i;

        if (i == lhs.first.size()) return false;
        else return lhs.first[i] < rhs.first[i];
    };

    auto result_1_ptr = get_state_sums(&switches_1);
    std::vector< std::pair<state_sum, int> > result_1 = *result_1_ptr;

    std::sort(result_1.begin(), result_1.end(), comparator);
/*
    std::cout << "Result 1:\n";
    for (int i = 0; i < result_1.size(); ++i)
    {
        std::cout << result_1[i].second << " : ";
        for (int j = 0; j < n_rooms; ++j)
            std::cout << result_1[i].first[j] << " ";
        std::cout << std::endl;
    }
*/
    int n_switches_2 = n_switches - n_switches_1;
    std::vector< std::vector<int> > switches_2(n_switches_2);
    for (int i_switch = 0; i_switch < n_switches_2; ++i_switch)
    {
        switches_2[i_switch].reserve(2 * n_rooms);
        for (int i = 0; i < 2 * n_rooms; ++i)
        {
            switches_2[i_switch].emplace_back();
            std::cin >> switches_2[i_switch].back();
        }
    }

    auto result_2_ptr = get_state_sums(&switches_2);
    std::vector< std::pair<state_sum, int> > result_2 = *result_2_ptr;

    std::sort(result_2.begin(), result_2.end(), comparator);
/*
    std::cout << "Result 2:\n";
    for (int i = 0; i < result_2.size(); ++i)
    {
        std::cout << result_2[i].second << " : ";
        for (int j = 0; j < n_rooms; ++j)
            std::cout << result_2[i].first[j] << " ";
        std::cout << std::endl;
    }
*/
    bool solution_found = false;
    int min_pressed;
    for (const auto &pair_1 : result_1)
    {
        state_sum wanted(n_rooms);
        for (int i_room = 0; i_room < n_rooms; ++i_room)
            wanted[i_room] = room_target[i_room] - pair_1.first[i_room];
/*
        std::cout << "Looking for: ";
        for (int i = 0; i < n_rooms; ++i) std::cout << wanted[i] << " ";
        std::cout << std::endl;

        int i = 0;
        while (i < n_rooms && wanted[i] == result_2[result_2.size() - 1].first[i]) ++i;
        if (i == n_rooms)
        {
            std::cout << " > Found 6 4 3 3" << std::endl;
            std::cout << comparator(std::make_pair(wanted, 0), result_2[result_2.size() - 1]) << std::endl;
            std::cout << comparator(result_2[result_2.size() - 1], std::make_pair(wanted, 0)) << std::endl;
            for (int j = 0; j < n_rooms; ++j) std::cout << result_2[result_2.size() - 1].first[j] << " ";
            std::cout << std::endl;
            std::cout << result_2[result_2.size() - 1].first.size() << std::endl;
        }
*/
        auto it_pair = std::equal_range(result_2.begin(), result_2.end(), std::make_pair(wanted, 0), comparator);
        for (auto pair_2_it = it_pair.first; pair_2_it != it_pair.second; ++pair_2_it)
        {
            //for (int i = 0; i < n_rooms; ++i) std::cout << pair_2_it->first[i] << " ";
            //std::cout << std::endl;

            if (!solution_found)
            {
                min_pressed = pair_1.second + pair_2_it->second;
                solution_found = true;
            }
            else if (min_pressed > pair_1.second + pair_2_it->second)
            {
                min_pressed = pair_1.second + pair_2_it->second;
            }
        }
    }

    if (!solution_found) std::cout << "impossible" << std::endl;
    else std::cout << min_pressed << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}

#include <iostream>
#include <vector>

void testcase()
{
    int n_defenders, n_attackers, attack_strength;
    std::cin >> n_defenders >> n_attackers >> attack_strength;

    std::vector<int> defense_strength(n_defenders);
    for (int i_def = 0; i_def < n_defenders; ++i_def) std::cin >> defense_strength[i_def];

    // v_i[idx] - start of the interval, which ends at idx (both inclusive)
    // -1 - no valid interval end in idx
    std::vector<int> valid_intervals(n_defenders);
    int i = 0, j = 0;
    int sum = defense_strength[j];
    while (j < n_defenders)
    {
        valid_intervals[j] = (sum == attack_strength) ? i : -1;

        if (sum < attack_strength)
        {
            ++j;
            if (j < n_defenders) sum += defense_strength[j];
        }
        else if (sum == attack_strength)
        {
            sum -= defense_strength[i++];
            ++j;
            if (j < n_defenders) sum += defense_strength[j];
        }
        else
        {
            sum -= defense_strength[i++];
        }

        if (i > j)
        {
            ++j;
            if (j < n_defenders) sum += defense_strength[j];
        }
    }

    std::vector<int> solution(n_defenders + 1);
    for (int i = 0; i < n_defenders + 1; ++i) solution[i] = 0;

    for (int i_attacker = 0; i_attacker < n_attackers; ++i_attacker)
    {
        std::vector<int> new_sol(n_defenders + 1);
        new_sol[0] = -1;
        for (int i_defender = 1; i_defender < n_defenders + 1; ++i_defender)
        {
            if (valid_intervals[i_defender - 1] == -1 || solution[valid_intervals[i_defender - 1]] == -1) new_sol[i_defender] = new_sol[i_defender - 1];
            else new_sol[i_defender] = std::max(new_sol[i_defender - 1], solution[valid_intervals[i_defender - 1]] + i_defender - valid_intervals[i_defender - 1]);
        }
        solution = new_sol;
    }

    if (solution[n_defenders] == -1) std::cout << "fail" << std::endl;
    else std::cout << solution[n_defenders] << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}

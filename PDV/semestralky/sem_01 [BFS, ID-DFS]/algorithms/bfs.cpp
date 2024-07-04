#include "bfs.h"

#include <bits/stdc++.h>

// Naimplementujte efektivni algoritmus pro nalezeni nejkratsi cesty v grafu.
// V teto metode nemusite prilis optimalizovat pametove naroky, a vhodnym algo-
// ritmem tak muze byt napriklad pouziti prohledavani do sirky (breadth-first
// search.
//
// Metoda ma za ukol vratit ukazatel na cilovy stav, ktery je dosazitelny pomoci
// nejkratsi cesty.

// Materials:
// https://www.youtube.com/watch?v=SKhMrCaaduU&ab_channel=IITDelhiJuly2018

std::shared_ptr<const state> bfs(std::shared_ptr<const state> root) {
    std::shared_ptr<const state> goal = nullptr;
    std::unordered_set<unsigned long long int> us;
    std::vector< std::shared_ptr<const state> > v1,v2;
    v1.push_back(root);

    while (goal == nullptr) {
#pragma omp parallel for
        for (size_t i = 0; i < v1.size(); i++) {
            if(v1[i]->is_goal()) {
                if(goal != nullptr && v1[i]->get_identifier() > goal->get_identifier()){
                    continue;
                }
#pragma omp critical
                {
                    goal = v1[i];
                }
            }



            if(goal == nullptr && us.find(v1[i]->get_identifier()) == us.end()){
                auto next = v1[i]->next_states();
#pragma omp critical
                {
                    us.insert(v1[i]->get_identifier());
                    v2.insert( v2.end(), next.begin(), next.end() );
                }
            }

        }

        v1.swap(v2);
        v2.clear();
    }
//    std::cout << "BFS ID ===== " << goal->get_identifier() << std::endl;

    return goal;
}
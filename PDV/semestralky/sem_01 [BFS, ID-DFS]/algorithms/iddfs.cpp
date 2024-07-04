#include "iddfs.h"

#include <bits/stdc++.h>
#include <omp.h>

// Naimplementujte efektivni algoritmus pro nalezeni nejkratsi (respektive nej-
// levnejsi) cesty v grafu. V teto metode mate ze ukol naimplementovat pametove
// efektivni algoritmus pro prohledavani velkeho stavoveho prostoru. Pocitejte
// s tim, ze Vami navrzeny algoritmus muze bezet na stroji s omezenym mnozstvim
// pameti (radove nizke stovky megabytu). Vhodnym pristupem tak muze byt napr.
// iterative-deepening depth-first search.
//
// Metoda ma za ukol vratit ukazatel na cilovy stav, ktery je dosazitelny pomoci
// nejkratsi/nejlevnejsi cesty.

std::shared_ptr<const state> goal = nullptr;

void dfs_seq(std::shared_ptr<const state> root, int max_depth) {
    if(root->is_goal()){
        if(goal == nullptr || goal->current_cost() > root->current_cost() || (goal->current_cost() == root->current_cost() && goal->get_identifier() > root->get_identifier())){
#pragma omp critical
            {
                goal = root;
            }
        }
        return;
    }

    if(max_depth == 0){
        return;
    }

    auto next = root->next_states();
    if(next.empty()){
        return;
    }

    for(const auto & val : next){
        if( val->get_predecessor() != nullptr && val->get_predecessor()->get_identifier() == val->get_identifier()){
            continue;
        }
        dfs_seq(val, max_depth - 1);
    }
}


void dfs_par(std::shared_ptr<const state> root, int max_depth, int limit){
    if(root->is_goal()){
        if(goal == nullptr || goal->current_cost() > root->current_cost() || (goal->current_cost() == root->current_cost() && goal->get_identifier() > root->get_identifier())){
#pragma omp critical
            {
                goal = root;
            }
        }
        return;
    }

    if(max_depth <= limit){
        dfs_seq(root,max_depth);
        return;
    }

    auto next = root->next_states();
    if(next.empty()){
        return;
    }

    for(int i = 0; i < next.size(); i++){
        if( next[i]->get_predecessor() != nullptr && next[i]->get_predecessor()->get_identifier() == next[i]->get_identifier()){
            continue;
        }

#pragma omp task
        dfs_par(next[i], max_depth - 1, limit);
    }
}

std::shared_ptr<const state> iddfs(std::shared_ptr<const state> root) {
    for(int i = 0; i < 1000 && goal == nullptr; i++){
//        std::cout << i << std::endl;
#pragma omp parallel
#pragma omp single
        dfs_par(root, i, floor(i * 0.75));
    }

    return goal;
}
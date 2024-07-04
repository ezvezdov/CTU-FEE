#ifndef LOCKBASED_H
#define LOCKBASED_H

#include <vector>
#include <iostream>
#include <atomic>

#include "spin.h"

class Concurrent {
public:
    class Node {
    public:
        long long value;
        Node * next = nullptr;
        spin_mutex m;

        Node(long long value) : value(value) {}
    };

    Node * head = new Node(-999999999999L);

    // Naimplementujte vlaknove-bezpecne vkladani do konkurentniho spojoveho seznamu.
    // Pro popis struktury spojoveho seznamu se podivejte do souboru 'sequential.h',
    // kde naleznete i implementaci vkladani do spojoveho seznamu, ktera neni vlaknove
    // bezpecna (tj., muze ji provadet pouze jedno vlakno).
    //
    // Vsimnete si, ze trida Node reprezentujici uzel spojoveho seznamu obsahuje svuj
    // vlastni mutex 'm'.
    void insert(long long value) {
        Node* node = new Node(value);

        // tohle je verze, ktera funguje i s `remove()`,
        //  lockuju nody uz pri hledani mista kam vkladam
        Node* current = head;
        spin_mutex* current_m = &current->m;
        current_m->lock();

        // standardni traversal linked listu, prochazim a porovnavam, jestli uz mam vkladat
        // i kdyz by byl implementovany `remove()`, tak mi nevadi, ze v podmince saham
        //  na `current->next->value` v dobe, kdy nemam zamceny `current->next->m`, protoze pred
        //  jeho odebranim by jine vlakno muselo zmenit hodnotu `current->next`,
        //  a to neudela, kdyz drzime `current->m`
        while (current->next != nullptr && current->next->value < value) {
            current = current->next;
            current_m->unlock();
            current_m = &current->m;
            current_m->lock();
        }
        node->next = current->next;
        current->next = node;
        current_m->unlock();
    }

    // Pokud byste si chteli vyzkouset slozitejsi operaci se spojovym seznamem, muzete
    // si zkusit naimplementovat metodu pro odebrani prvku ze seznamu. Vzpomente si,
    // ze na prednasce jsme si rikali, ze mazani prvku probiha dvoufazove.
    //   1) Nejprve prvek oznacim za smazany. Tim zabranim ostatnim vlaknu vkladat za
    //      nej nove prvky.
    //   2) Pote ho vypojim ze seznamu
    //
    // Oznaceni prvku za smazany muzete provest pomoci atomickych operaci tak, ze ukazatel
    // na naslednika oznacite bitovym priznakem (schematicky napr., current->next | 1ULL).
    //
    // Pro jednoduchost nemusite resit dealokaci pameti po odebrani prvku.
    bool remove(long long value) {
        throw "Not implemented yet";
    }
};

#endif
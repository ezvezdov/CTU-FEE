#ifndef LOCKFREE_H
#define LOCKFREE_H

#include <vector>
#include <iostream>
#include <atomic>

class Lockfree {
public:
    class Node {
    public:
        unsigned long long value;
        std::atomic<Node*> next{nullptr};

        Node(unsigned long long value) : value(value) {}
    };

    Node* head = new Node(999999999999UL);

    void insert(unsigned long long value) {
        // 1. alokuju node
        Node* new_node = new Node(value);

        Node* current = head;
        while (true) {
            // prochazim linked list podobne jako u seriove varianty
            Node* next = current->next;
            if (next != nullptr && next->value <= value) {
                // posouvam s k dalsimu nodu
                current = current->next;
            } else {
                // nasel jsem spravne misto pro insert
                // 2. nastavim ->next sveho nodu na `current->next`
                new_node->next = next;
                // 3. zkusim provest `current->next = new_node`, ale jen pokud je v momente
                //  prirazeni porad `current->next == next`; pokud jsem uspel, mam hotovo;
                //  potrebuji zopakovat cely loop od zacatku, protoze v pripade, ze ma nove pridany
                //  node mensi hodnotu nez `value`, tak se musim posunout az za nej
                if (current->next.compare_exchange_strong(next, new_node)) {
                    break;
                }
                // jinak zkusim cely proces od aktualniho nodu zopakovat
            }
        }
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
    bool remove(unsigned long long value) {
        throw "Not implemented yet";
    }
};

#endif
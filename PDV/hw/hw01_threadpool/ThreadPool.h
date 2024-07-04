//#ifndef PRODUCENTCONSUMER_THREADPOOL_H
//#define PRODUCENTCONSUMER_THREADPOOL_H
//
//#include <list>
//#include <thread>
//#include <vector>
//#include <iostream>
//#include <condition_variable>
//
//template<typename job_t, typename worker_t>
//class ThreadPool {
//private:
//    // Fronta uloh
//    std::list<job_t> queue;
//
//    // Vlakna konzumentu zpracovavajicich ulohy
//    std::vector<std::thread> threads;
//
//    // Funkce, kterou maji konzumenti vykonavat
//    const worker_t & worker;
//
//    // Mutex for accessing queue
//    std::mutex mtx_q;
//
//    // Conditional variable to avoid busy waiting
//    std::condition_variable cv;
//
//public:
//    ThreadPool(const unsigned int num_threads, const worker_t & worker);
//    void process(const job_t job);
//    void join();
//
//};
//
//
//template<typename job_t, typename worker_t>
//ThreadPool<job_t, worker_t>::ThreadPool(const unsigned int num_threads, const worker_t &worker) : worker(worker) {
//    // Zde vytvorte "num_threads" vlaken konzumentu:
//    //   - Po spusteni bude vlakno kontrolovat, zda je ve fronte uloh "queue" nejaka
//    //     uloha ke zpracovani, tj., fronta neni prazdna - !queue.empty()
//    //   - Ve chvili, kdy je ve fronte uloha ke zpracovani, tak ulohu z fronty vyjme
//    //     a zavola worker(task), kde task je uloha ke zpracovani (priklad pouziti
//    //     naleznete nize).
//    //   - Vlakno se ukonci pokud uloha ke zpracovani je 0
//    //   - Vytvorena vlakna vlozte do pole "threads"
//
//    bool exit = false;
//
//    for(int i = 0; i < num_threads; i++) {
//
//        threads.push_back(std::thread([&]() {
//
//            std::unique_lock<std::mutex> lck(mtx_q);
//
//            while (!exit) {
//                job_t job = -1;
//
//                if (!queue.empty()) {
//                    job = queue.front(); // Get first job in queue
//                    queue.pop_front(); // Remove from queue
//                }
//
//                if(job == -1){
//                    cv.wait(lck); // waiting for (adding smth to queue)/(end of work)
//                    continue;
//                }
//
//                lck.unlock();
//                cv.notify_all();
//
//                if (job == 0){
//                    exit = true;
//                    cv.notify_all();
//                    break;
//                }  // Pokud je "zpracovana" uloha 0, skoncime
//
//                worker(job); // Na zaver zavolame "worker" funkci, ktera ulohu vykona
//                lck.lock();
//            }
//        }));
//    }
//
//    // Tento kod nesplnuje zadani z nekolika duvodu:
//    //   - Spousti pouze jedno vlakno konzumenta. Pokud vykonani uloh pomoci worker(task)
//    //     trva delsi dobu a konzument nestiha ulohy zpracovavat, zacnou se nam ulohy ve
//    //     fronte hromadit. Vypocet muzeme zrychlit tim, ze si vice konzumentu podeli o
//    //     ulohy ke zpracovani
//    //   - Pokud vice uloh pristupuje ke fronte uloh, vysledek neni predvidatelny. Muze
//    //     se nam pak stat, ze se jedna uloha zpracuje vicekrat, nebo naopak na nejakou
//    //     ulohu zapomeneme. Zajistete vylucny pristup ke fronte uloh "queue" za pouziti
//    //     mutexu.
//
//    // Nezapomente, ze v kriticke sekci chceme travit co nejmene casu (tj., uvolnovat zamek
//    // ihned pote, co uz ho prestaneme potrebovat). Napriklad si dejte pozor na to, abyste
//    // zamek uvolnili pred tim, nez spustite vypocet ulohy pomoci worker(task). Pokud by
//    // totiz vypocet ulohy byl soucasti kriticke sekce, mohla by se v danou chvili pocitat
//    // pouze jedna uloha - a prisli bychom o vyhody paralelizace.
//}
//
//template<typename job_t, typename worker_t>
//void ThreadPool<job_t, worker_t>::process(const job_t job) {
//    // Bezpecne vlozte ulohu "job" do fronty uloh "queue"
//
//    mtx_q.lock();
//    queue.push_back(job);
//    mtx_q.unlock();
//
//    cv.notify_all();
//
//}
//
//// Tato metoda nam umozni volajici funkci v main.cpp pockat na vsechna spustena vlakna konzumentu
//template<typename job_t, typename worker_t>
//void ThreadPool<job_t, worker_t>::join() {
//    for(unsigned int i = 0 ; i < threads.size() ; i++) threads[i].join();
//}
//
//#endif //PRODUCENTCONSUMER_THREADPOOL_H


#ifndef PRODUCENTCONSUMER_THREADPOOL_H
#define PRODUCENTCONSUMER_THREADPOOL_H

#include <list>
#include <thread>
#include <vector>
#include <iostream>
#include <condition_variable>

template<typename job_t, typename worker_t>
class ThreadPool {
private:
    // Fronta uloh
    std::list<job_t> queue;

    // Vlakna konzumentu zpracovavajicich ulohy
    std::vector<std::thread> threads;

    // Funkce, kterou maji konzumenti vykonavat
    const worker_t & worker;

    // Mutex for accessing queue
    std::mutex mtx_q;

    // Conditional variable to avoid busy waiting
    std::condition_variable cv;

public:
    ThreadPool(const unsigned int num_threads, const worker_t & worker);
    void process(const job_t job);
    void join();

};


template<typename job_t, typename worker_t>
ThreadPool<job_t, worker_t>::ThreadPool(const unsigned int num_threads, const worker_t &worker) : worker(worker) {
    // Zde vytvorte "num_threads" vlaken konzumentu:
    //   - Po spusteni bude vlakno kontrolovat, zda je ve fronte uloh "queue" nejaka
    //     uloha ke zpracovani, tj., fronta neni prazdna - !queue.empty()
    //   - Ve chvili, kdy je ve fronte uloha ke zpracovani, tak ulohu z fronty vyjme
    //     a zavola worker(task), kde task je uloha ke zpracovani (priklad pouziti
    //     naleznete nize).
    //   - Vlakno se ukonci pokud uloha ke zpracovani je 0
    //   - Vytvorena vlakna vlozte do pole "threads"

    // We should pop ALL zeros from the queue (producent adds `num_threads` zeros to queue as END OF WORK)

    for(int i = 0; i < num_threads; i++) {

        threads.push_back(std::thread([&]() {
            while (true) {
                job_t job = -1;
                std::unique_lock<std::mutex> lck(mtx_q); // equal mtx_q.lock()

                if (!queue.empty()) {
                    job = queue.front(); // Read the job from front of queue
                    queue.pop_front(); // Pop job that we read
                }

                // Check if end of work
                if(job == 0){
                    lck.unlock();
                    break;
                }

                // Queue is empty, wait until full (cv.notification)
                if(job == -1){
                    cv.wait(lck);
                    lck.unlock();
                    continue;
                }

                lck.unlock();
                cv.notify_all();

                worker(job); // Working...
            }
        }));
    }
    // Tento kod nesplnuje zadani z nekolika duvodu:
    //   - Spousti pouze jedno vlakno konzumenta. Pokud vykonani uloh pomoci worker(task)
    //     trva delsi dobu a konzument nestiha ulohy zpracovavat, zacnou se nam ulohy ve
    //     fronte hromadit. Vypocet muzeme zrychlit tim, ze si vice konzumentu podeli o
    //     ulohy ke zpracovani
    //   - Pokud vice uloh pristupuje ke fronte uloh, vysledek neni predvidatelny. Muze
    //     se nam pak stat, ze se jedna uloha zpracuje vicekrat, nebo naopak na nejakou
    //     ulohu zapomeneme. Zajistete vylucny pristup ke fronte uloh "queue" za pouziti
    //     mutexu.

    // Nezapomente, ze v kriticke sekci chceme travit co nejmene casu (tj., uvolnovat zamek
    // ihned pote, co uz ho prestaneme potrebovat). Napriklad si dejte pozor na to, abyste
    // zamek uvolnili pred tim, nez spustite vypocet ulohy pomoci worker(task). Pokud by
    // totiz vypocet ulohy byl soucasti kriticke sekce, mohla by se v danou chvili pocitat
    // pouze jedna uloha - a prisli bychom o vyhody paralelizace.
}

template<typename job_t, typename worker_t>
void ThreadPool<job_t, worker_t>::process(const job_t job) {
    // Bezpecne vlozte ulohu "job" do fronty uloh "queue"

    // Locking queue to access the queue
    mtx_q.lock();

    // add job
    queue.push_back(job);

    // Unlocking queue
    mtx_q.unlock();

    // Notify about adding job to the queue
    cv.notify_all();
}

// Tato metoda nam umozni volajici funkci v main.cpp pockat na vsechna spustena vlakna konzumentu
template<typename job_t, typename worker_t>
void ThreadPool<job_t, worker_t>::join() {
    for(unsigned int i = 0 ; i < threads.size() ; i++) threads[i].join();
}

#endif //PRODUCENTCONSUMER_THREADPOOL_H

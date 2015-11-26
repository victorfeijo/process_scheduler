/*
 * File:   Scheduler.cpp
 * Author: José Victor Feijó de Araujo
 *
 * Created on September 27, 2015, 11:19 AM
 */

#include "Debug.h"
#include "Scheduler.h"

Scheduler::Scheduler() {
    _readyQueue = new Scheduling_Queue();
}

Thread* Scheduler::choosen() {
    return _choosen;
}

void Scheduler::insert(Thread* thread) {
    this->_readyQueue->insert(thread);
}

Thread* Scheduler::remove(Thread* thread) {
    // remove a specific thread from the readyQueue
    this->_readyQueue->remove(thread);
}

/*
 * Threads podem solicitar ao escalonador para escolher outra thread para executar, invocando o método
 * Thread* Scheduler::choose(), que deve ser implementado. A implementação desse método deve verificar se há alguma
 * thread na fila de threads prontas para executar. Se houver, deve retornar o primeiro elemento dessa fila, excluindo-o
 * da fila. Se a fila estiver vazia, deve retornar nulo.
 */
Thread* Scheduler::choose() {
    Debug::cout(Debug::Level::trace, "Scheduler::choose()");
    // INSERT YOUR CODE HERE
    // ...
    if (this->_readyQueue->empty()) {
        return nullptr;
    } else {
        Thread* next = this->_readyQueue->top();
        //Não retirar mais a thread escolhida da fila de prontos  no método choose() quando a thread a escolhida.
        //Isso depende da implemetação de vocês, mas deve ser simplesmente comantar a lnha que retira  a cabça da lista, como:
        //this->_readyQueue->pop();
        return next;
    }
}

/*
 * Quando o algoritmo de escalonamento usado for de prioridade dinâmica ou então quando algum mecanismo do tipo aging
 * for usado, pode ser necessário alterar algum atributo das threads que estão na fila de prontos e reordenar essa fila
 * conforme os novos valores dos atributos. Nesses casos, isso pode ser feito quando a fila de threads prontas for alterada,
 * invocando o método void Scheculer::reschedule(), que precisa ser implementado. Esse método precisa varrer a fila
 * de pronto ajustando os atributos necessários das threads na fila, possivelmente tendo que removê-las da fila e depois
 * reinseri-las para que o ordenamento ocorra, o que depende da estrutura de dados da fila sendo usada.
 */
void Scheduler::reschedule() {
    Debug::cout(Debug::Level::trace, "Scheduler::reschedule()");
    if (!this->_readyQueue->empty()) {
        // INSERT YOUR CODE HERE
        // ...
        //Criando uma lista nova e inserindo em ordem todos os elementos, substituindo a fila velha
        Scheduling_Queue* newQueue = new Scheduling_Queue();
        while(!(this->_readyQueue->empty())) {
            newQueue->insert(this->_readyQueue->top());
            this->_readyQueue->pop();
        }
        delete this->_readyQueue;
        this->_readyQueue = newQueue;
    }
}

/*
 * File:   Process.cpp
 * Author: José Victor Feijó de Araujo
 *
 * Created on August 31, 2015, 7:49 AM
 */

#include "Debug.h"
#include "Process.h"
#include "Thread.h"
#include "System.h"
#include "Simulator.h"
#define PARENT_ID 0

Process::Process(unsigned int parentId) {
    this->_entity = Simulator::getInstance()->getEntity(); // simulation purposes only
    this->_entity->setPointer(this); // simulation purposes only
    
    this->_idInfo._processId = Process::getNewId();
    this->_idInfo._parentId = parentId;
    this->_idInfo._priority = Simulator::generate_uniform_distribution(0, 10);
    // random memory info about the new process
    this->_memInfo._segments = new std::list<MemorySegment*>();
    unsigned int numSegments = Simulator::generate_uniform_distribution(2, 5);
    for (unsigned int i = 0; i < numSegments; i++) {
        MemorySegment* segment = new MemorySegment(Simulator::generate_uniform_distribution(250 / (numSegments + 3), 250 / numSegments));
        _memInfo._segments->insert(_memInfo._segments->begin(), segment);
    }    
    // INSERT YOUR CODE HERE
    // ...
}
Process::Process(const Process& orig) {
}

Process::~Process() {
}

unsigned int Process::getParentId() const {
    return _idInfo._parentId;
}

unsigned int Process::getId() const {
    return _idInfo._processId;
}

/**
 * Processos chegarão ao sistema quando for invocada a chamada static Process* Process::exec(), que precisa
 * ser implementada. A implementação dessa chamada de sistema deve criar um Process, alocar memória para ele invocando
 * a chamada de sistema Partition* MemoryManagement::alloc(Process* process), inicializar seus atributos, colocá-lo na lista
 * de processos no sistema e criar uma thread (correspondente a "main") para esse processo invocando a chamada
 * static Thread* Thread::thread_create(Process* parent). O metodo deve retornar o processo criado.
 **/
Process* Process::exec() { /*static*/
    Debug::cout(Debug::Level::trace, "Process::exec()");
    // INSERT YOUR CODE HERE (just complete what is missing... easy this time...)

    // criar um Process
    Process* newProcess = new Process(PARENT_ID);

    // alocar memória para ele
    std::list<MemorySegment*>* segments = newProcess->_memInfo._segments;
    Partition* partition = System::memoryManager()->allocateMemory(newProcess->getSize(segments));
    newProcess->_memInfo._partition = partition;

    // inicializar seus atributos
    //

    // colocá-lo na lista de processos no sistema
    Process::getProcessesList()->push_back(newProcess);

    // criar uma thread (correspondente a "main") para esse processo
    // ...
    Thread::thread_create(newProcess);
    
    // retornar o método criado
    return newProcess;
}

/**
 * Processos serão finalizados quando for invocada a chamada static void Process::exit(int status = 0), que precisa ser
 * implementada. A implementação dessa chamada deve desalocar a memória do processo invocando
 * void MemoryManagement::desalloc(Process* process), excluir todas as threads desse processo, excluir o processo (destruir
 * o objeto Process), invocar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose() e então
 * o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando
 * void Dispatcher::dispatch(Thread* previous, Thread* next).
 **/
void Process::exit(int status) { /*static*/
    Debug::cout(Debug::Level::trace, "Process::exit(" + std::to_string(status) + ")");
    // INSERT YOUR CODE HERE (Now it's up to you...)
    // ...
    //pegar a thread running e deletar o processo referente a ela
    Thread* mainThread = Thread::running();
    Process* process = mainThread->getProcess();
    //System::memoryManager()->deallocateMemory(process);
    //dar exit em cada thread do relacionada ao processo
    std::list<Thread*>* threads = Thread::getThreadsList();
    std::list<Thread*>::iterator iterator;
    std::list<Thread*> killedThreads;
    //necessidade de fazer um for para uma fila de Thread a serem Excluidas
    for (iterator = threads->begin(); iterator != threads->end(); ++iterator) {
        Thread* actualThread = (*iterator);
        if (actualThread->getProcess()->_idInfo._processId == process->_idInfo._processId) {
            killedThreads.push_back(actualThread);
        }
    }
    //enquanto essa fila de threads do processo nao tiver vazia, excluir uma por uma
    while(!(killedThreads.empty())) {
        Thread* killed = (*(killedThreads.begin()));
        killedThreads.pop_front();
        if (killed->_state == Thread::State::READY) {
            System::scheduler()->remove(killed);
        }
        Thread::getThreadsList()->remove(killed);
        delete killed;
    }
    delete process;
    Thread* next = System::scheduler()->choose();
    Thread::dispatch(nullptr, next);
}
// Calcula o tamanho do processo a partir da lista de segmentos de memória
unsigned int Process::getSize(std::list<MemorySegment*>* segments) {
    std::list<MemorySegment*>::iterator iterator;
    unsigned int sum = 0;
    for (iterator = segments->begin(); iterator != segments->end(); ++iterator) {
        sum = sum + (*iterator)->getSize();
    }
    return sum;
}


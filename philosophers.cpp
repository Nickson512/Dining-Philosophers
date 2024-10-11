#include <iostream>
#include <thread> 
#include <mutex> 
#include <chrono>


using namespace std;

class philosopher {
    private:
        const int MAX_HUNGER_METER = 100;
        int hungerMeter = 100;
        const int CHANGE_AMOUNT = 5;
        const int EAT_TIME = 3;
        const int THINK_TIME = 6;
        bool isAlive = true;
    public:
        int getHunger (void) {
            return hungerMeter;
        } 
        void eat (void) { //will fill hunger meter a set amount
            if (hungerMeter < (MAX_HUNGER_METER - CHANGE_AMOUNT)){ //guarantees the max will never be exceeded
                hungerMeter += CHANGE_AMOUNT;    
            }
            else {
                hungerMeter = 100;
            }
            return;
        }
        
        bool think (void) { //will deplete the hunger meter a set amount
            hungerMeter -= CHANGE_AMOUNT;
            if (hungerMeter <= 0) {
                isAlive = false;
            }
            return isAlive;
            
        }
        
        void getForks (fork forkOne, fork forkTwo) { //try to acquire 2 adjacent forks to begin eating
                                                     //TODO: figure out ifndef so compiler doesn't complain about undefined fork
        }
        
};

class fork {
    private:
        mutex usage;
    public:
        void useFork (void) {
            usage.lock();
            return;
        }
        
        void doneFork (void) {
            usage.unlock();
            return;
        }
        
        int isLocked (void) {
            int x = try_lock(usage);
            return x;
        }
};

int main (void) {
    philosopher Aristotle;
    cout<<Aristotle.getHunger();
    Aristotle.eat();
    cout<<Aristotle.getHunger();
    return 0;
}

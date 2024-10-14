#include <iostream>
#include <thread> 
#include <mutex> 
#include <chrono>


using namespace std;

class fork {
    private:
        mutex usage;
	bool locked = false; 
    public:
        void useFork (void) {
            usage.lock();
	    locked = true;
            return;
        }
        
        void doneFork (void) {
            usage.unlock();
	    locked = false;
            return;
        }
        
        bool isLocked (void) {
            //int x = try_lock(usage); << this function attempts to lock the mutex; should not be used for this
            return locked;
        }
};

class philosopher {
    private:
        const int MAX_HUNGER_METER = 100;
        int hungerMeter = 100;
        const int CHANGE_AMOUNT = 5;
        const int EAT_TIME = 3;
        const int THINK_TIME = 6;
        bool isAlive = true;
	bool hasForks = false;
    public:
        int getHunger (void) {
            return hungerMeter;
        } 
        void eat (void) { //will fill hunger meter a set amount
	    if (hasForks) {
		std::this_thread::sleep_for(std::chrono::seconds(EAT_TIME));
                if (hungerMeter < (MAX_HUNGER_METER - CHANGE_AMOUNT)){ //guarantees the max will never be exceeded
                    hungerMeter += CHANGE_AMOUNT;    
                }
                else {
                    hungerMeter = MAX_HUNGER_METER;
                }
	    }
            return;
        }
        
        bool think (void) { //will deplete the hunger meter a set amount
	    std::this_thread::sleep_for(std::chrono::seconds(THINK_TIME));
	    hungerMeter -= CHANGE_AMOUNT;
            if (hungerMeter <= 0) {
                isAlive = false;
            }
            return isAlive;
            
        }
        
        void tryForks (fork forkOne, fork forkTwo) { //try to acquire 2 adjacent forks to begin eating
	    if (!(forkOne.isLocked || forkTwo.isLocked) && !hasForks) { //TODO make sure forks are passed by reference
	        forkOne.useFork;
		forkTwo.useFork;
		hasForks = true;
	    }
	    return;
	        
        }
	
        
};

int main (void) {
    philosopher Aristotle;
    cout<<Aristotle.getHunger()<<endl;
    Aristotle.think();
    cout<<Aristotle.getHunger();
    return 0;
}

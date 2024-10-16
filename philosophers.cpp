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
        bool permissionForks = false;
    public:
        int getHunger (void) {
            return hungerMeter;
        } 
        void eat (void) { //will fill hunger meter a set amount and give up forks
        if (hasForks) {
        std::this_thread::sleep_for(std::chrono::seconds(EAT_TIME));
                if (hungerMeter < (MAX_HUNGER_METER - CHANGE_AMOUNT)){ //guarantees the max will never be exceeded
                    hungerMeter += CHANGE_AMOUNT;    
                }
                else {
                    hungerMeter = MAX_HUNGER_METER;
                }
                //Forks are now done being used
            }
            return;
        }
        
        void think (void) { //will deplete the hunger meter a set amount
        std::this_thread::sleep_for(std::chrono::seconds(THINK_TIME));
        hungerMeter -= CHANGE_AMOUNT;
            if (hungerMeter <= 0) {
                isAlive = false;
            }
            return;
            
        }
        
        bool tryForks (fork& forkOne, fork& forkTwo) { //try to acquire the adjacent forks to begin eating
            bool gotForks = false;
            if (!(forkOne.isLocked() || forkTwo.isLocked()) && !hasForks) { 
                forkOne.useFork();
                forkTwo.useFork();
                gotForks = true;
            }
        return gotForks;
            
        }
        
        void setPermission (void) {
            permissionForks = true;
        }
        
        void action (fork& forkOne, fork& forkTwo) { //The lifecycle of the philosopher. As long as it's alive it'll try to eat or think if it can't
            while (isAlive) {
                if (permissionForks) {
                    hasForks = tryForks(forkOne, forkTwo);
                    if (hasForks) {
                        eat();
                        hasForks = false;
                        forkOne.doneFork();
                        forkTwo.doneFork();
                    }
                    permissionForks = false;
                }
                else {
                    think();
                }
            }
            return;
        }
};

class waiter{
    private:
    string nameOne;
    string nameTwo;
    string nameThr;
    string nameFour;
    string nameFive;
    public:
    void giveName (string one, string two, string thr, string four, string five) { //get the names of the five philosophers
        nameOne = one;
        nameTwo = two;
        nameThr = thr;
        nameFour = four;
        nameFive = five;
        return;
    }
    
    
}

int main (void) {
    philosopher Aristotle;
    cout<<Aristotle.getHunger()<<endl;
    Aristotle.think();
    cout<<Aristotle.getHunger();
    return 0;
}

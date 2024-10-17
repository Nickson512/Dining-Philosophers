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
        const int EAT_TIME = 2;
        const int THINK_TIME = 6;
        bool isAlive = true;
        bool hasForks = false;
        bool permissionForks = false;
    public:
        int getHunger (void) {
            return hungerMeter;
        } 

	bool getPermission (void) {
		return permissionForks;
	}

	bool getAlive () {
		return isAlive;
	}

        void eat (void) { //will fill hunger meter a set amount and give up forks
        if (hasForks) {
		while (hungerMeter < MAX_HUNGER_METER) {//Philosophers will gorge themselves until they are full. 
        		std::this_thread::sleep_for(std::chrono::seconds(EAT_TIME));
                	if (hungerMeter < (MAX_HUNGER_METER - CHANGE_AMOUNT)){ //guarantees the max will never be exceeded
                    	hungerMeter += CHANGE_AMOUNT;    
                	}
                	else {
                    	hungerMeter = MAX_HUNGER_METER;
                	}
        	}
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
/*
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
*/

void invokePhilosopher (philosopher& phil, fork& forkOne, fork& forkTwo) {
	//this function allows the invokation of a philosopher's action in the declaration of a thread
	phil.action(forkOne, forkTwo);
	return;

}	

int main (void) {
    philosopher nerds[5];
    fork forks[5];
    int hungriest = 0;
    int secondary = 0;
    int hungerMin;
    bool eaters;
    bool nobodyDied = true;
    thread thread1(invokePhilosopher, std::ref(nerds[0]), std::ref(forks[4]), std::ref(forks[0]));//Reference wrappers pass by reference
    thread thread2(invokePhilosopher, std::ref(nerds[1]), std::ref(forks[0]), std::ref(forks[1]));
    thread thread3(invokePhilosopher, std::ref(nerds[2]), std::ref(forks[1]), std::ref(forks[2]));
    thread thread4(invokePhilosopher, std::ref(nerds[3]), std::ref(forks[2]), std::ref(forks[3]));
    thread thread5(invokePhilosopher, std::ref(nerds[4]), std::ref(forks[3]), std::ref(forks[4]));
    
	while(nobodyDied) {
		//the waiter. the waiter chooses 2 philosophers to eat when nobody is.
		hungerMin = 100;
		eaters = false;
		hungriest = 0;
		secondary = 3;
		for (int i = 0; i < 5; i++) {
			if (!nerds[i].getPermission() && (nerds[i].getHunger()<hungerMin)) {
				hungerMin = nerds[i].getHunger();
				hungriest = i;
			}
			if (nerds[i].getPermission()) {
				eaters = true;
				cout<<"Philosopher "<<i+1<<" is gonna eat."<<endl;
			}
			if (!nerds[i].getAlive()) {
				nobodyDied=false;
				cout<<"Philosopher "<<i+1<<" just died."<<endl;
				break;
			}
			cout<<"Philosopher "<<i+1<<" hunger meter:"<<nerds[i].getHunger()<<endl;
		}
		if (!eaters) {
			nerds[hungriest].setPermission();
			if (hungriest > 2) {
				secondary = (hungriest+2)%4 - 1;
			}
			else {
				secondary = hungriest +2; 
			}
			if (secondary == 4) {
				if (nerds[4].getHunger() < nerds[0].getHunger()) {
					nerds[4].setPermission();
				}
				else {
					nerds[0].setPermission();
				
				}
			}
			else {
				if (nerds[secondary].getHunger() < nerds[secondary+1].getHunger()){
					nerds[secondary].setPermission();
				}
				else {
					nerds[secondary+1].setPermission();
				
				}

			}
			
		}
        	std::this_thread::sleep_for(std::chrono::seconds(1));
		cout<<endl;
		
	}

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    cout<<"Finished."<<endl; 
	

    /*
     *          o
     *       x     x 
     *     o         o
     *      x       x
     *       o  x  o
     *
     */


    return 0;
}

#include<iostream>
using namespace std;

class process{
    private:
        int p;
        int *burst;
        int *arrival;
        int *waiting;

    public:
        process(){
            p=0;
        }
        
        void displayData(){
            cout<<"The process data is as following - \n";
            cout<<"Process ID\t"<<"Arrival Time\t"<<"Burst Time\n";
            for(int i=0; i<p; i++){
                cout<<"   "<<i+1<<"\t\t  "<<arrival[i]<<"\t\t  "<<burst[i]<<"\n";
            }
            cout<<endl<<endl;
        }

        void inputData(){
            cout<<"How many Processes are there to be executed?\n";
            cin>>p;
            arrival = new int[p];
            burst = new int[p];
            waiting = new int[p];
            cout<<"Enter the arrival time for each process\n";
            for(int i=0; i<p; i++){
                cin>>arrival[i];
            }
            cout<<"Enter the burst time for each process\n";
            for(int i=0; i<p; i++){
                cin>>burst[i];
            }
            cout<<endl<<endl;

            displayData();
        }

        void calculateWaitingTime(){
            waiting[0] = 0;
            cout<<"ProcessID\t"<<"Waiting Time\n";
            cout<<"   "<<1<<"\t\t\t"<<0<<endl;
            for(int i=1; i<p; i++){
                waiting[i] = burst[i-1]+waiting[i-1];
                // condition to check if the cpu is in ideal mode 
                if(arrival[i]>waiting[i]){
                    waiting[i] = arrival[i];
                }
                cout<<"   "<<i+1<<"\t\t\t"<<waiting[i]<<endl; 
            }   
            cout<<endl<<endl;
        }
};

int main(){
    process p;
    p.inputData();
    p.calculateWaitingTime();
    return 0;
}
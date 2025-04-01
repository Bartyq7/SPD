#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <queue>
#include <thread>

class Task{
public:
    int j;
    int pj;
    int rj;
    int qj;
};

bool compareTasksJ(Task& a, Task & b){
    return a.j<b.j;
}
bool compareTasksRj(Task& a, Task & b){
    return a.rj>b.rj;
}
bool compareTasksQj(Task& a, Task & b){
    return a.qj<b.qj;
}





class Problem{
    std::vector<Task> task_instance;
    int num_of_tasks=5;
    int C_max;
    public:
    void set_num_of_tasks(int number){
        num_of_tasks=number;
    }
   std::vector<Task> solution;

    // void generate_instance(){
    //     for(int i =0; i < num_of_tasks; i++){
    //         Task t1;
    //         t1.j = i+1;
    //         t1.pj = ( std::rand() % 15 )+1;
    //         t1.rj = ( std::rand() % 15 );
    //         t1.qj = ( std::rand() % 15 )+1;
    
    //         //std::cout<<t1.pj<<std::endl;
    //         task_instance.push_back(t1);
    //     }
    // }


    Task get_task(int x){
        return task_instance[x];
    }

    void delete_task(){
        task_instance.pop_back();
    }
    
    std::vector<Task> get_inst(){
        return task_instance;
    }

    void print_tasks(){
        std::cout<<"j: \t rj \t pj \t qj\n\n";
        for(int i=0; i < num_of_tasks; i++){
            std::cout<< task_instance[i].j << " \t "<<task_instance[i].rj << " \t "<<task_instance[i].pj << " \t "<< task_instance[i].qj<<std::endl;
        }
    }

    void print_solution(){
        for(int i=0; i < num_of_tasks; i++){
            std::cout<< solution[i].j<<",  ";
        }
        std::cout<<std::endl;
    }

    int calculate_Cmax(){
        int Cmax=0;
        if(num_of_tasks>=1000)
            std::cout<<"Wariacie! Wlasnie obliczasz Cmaxa dla pliku, ktory ma 1000 instancji czy Ciebie nie popierniczylo?"<<std::endl;
        for(int i=0; i<num_of_tasks;i++){
            Cmax=std::max(Cmax, task_instance[i].rj)+task_instance[i].qj+task_instance[i].pj;

        }
        return Cmax;
    }

    
    void sort_pj(){
        std::sort(task_instance.begin(), task_instance.end(), [](const Task& a, const Task& b){return  a.pj<b.pj;});
    }


    void sort_rj(){
        std::sort(task_instance.begin(), task_instance.end(), [](const Task& a, const Task& b){return  a.rj<b.rj;});
    }


     void read_from_file(std::ifstream& file){
        char buf;
        int bufor=0;
        for(int i =0; i < num_of_tasks; i++){
            Task t1;
            t1.j = i+1;
            file>>t1.rj>>t1.pj>>t1.qj;
            task_instance.push_back(t1);
        }
    }   



    void next_perm_fun(){
        int Cmax=0;
        int Cmin=INT_MAX;
        int bufor=0;

        do {
            int Cmax=0;
            int bufor=0;
            for(auto &x : task_instance){
                bufor=std::max(bufor, x.rj)+x.pj;
                Cmax=std::max(bufor +x.qj, Cmax);
            }
            if(Cmin>Cmax){
                Cmin=Cmax;
                solution=task_instance;
            }

        } while(next_permutation(task_instance.begin(), task_instance.end(), compareTasksJ));
        std::cout<<Cmin<<" <- Cmin"<<std::endl;
    }



    void schrage_fun(){
        std::priority_queue<Task, std::vector<Task>, decltype(&compareTasksRj)> moundQueue(compareTasksRj);
        std::priority_queue<Task, std::vector<Task>, decltype(&compareTasksQj)> moundByQj(compareTasksQj);
        int time=0;
        int Cmax=0;
        int comparsion;
        int last_qj=0;
        int lastValue=0;
        int value;
        std::vector<Task> bufor;
        for(auto& x : task_instance){          
            moundQueue.push(x);
        }

        while(!moundQueue.empty() || !moundByQj.empty()){
           // std::this_thread::sleep_for(std::chrono::seconds(1));
            while(!moundQueue.empty() && moundQueue.top().rj<=time){
                //std::cout<<"rj<=time, rj: "<< moundQueue.top().rj<<" czas: "<<time<<std::endl;
                moundByQj.push(moundQueue.top());
                moundQueue.pop();
            }
            if(moundByQj.empty()){
                time=moundQueue.top().rj;
                //std::cout<<"ByQ jest empty i czas ustawia na: "<<time<<std::endl;
            }
            else{
               // std::cout<<"jest w else i aktualny czas na poczatku to: "<<time;
                solution.push_back(moundByQj.top());
                time=time+moundByQj.top().pj;
                Cmax=std::max(Cmax,time+moundByQj.top().qj);
                moundByQj.pop();
                //std::cout<<" a na koncu czas: "<<time<<" Cmax: "<<Cmax<<std::endl;

            }
        }
        std::cout<<"Cmax -> "<<Cmax<<std::endl;
        //nie udane proby ale szkoda mi je usuwac bo za duzo czasu nad nimi spedzilem wiec niech zasmiecaja kod


        // bufor.push_back(moundQueue.top());
        // moundQueue.pop();
        // moundQueue.push(bufor[0]);

        // while(!moundQueue.empty()){
        //     if(moundQueue.top().rj<=time){
        //         while(1){
        //             if(moundQueue.empty() || moundQueue.top().rj>time){
        //                 time=moundQueue.top().rj;
        //                 break;
        //             }
        //             bufor.push_back(moundQueue.top());
        //             moundQueue.pop();
        //         }
        //         std::sort(bufor.begin(), bufor.end(), compareTasksQj);
        //         for(int counter=0; counter<bufor.size(); counter++){
        //             solution.push_back(bufor[counter]);
        //             time=time+bufor[counter].rj;
        //         }
        //         bufor.clear();
        //     }
        //     else{
        //         time++;
        //     }
        // }
        // std::cout<<"Cmax: "<<time<<std::endl;
    //     while(!moundQueue.empty()){
    //         while(!moundQueue.empty() && moundQueue.top().rj<=time){
    //             bufor.push_back(moundQueue.top());
    //             moundQueue.pop();            
    //         }
    //         if(!bufor.empty()){
    //             std::sort(bufor.begin(), bufor.end(), compareTasksQj);
    //             for(int i=0; i<bufor.size(); i++){

    //                 if(i>0)
    //                     lastValue=lastValue+bufor[i-1].pj;
    //                 else
    //                     lastValue=0;

    //                 value=bufor[i].qj+lastValue;

    //                 if(value>last_qj){
    //                     last_qj=value;
    //                 }
    //             }
    //             while(!bufor.empty()){

    //                 Task current=bufor.front();
    //                 solution.push_back(current);
    //                 time=time+current.pj;
    //                 bufor.erase(bufor.begin());
    //             }
    //         }
    //         else{
    //             ++time;
    //         }

    //     }
    //     time=time+last_qj;
    //     std::cout<<"Cmax: "<<time<<std::endl;
    }




};



int get_number_of_tasks(std::ifstream& cosik){
    std::string first_lane;
    int numer_of_tasks;


    if(std::getline(cosik, first_lane)){
        numer_of_tasks=stoi(first_lane);
    }
    else{
        std::cout<<"Wariacie! Jesli myslisz, ze mozesz tak o sobie pobrac\npierwsza linijke pliku to jestes w bledzie!"<<std::endl;
        return -1;
    }
    return numer_of_tasks;
}


int main(){
    srand(time(NULL));
    int console=2;
    /*##########################################                
    ###    WCZYTANIE DANYCH/CONFIG OBIEKTU   ###
    ##########################################*/ 
    std::ifstream file("SCHRAGE2.dat");
    if (!file.is_open()) {
        std::cerr << "Blad: Nie udalo sie otworzyc pliku!" << std::endl;
        return 1;
    }
    Problem p1;
    p1.set_num_of_tasks(get_number_of_tasks(file));   // <- dziala igla
    p1.read_from_file(file);

    p1.print_tasks();


    /*###########################
    ###    WYBOR ALGORYTMU    ###
    ###########################*/
    // std::cout<<"Wybierz algorytm:\n1 - Przeglad calkowity\n2 - Schrage\n";
    // std::cin>>console;
    auto start = std::chrono::high_resolution_clock::now();
    /*##############################
    ###    PRZEGLAD CALKOWITY    ###
    ##############################*/
    if(console==1){
        p1.next_perm_fun();
    }
    /*#####################
    ###    SCHRAGE      ###
    #####################*/
    if(console==2){
        p1.schrage_fun();
    }

// kopiec priority cop
//schrage to stroujesz po rj i pozniej jak masz dwa dostepne (lub wiecej) w tej chwili to porownujesz qj i ten kto ma wiekszego (qj'a) ten wchodzi pierwszy
    auto koniec = std::chrono::high_resolution_clock::now();
    auto zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
        
    std::cout<<"Czas wykonania alogyrtmu: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    p1.print_solution();
    return 0;
}
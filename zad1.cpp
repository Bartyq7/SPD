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
    void taskClear(){
        task_instance.clear();
    }

   std::vector<Task> solution;

    void generate_instance(){
        for(int i =0; i < num_of_tasks; i++){
            Task t1;
            t1.j = i+1;
            t1.pj = ( std::rand() % 15 )+1;
            t1.rj = ( std::rand() % 15 );
            t1.qj = ( std::rand() % 15 )+1;
    
            //std::cout<<t1.pj<<std::endl;
            task_instance.push_back(t1);
        }
    }


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


    void sort_rj_fun(){
        sort_rj();
        int Cmax=calculate_Cmax();
        std::cout<<"Cmax: "<<Cmax<<std::endl;
    }

    void sort_qj_fun(){
        sort_qj();
        int Cmax=calculate_Cmax();
        std::cout<<"Cmax: "<<Cmax<<std::endl;
    }

    void sort_qj(){
        std::sort(task_instance.begin(), task_instance.end(), [](const Task& a, const Task& b){return  a.qj<b.qj;});
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


        //nie udane proby ale szkoda mi je usuwac bo za duzo czasu nad nimi spedzilem wiec niech zasmiecaja kod xd
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
    void print_tasks_debug(const std::vector<Task>& tasks) {
        std::cout << "j: \t rj \t pj \t qj\n\n";
        for (int i = 0; i < tasks.size(); ++i) {
            std::cout << tasks[i].j << " \t "
                      << tasks[i].rj << " \t "
                      << tasks[i].pj << " \t "
                      << tasks[i].qj << std::endl;
        }
    }
    int select_and_remove_min_pj_qj(int& Cmax) {
        int min_index = -1;
        int min_sum = INT_MAX;
        // szuka minimalnego czasu z tych ktore maja mniejszego rj od Cmax
        for (int i = 0; i < task_instance.size(); ++i) {
            if (task_instance[i].rj <= Cmax) {
                int sum = task_instance[i].pj + task_instance[i].qj;
                if (sum < min_sum) {
                    min_sum = sum;
                    min_index = i;
                }
            }
        }
        //Jesli zadne nie ma mniejszego rj od Cmax to szuka tego ktory ma najblizszego rj
        if (min_index == -1) {
            int min_rj = INT_MAX;
            for (int i = 0; i < task_instance.size(); ++i) {
                if (task_instance[i].rj > Cmax && task_instance[i].rj < min_rj) {
                    min_rj = task_instance[i].rj;
                    min_index = i;
                }
            }
        }
        //std::cout<<"Cmax przed "<<Cmax<<std::endl;
        Cmax = Cmax + task_instance[min_index].pj + task_instance[min_index].qj;
        //std::cout<<"aktualne pj"<< task_instance[min_index].pj<<"aktualne qj"<<task_instance[min_index].qj<<std::endl;
        //std::cout<<"Cmax po "<<Cmax<<std::endl;
        task_instance.erase(task_instance.begin() + min_index); 
        //print_tasks_debug(task_instance);
        
        if(task_instance.empty()){
            return -2;
        }
        return min_index;
    }


    //okazuje sie ze to jest algorytm greedy?
    // ^ XD
    void create_algo_fun(){
        sort_rj();
        //print_tasks();
        int Cmax_cr = 0;
        //sortuje po rj  i wybiera najkrotszy i potem zaczyna sie magia
        Cmax_cr = task_instance[0].pj + task_instance[0].qj;
        task_instance.erase(task_instance.begin());

        int result = 0;
        while (result!=-2)
        {
            result = select_and_remove_min_pj_qj(Cmax_cr);
        }
        std::cout<<"Cmax final "<<Cmax_cr<<std::endl;
    }

    int select_and_remove_max_pj_qj(int& Cmax) {
        int max_index = -1;
        int max_sum = INT_MIN;
        // szuka maksymalnego czasu z tych ktore maja mniejszego rj od Cmax
        for (int i = 0; i < task_instance.size(); ++i) {
            if (task_instance[i].rj <= Cmax) {
                int sum = task_instance[i].pj + task_instance[i].qj;
                if (sum > max_sum) {
                    max_sum = sum;
                    max_index = i;
                }
            }
        }
        //Jesli zadne nie ma mniejszego rj od Cmax to szuka tego ktory ma najblizszego rj
        if (max_index == -1) {
            int min_rj = INT_MAX;
            for (int i = 0; i < task_instance.size(); ++i) {
                if (task_instance[i].rj > Cmax && task_instance[i].rj < min_rj) {
                    min_rj = task_instance[i].rj;
                    max_index = i;
                }
            }
        }
        //std::cout<<"Cmax przed "<<Cmax<<std::endl;
        Cmax = Cmax + task_instance[max_index].pj + task_instance[max_index].qj;
        //std::cout<<"aktualne pj"<< task_instance[max_index].pj<<"aktualne qj"<<task_instance[max_index].qj<<std::endl;
        //std::cout<<"Cmax po "<<Cmax<<std::endl;
        task_instance.erase(task_instance.begin() + max_index); 
        //print_tasks_debug(task_instance);
        
        if(task_instance.empty()){
            return -2;
        }
        return max_index;
    }
    void create_algo_fun_2(){
        sort_rj();
        //print_tasks();
        int Cmax_cr = 0;
        //sortuje po rj  i wybiera najkrotszy i potem zaczyna sie magia
        Cmax_cr = task_instance[0].pj + task_instance[0].qj;
        task_instance.erase(task_instance.begin());

        int result = 0;
        while (result!=-2)
        {
            result = select_and_remove_max_pj_qj(Cmax_cr);
        }
        std::cout<<"Cmax final 2 "<<Cmax_cr<<std::endl;
    }



    void schrage_interruption(){
        std::priority_queue<Task, std::vector<Task>, decltype(&compareTasksRj)> moundQueue(compareTasksRj);
        std::priority_queue<Task, std::vector<Task>, decltype(&compareTasksQj)> moundByQj(compareTasksQj);
        int time=0;
        int Cmax=0;
        int comparsion;
        int last_qj=0;
        int lastValue=0;
        int value;
        Task actual{0,0,0,INT_MAX}; // a to nie powinno sie nazywac actual i previous?
        Task next;                  // ^no moze ale wtedy atual=previous i next=actual i chyba to tez nie tak do konca xd
        for(auto& x : task_instance){          
            moundQueue.push(x);
        }

        while(!moundQueue.empty() || !moundByQj.empty()){
           // std::this_thread::sleep_for(std::chrono::seconds(1));
            while(!moundQueue.empty() && moundQueue.top().rj<=time){
                //std::cout<<"rj<=time, rj: "<< moundQueue.top().rj<<" czas: "<<time<<std::endl;
                moundByQj.push(moundQueue.top());
                next=moundQueue.top();
                moundQueue.pop();

                if(next.qj>actual.qj){
                    //std::cout<<"actual pj: "<< actual.pj<<"  czas przed: "<<time <<std::endl;
                    actual.pj=time-next.rj;
                    time = next.rj;
                    //std::cout<<"  czas po: "<<time <<std::endl;
                    if(actual.pj>0){
                        moundByQj.push(actual);
                    }
                }
            }
            if(moundByQj.empty()){
                time=moundQueue.top().rj;
                //std::cout<<"ByQ jest empty i czas ustawia na: "<<time<<std::endl;
            }
            else{
               // std::cout<<"jest w else i aktualny czas na poczatku to: "<<time;
                //solution.push_back(moundByQj.top());
                next=moundByQj.top();
                actual=next;
                //std::cout<<"next.qj: "<<next.qj<<" next.pj: "<<next.pj<<std::endl;
                time=time+moundByQj.top().pj;
                Cmax=std::max(Cmax,time+moundByQj.top().qj);
                //std::cout<<"czas po zmianie: "<<time<<"Cmax:  "<<Cmax<<std::endl;
                moundByQj.pop();
                //std::cout<<" a na koncu czas: "<<time<<" Cmax: "<<Cmax<<std::endl;

            }
        }
        std::cout<<"Cmax -> "<<Cmax<<std::endl;
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
    //int console=7;
    /*##########################################                
    ###    WCZYTANIE DANYCH/CONFIG OBIEKTU   ###
    ##########################################*/ 
    // std::ifstream file("SCHRAGE1.dat");
    // if (!file.is_open()) {
    //     std::cerr << "Blad: Nie udalo sie otworzyc pliku!" << std::endl;
    //     return 1;
    // }
    Problem p1;
    // p1.set_num_of_tasks(get_number_of_tasks(file));   // <- dziala igla
    // p1.read_from_file(file);

    //p1.print_tasks();


    /*###########################
    ###    WYBOR ALGORYTMU    ###
    ###########################*/
    // std::cout<<"Wybierz algorytm:\n1 - Przeglad calkowity\n2 - Schrage\n";
    // std::cin>>console;
    //auto start = std::chrono::high_resolution_clock::now();
    /*##############################
    ###    PRZEGLAD CALKOWITY    ###
    ##############################*/
    // if(console==1){
    //     p1.next_perm_fun();
    // }
    /*#####################
    ###    SCHRAGE      ###
    #####################*/
    // else if(console==2){
    //     p1.schrage_fun();
    // }
    //ogolnie to te dwa sa mega nieoptymalne ale sam je wymyslilem hehe
    //ty okazuje sie ze moj wymyslony create_algo_fun to jest jakis algorytm greedy, wiec jestem sprytny jednak,
    // chociaz nadal jest on kiepski dla zestawu SHRAGE2.txt xd
    //^ sznurowki cie zaraz wyprzedza
    /*###############################################################
    ###    Ktory to w koncu ten wlasciwy do sprawka potrzebne     ###
    ###############################################################*/
    // else if(console == 3){
    //     p1.create_algo_fun();
    // }
    /*#############################################
    ###   Dobra, wzialem obydwa nie zaszkodzi   ###
    #############################################*/
    // else if(console == 4){
    //     p1.create_algo_fun_2();
    // }
    /*#####################################
    ###    SCHRAGE (z przerwaniami)     ###
    #####################################*/
    // else if(console ==5){
    //     p1.schrage_interruption();
    // }
    /*#################
    ###   QJ SORT   ###
    #################*/
    // else if(console==6){
    //     p1.sort_qj_fun();
    // }
    /*#################
    ###   RJ SORT   ###
    #################*/
    // else if(console==7){
    //     p1.sort_rj_fun();
    // }


    /*################################################################################
    ###   Tak szczerze to nie wiem czemu tego w pętli nie zrobiłem xddd            ###
    ###   jakies zacmienie chyba mialem albo cos ale juz trudno, poradzisz sobie   ###   
    ################################################################################*/


    p1.set_num_of_tasks(6);
    p1.generate_instance();
    auto start = std::chrono::high_resolution_clock::now();
    p1.next_perm_fun();
    auto koniec = std::chrono::high_resolution_clock::now();
    auto zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu przeglad: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();


    std::cout<<"\n";    std::cout<<"\n";



    p1.set_num_of_tasks(8);
    p1.generate_instance();
    std::cout<<"Teraz dla 8"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.next_perm_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu przeglad: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();





    std::cout<<"\n";    std::cout<<"\n";



    p1.set_num_of_tasks(10);
    p1.generate_instance();
    std::cout<<"Teraz dla 10"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.next_perm_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu przeglad: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    p1.taskClear();



    std::cout<<"\n";    std::cout<<"\n";



    p1.set_num_of_tasks(12);
    p1.generate_instance();
    std::cout<<"Teraz dla 12"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
   // p1.next_perm_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu przeglad: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n\n";
    p1.taskClear();

    std::cout<<"\n";    std::cout<<"\n";



    p1.set_num_of_tasks(15);
    p1.generate_instance();
    std::cout<<"Teraz dla 15"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();

    std::cout<<"\n";    std::cout<<"\n";

    p1.set_num_of_tasks(17);
    p1.generate_instance();
    std::cout<<"Teraz dla 17"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();


    std::cout<<"\n";    std::cout<<"\n";

    p1.set_num_of_tasks(20);
    p1.generate_instance();
    std::cout<<"Teraz dla 20"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();


    std::cout<<"\n";    std::cout<<"\n";


    p1.set_num_of_tasks(30);
    p1.generate_instance();
    std::cout<<"Teraz dla 30"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();

    std::cout<<"\n";    std::cout<<"\n";

    p1.set_num_of_tasks(40);
    p1.generate_instance();
    std::cout<<"Teraz dla 40"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();




    p1.set_num_of_tasks(50);
    p1.generate_instance();
    std::cout<<"Teraz dla 50"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();












    p1.set_num_of_tasks(75);
    p1.generate_instance();
    std::cout<<"Teraz dla 75"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();




    p1.set_num_of_tasks(100);
    p1.generate_instance();
    std::cout<<"Teraz dla 100"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();












    p1.set_num_of_tasks(250);
    p1.generate_instance();
    std::cout<<"Teraz dla 250"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";    std::cout<<"\n";
    p1.taskClear();


















    p1.set_num_of_tasks(750);
    p1.generate_instance();
    std::cout<<"Teraz dla 750"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();

    std::cout<<"\n";    std::cout<<"\n";
    p1.set_num_of_tasks(1000);
    p1.generate_instance();
    std::cout<<"Teraz dla 1000"<<std::endl;
    start = std::chrono::high_resolution_clock::now();
    p1.sort_rj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.sort_qj_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.schrage_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    start = std::chrono::high_resolution_clock::now();
    p1.schrage_interruption();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";

    start = std::chrono::high_resolution_clock::now();
    p1.create_algo_fun();
    koniec = std::chrono::high_resolution_clock::now();
    zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    std::cout<<"Czas wykonania algorytmu wlasny1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    std::cout<<"\n";
    // start = std::chrono::high_resolution_clock::now();
    // p1.create_algo_fun_2();
    // koniec = std::chrono::high_resolution_clock::now();
    // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    // std::cout<<"Czas wykonania algorytmu wlasny2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;

    p1.taskClear();

    //p1.print_solution(); //<- dla schrage interruption bez sensu jest to solution
    return 0;
}
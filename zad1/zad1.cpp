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
    int upper_bound = INT_MAX;
    int lower_bound = 0;
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
    void taskClear(){
        task_instance.clear();
    }
    std::vector<Task> get_instance(){
        return task_instance;
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


    int sort_rj_fun(){
        sort_rj();
        int Cmax=calculate_Cmax();
        //std::cout<<"Cmax: "<<Cmax<<std::endl;
        return Cmax;
    }

    int sort_qj_fun(){
        sort_qj();
        int Cmax=calculate_Cmax();
        //std::cout<<"Cmax: "<<Cmax<<std::endl;
        return Cmax;
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



    int next_perm_fun(){
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
        //std::cout<<Cmin<<" <- Cmin"<<std::endl;
        return Cmin;
    }



    int schrage_fun(std::vector<Task>& tasks_sh){
        std::priority_queue<Task, std::vector<Task>, decltype(&compareTasksRj)> moundQueue(compareTasksRj);
        std::priority_queue<Task, std::vector<Task>, decltype(&compareTasksQj)> moundByQj(compareTasksQj);
        int time=0;
        int Cmax=0;
        int comparsion;
        int last_qj=0;
        int lastValue=0;
        int value;
        std::vector<Task> bufor;
        for(auto& x : tasks_sh){          
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
        //std::cout<<"Cmax -> "<<Cmax<<std::endl;

        return Cmax;
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
    int create_algo_fun(){
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
        //std::cout<<"Cmax final "<<Cmax_cr<<std::endl;
        return Cmax_cr;
    }

    int select_and_remove_max_pj_qj(int& Cmax) {
        int max_index = -1;
        int max_sum = 0;
        // szuka minimalnego czasu z tych ktore maja mniejszego rj od Cmax
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
        //std::cout<<"aktualne pj"<< task_instance[min_index].pj<<"aktualne qj"<<task_instance[min_index].qj<<std::endl;
        //std::cout<<"Cmax po "<<Cmax<<std::endl;
        task_instance.erase(task_instance.begin() + max_index); 
        //print_tasks_debug(task_instance);
        
        if(task_instance.empty()){
            return -2;
        }
        return max_index;
    }
    int create_algo_fun_2(){
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
        //std::cout<<"Cmax final 2 "<<Cmax_cr<<std::endl;
        return Cmax_cr;
    }



    int schrage_interruption(std::vector<Task>& tasks_sh_in){
        std::priority_queue<Task, std::vector<Task>, decltype(&compareTasksRj)> moundQueue(compareTasksRj);
        std::priority_queue<Task, std::vector<Task>, decltype(&compareTasksQj)> moundByQj(compareTasksQj);
        int time=0;
        int Cmax=0;
        int comparsion;
        int last_qj=0;
        int lastValue=0;
        int value;
        Task actual{0,0,0,INT_MAX};
        Task next;
        for(auto& x : tasks_sh_in){          
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
                    actual.pj=time-next.rj;
                    time = next.rj;
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
                time=time+moundByQj.top().pj;
                Cmax=std::max(Cmax,time+moundByQj.top().qj);
                moundByQj.pop();
                //std::cout<<" a na koncu czas: "<<time<<" Cmax: "<<Cmax<<std::endl;

            }
        }
        //std::cout<<"Cmax -> "<<Cmax<<std::endl;
        return Cmax;
    }
    void Carlier_fun(){
        std::vector<Task> tasks_copy(task_instance);
        int Cmax = schrage_fun(tasks_copy);
        int a = 0;
        if(Cmax < upper_bound){
            upper_bound = Cmax;
        }
        int time = 0;
        int b = 0;
        //szukam b, trzeba szukac od konca bo jest to najwiekszy indeks zadania, ktore koczy serie
        for(int i = (tasks_copy.size()-1); i>=0; i--){
            time = std::max(time, tasks_copy[i].rj)+tasks_copy[i].pj;
            if(time+tasks_copy[i].qj == Cmax){
                b = i;
                break;
            }
        }
        //szukam a
        int sum_pj;
        for(int i = 0; i <= b ; i++){
            sum_pj = 0;
            for(int s = i; s<=b; s++){
                sum_pj = sum_pj +tasks_copy[s].pj;
            }
            if(tasks_copy[i].rj + sum_pj + tasks_copy[i].qj == Cmax){
                a = i;
                break;
            }
        }
        //szukam c - ostatnie zadanie w przedziale <a,b> ktorego czas stygniecia jest mniejszy niz czas stgniecia od b
        int c = 0;
        for(int i = b; i>=a; i--){
            if(tasks_copy[i].qj < tasks_copy[b].qj){
                c = i;
                break;
            }
        }
        if(c == 0){
            //nie da sie bardziej oganiczyc ale jeszcze nie wiem co tutaj dac 
            return;
        }
        //szukamy h_K bez c
        int r_K = INT_MAX;
        int q_K = INT_MAX;
        int sum_p_K = 0;
        for(int K = (c+1); K<=b; K++){
            if(tasks_copy[K].rj < r_K){
                r_K = tasks_copy[K].rj;
            }
            if(tasks_copy[K].qj < q_K){
                q_K = tasks_copy[K].qj;
            }
            
            sum_p_K = sum_p_K + tasks_copy[K].pj;
        }
        // szukamy h_K z właczenie c do zbioru
        int r_K_c = INT_MAX;
        int q_K_c = INT_MAX;
        int sum_p_K_c = 0;
        for(int K = c; K<=b; K++){
            if(tasks_copy[K].rj < r_K_c){
                r_K_c = tasks_copy[K].rj;
            }
            if(tasks_copy[K].qj < q_K_c){
                q_K_c = tasks_copy[K].qj;
            }
            
            sum_p_K_c = sum_p_K_c + tasks_copy[K].pj;
        }
        int save_rj_c = tasks_copy[c].rj;
        tasks_copy[c].rj = std::max(tasks_copy[c].rj, r_K + sum_p_K);
        int h_K = r_K + sum_p_K + q_K;
        int h_K_c = r_K_c + sum_p_K_c +  q_K_c;
        lower_bound = schrage_interruption(tasks_copy);
        lower_bound = std::max(std::max(h_K, h_K_c) , lower_bound);

        if(lower_bound < upper_bound){
            Carlier_fun();
            //chyba jak ma buc rekurencja to nie moge inicjowac i zerowac zmiennych na poczatku
        }
        tasks_copy[c].rj = save_rj_c;
        int save_qj_c = tasks_copy[c].qj;
        tasks_copy[c].qj = std::max(tasks_copy[c].qj, q_K + sum_p_K);
        lower_bound = schrage_interruption(tasks_copy);
        lower_bound = std::max(std::max(h_K, h_K_c) , lower_bound);
        if( lower_bound < upper_bound){
            Carlier_fun();
        }
        tasks_copy[c].qj = save_qj_c;
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
    // srand(time(NULL));
    // int console=7;
    // int result = 0;
    // /*##########################################                
    // ###    WCZYTANIE DANYCH/CONFIG OBIEKTU   ###
    // ##########################################*/ 
    // std::ifstream file("SCHRAGE1.dat");
    // if (!file.is_open()) {
    //     std::cerr << "Blad: Nie udalo sie otworzyc pliku!" << std::endl;
    //     return 1;
    // }
    Problem p1;

    // p1.set_num_of_tasks(get_number_of_tasks(file));   // <- dziala igla
    // p1.read_from_file(file);

    // //p1.print_tasks();


    // /*###########################
    // ###    WYBOR ALGORYTMU    ###
    // ###########################*/
    // // std::cout<<"Wybierz algorytm:\n1 - Przeglad calkowity\n2 - Schrage\n";
    // // std::cin>>console;
    // auto start = std::chrono::high_resolution_clock::now();
    // /*##############################
    // ###    PRZEGLAD CALKOWITY    ###
    // ##############################*/
    // if(console==1){
    //     p1.next_perm_fun();
    // }
    // /*#####################
    // ###    SCHRAGE      ###
    // #####################*/
    // else if(console==2){
    //     std::vector<Task> tasks = p1.get_inst();
    //     result = p1.schrage_fun(tasks);
    //     std::cout<<"Cmax "<<result<<std::endl;
    // }
    // //ogolnie to te dwa sa mega nieoptymalne ale sam je wymyslilem hehe
    // //ty okazuje sie ze moj wymyslony create_algo_fun to jest jakis algorytm greedy, wiec jestem sprytny jednak,
    // // chociaz nadal jest on kiepski dla zestawu SHRAGE2.txt xd
    // else if(console == 3){
    //     p1.create_algo_fun();
    // }
    // else if(console == 4){
    //     p1.create_algo_fun_2();
    // }
    // else if(console ==5){
    //     std::vector<Task> tasks = p1.get_inst();
    //     result = p1.schrage_interruption(tasks);
    //     std::cout<<"Cmax "<<result<<std::endl;
    // }
    // else if(console==6){
    //     p1.sort_qj_fun();
    // }
    // else if(console==7){
    //     p1.sort_rj_fun();
    // }
    // auto koniec = std::chrono::high_resolution_clock::now();
    // auto zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
        
    // std::cout<<"Czas wykonania algorytmu: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    // //p1.print_solution(); //<- dla schrage interruption bez sensu jest to solution

    /////////////////////////////////////////////////////////////////////////////////////////////
    //////////////// TO JEST GIT ALE POLICZYMY SREDNIA /////////////////////////////////////////
    // std::vector<int> instance_size = {5, 7, 9, 10, 11};
    // int result = 0;
    // int result_przeglad = 0;
    // for (int i = 0; i < instance_size.size(); i++){
    //     std::cout<<"Wielkosc intacji: "<< instance_size[i]<<std::endl;
    //     p1.set_num_of_tasks(instance_size[i]);
    //     p1.generate_instance();
    //     auto start = std::chrono::high_resolution_clock::now();
    //     result_przeglad = p1.next_perm_fun();
    //     auto koniec = std::chrono::high_resolution_clock::now();
    //     auto zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    //     std::cout<<"Wynik algorytmu przeglad: "<<result_przeglad<<std::endl;
    //     std::cout<<"Czas wykonania algorytmu przeglad: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    //     std::cout<<"\n";
    //     start = std::chrono::high_resolution_clock::now();
    //     result = p1.sort_rj_fun();
    //     koniec = std::chrono::high_resolution_clock::now();
    //     zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    //     std::cout<<"Wynik algorytmu sort r_j: "<<result<<std::endl;
    //     std::cout<<"Blad algorytmu sort r_j: "<<static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad* 100<<"%"<<std::endl;
    //     std::cout<<"Czas wykonania algorytmu sort r_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    //     std::cout<<"\n";
    //     start = std::chrono::high_resolution_clock::now();
    //     result = p1.sort_qj_fun();
    //     koniec = std::chrono::high_resolution_clock::now();
    //     zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    //     std::cout<<"Wynik algorytmu q_j: "<<result<<std::endl;
    //     std::cout<<"Blad algorytmu sort q_j: "<<static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad* 100<<"%"<<std::endl;
    //     std::cout<<"Czas wykonania algorytmu sort q_j: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    //     std::cout<<"\n";

    //     start = std::chrono::high_resolution_clock::now();
    //     std::vector<Task> tasks = p1.get_inst();
    //     result = p1.schrage_fun(tasks);
    //     koniec = std::chrono::high_resolution_clock::now();
    //     zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    //     std::cout<<"Wynik algorytmu schrage: "<<result<<std::endl;
    //     std::cout<<"Blad algorytmu schrage: "<<static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad* 100<<"%"<<std::endl;
    //     std::cout<<"Czas wykonania algorytmu schrage: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    //     std::cout<<"\n";
    //     start = std::chrono::high_resolution_clock::now();
    //     std::vector<Task> tasks1 = p1.get_inst();
    //     result = p1.schrage_interruption(tasks1);
    //     koniec = std::chrono::high_resolution_clock::now();
    //     zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    //     std::cout<<"Wynik algorytmu schrage z przerwaniami: "<<result<<std::endl;
    //     std::cout<<"Blad algorytmu schrage z przerwaniami: "<<static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad* 100<<"%"<<std::endl;
    //     std::cout<<"Czas wykonania algorytmu schrage przerwania: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    //     std::cout<<"\n";

    //     start = std::chrono::high_resolution_clock::now();
    //     result = p1.create_algo_fun();
    //     koniec = std::chrono::high_resolution_clock::now();
    //     zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    //     std::cout<<"Wynik algorytmu wlasnego 1: "<<result<<std::endl;
    //     std::cout<<"Blad algorytmu wlasnego 1: "<<static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad* 100<<"%"<<std::endl;
    //     std::cout<<"Czas wykonania algorytmu wlasny 1: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    //     std::cout<<"\n";
    //     // start = std::chrono::high_resolution_clock::now();
    //     // result = p1.create_algo_fun_2();
    //     // koniec = std::chrono::high_resolution_clock::now();
    //     // zmierzonyCzas = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start);
    //     // std::cout<<"Wynik algorytmu wlasnego 2: "<<result<<std::endl;
    //     // std::cout<<"Czas wykonania algorytmu wlasny 2: "<< zmierzonyCzas.count() * 1e-9<<" sekund"<<std::endl;
    //     // std::cout<<"\n";

    //     p1.taskClear();
    // }
    std::vector<int> instance_size = {5, 7, 9, 10, 11};
const int num_repeats = 50;

for (int i = 0; i < instance_size.size(); i++) {

    double avg_time_przeglad = 0, avg_time_rj = 0, avg_time_qj = 0, avg_time_schrage = 0, avg_time_inter = 0, avg_time_custom1 = 0,avg_time_custom2 = 0;
    double avg_result_przeglad = 0, avg_result_rj = 0, avg_result_qj = 0, avg_result_schrage = 0, avg_result_inter = 0, avg_result_custom1 = 0, avg_result_custom2 = 0;
    double avg_error_rj = 0, avg_error_qj = 0, avg_error_schrage = 0, avg_error_inter = 0, avg_error_custom1 = 0, avg_error_custom2 = 0;

    for (int j = 0; j < num_repeats; j++) {
        p1.set_num_of_tasks(instance_size[i]);
        p1.generate_instance();

        // Przegląd
        auto start = std::chrono::high_resolution_clock::now();
        int result_przeglad = p1.next_perm_fun();
        auto koniec = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        avg_time_przeglad += time;
        avg_result_przeglad += result_przeglad;

        // sort_rj
        start = std::chrono::high_resolution_clock::now();
        int result = p1.sort_rj_fun();
        koniec = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        avg_time_rj += time;
        avg_result_rj += result;
        avg_error_rj += static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad * 100;

        // sort_qj
        start = std::chrono::high_resolution_clock::now();
        result = p1.sort_qj_fun();
        koniec = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        avg_time_qj += time;
        avg_result_qj += result;
        avg_error_qj += static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad * 100;

        // schrage
        std::vector<Task> tasks = p1.get_inst();
        start = std::chrono::high_resolution_clock::now();
        result = p1.schrage_fun(tasks);
        koniec = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        avg_time_schrage += time;
        avg_result_schrage += result;
        avg_error_schrage += static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad * 100;

        // schrage z przerwaniami
        std::vector<Task> tasks1 = p1.get_inst();
        start = std::chrono::high_resolution_clock::now();
        result = p1.schrage_interruption(tasks1);
        koniec = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        avg_time_inter += time;
        avg_result_inter += result;
        //avg_error_inter += static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad * 100;

        // wlasna 1
        start = std::chrono::high_resolution_clock::now();
        result = p1.create_algo_fun();
        koniec = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        avg_time_custom1 += time;
        avg_result_custom1 += result;
        avg_error_custom1 += static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad * 100;
        // wlasna 2
        start = std::chrono::high_resolution_clock::now();
        //result = p1.create_algo_fun_2();
        // koniec = std::chrono::high_resolution_clock::now();
        // time = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        // avg_time_custom2 += time;
        // avg_result_custom2 += result;
        // avg_error_custom2 += static_cast<double>(std::abs(result - result_przeglad)) / result_przeglad * 100;

        p1.taskClear();
    }

    // Oblicz średnie
    avg_time_przeglad /= num_repeats;
    avg_time_rj /= num_repeats;
    avg_time_qj /= num_repeats;
    avg_time_schrage /= num_repeats;
    avg_time_inter /= num_repeats;
    avg_time_custom1 /= num_repeats;
    //avg_result_custom2 /= num_repeats;

    avg_result_przeglad /= num_repeats;
    avg_result_rj /= num_repeats;
    avg_result_qj /= num_repeats;
    avg_result_schrage /= num_repeats;
    avg_result_inter /= num_repeats;
    avg_result_custom1 /= num_repeats;
    //avg_result_custom2 /= num_repeats;

    avg_error_rj /= num_repeats;
    avg_error_qj /= num_repeats;
    avg_error_schrage /= num_repeats;
    avg_error_inter /= num_repeats;
    avg_error_custom1 /= num_repeats;
    //avg_error_custom2 /= num_repeats;

    // Wyświetl wyniki
    std::cout << "SREDNIE WYNIKI DLA INSTANCJI: " << instance_size[i] << " (na podstawie " << num_repeats << " prob):\n";
    std::cout << "Alg. przeglad: wynik = " << avg_result_przeglad << ", czas = " << avg_time_przeglad << "s\n";
    std::cout << "Alg. sort r_j: wynik = " << avg_result_rj << ", blad = " << avg_error_rj << "%, czas = " << avg_time_rj << "s\n";
    std::cout << "Alg. sort q_j: wynik = " << avg_result_qj << ", blad = " << avg_error_qj << "%, czas = " << avg_time_qj << "s\n";
    std::cout << "Alg. schrage: wynik = " << avg_result_schrage << ", blad = " << avg_error_schrage << "%, czas = " << avg_time_schrage << "s\n";
    std::cout << "Alg. schrage + przerwania: wynik = " << avg_result_inter << ", blad = " << avg_error_inter << "%, czas = " << avg_time_inter << "s\n";
    std::cout << "Alg. wlasny 1: wynik = " << avg_result_custom1 << ", blad = " << avg_error_custom1 << "%, czas = " << avg_time_custom1 << "s\n";
    //std::cout << "Alg. wlasny 2: wynik = " << avg_result_custom2 << ", blad = " << avg_error_custom2 << "%, czas = " << avg_time_custom2 << "s\n";
    std::cout << "\n==========================================================\n\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<int> instance_size2 = {20, 50, 100};

for (int i = 0; i < instance_size2.size(); i++) {

    double time_rj = 0, time_qj = 0, time_schrage = 0, time_inter = 0, time_custom1 = 0;
    double result_rj = 0, result_qj = 0, result_schrage = 0, result_inter = 0, result_custom1 = 0;
    double error_rj = 0, error_qj = 0, error_schrage = 0, error_inter = 0, error_custom1 = 0;
    
    p1.set_num_of_tasks(instance_size2[i]);
    p1.generate_instance();
    std::vector<int> results;
    

        // sort_rj
        auto start = std::chrono::high_resolution_clock::now();
        result_rj = p1.sort_rj_fun();
        auto koniec = std::chrono::high_resolution_clock::now();
        time_rj = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        results.push_back(result_rj);
        

        // sort_qj
        start = std::chrono::high_resolution_clock::now();
        result_qj = p1.sort_qj_fun();
        koniec = std::chrono::high_resolution_clock::now();
        time_qj = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        results.push_back(result_qj);
       
        // schrage
        std::vector<Task> tasks = p1.get_inst();
        start = std::chrono::high_resolution_clock::now();
        result_schrage = p1.schrage_fun(tasks);
        koniec = std::chrono::high_resolution_clock::now();
        time_schrage = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        results.push_back(result_schrage);
        
        // schrage z przerwaniami
        std::vector<Task> tasks1 = p1.get_inst();
        start = std::chrono::high_resolution_clock::now();
        result_inter = p1.schrage_interruption(tasks1);
        koniec = std::chrono::high_resolution_clock::now();
        time_inter = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;

        // wlasna 1
        start = std::chrono::high_resolution_clock::now();
        result_custom1 = p1.create_algo_fun();
        koniec = std::chrono::high_resolution_clock::now();
        time_custom1 = std::chrono::duration_cast<std::chrono::nanoseconds>(koniec - start).count() * 1e-9;
        results.push_back(result_custom1);

        p1.taskClear();
    

    // Oblicz najlepszy wynik
    
    int best_result = *std::min_element(results.begin(), results.end());

    //std::cout<<"best "<< best_result<<std::endl;


    error_rj += static_cast<double>(std::abs(result_rj - best_result)) / best_result * 100;
    error_qj += static_cast<double>(std::abs(result_qj - best_result)) / best_result * 100;
    error_schrage += static_cast<double>(std::abs(result_schrage - best_result)) / best_result * 100;
    error_custom1 += static_cast<double>(std::abs(result_custom1 - best_result)) / best_result * 100;

    // Wyświetl wyniki
    std::cout << "WYNIKI DLA INSTANCJI: " << instance_size2[i] << std::endl;
    std::cout << "Alg. sort r_j: wynik = " << result_rj << ", blad = " << error_rj << "%, czas = " << time_rj << "s\n";
    std::cout << "Alg. sort q_j: wynik = " << result_qj << ", blad = " << error_qj << "%, czas = " << time_qj << "s\n";
    std::cout << "Alg. wlasny 1: wynik = " << result_custom1 << ", blad = " << error_custom1 << "%, czas = " << time_custom1 << "s\n";
    std::cout << "Alg. schrage: wynik = " << result_schrage << ", blad = " << error_schrage << "%, czas = " << time_schrage << "s\n";
    std::cout << "Alg. schrage + przerwania: wynik = " << result_inter << " , czas = " << time_inter << "s\n";
    
    std::cout << "\n==========================================================\n\n";
}

    
    return 0;
}
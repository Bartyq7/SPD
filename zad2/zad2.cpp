#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <queue>
#include <thread>
#include <math.h>


class Task{
    int j;
    int pj;
    int machine;
    public:
    int get_j(){
        return j;
    }
    int get_pj(){
        return pj;
    }
    int get_machine(){
        return machine;
    }
    void set_j(int a){
        j=a;
    }
    void set_pj(int b){
        pj=b;
    }
    void set_machine(int c){
        machine=c;
    }
};
bool compareTasksPj(Task& a, Task & b){
    return a.get_pj()>b.get_pj();
}
bool compareTasksJ(Task& a, Task & b){
    return a.get_j()<b.get_j();
}

class Machine{
    std::vector<Task> machine_tasks;
    public:
};

class Problem{
    int num_of_tasks=3;
    int num_of_machines=2;
    std::vector<Task> begin_instance;
    //std::vector<Task> solution;
    public:
    void generate_instance(){
        for(int i =0; i < num_of_tasks; i++){
            Task t1;
            t1.set_j(i+1);
            t1.set_pj(( std::rand() % 15 )+1);
            t1.set_machine(-1);
            //std::cout<<t1.pj<<std::endl;
            begin_instance.push_back(t1);
        }
    }
    void print_begin_instance(){
        std::cout<<"BEGIN INSTANCE\n";
        std::cout<<"j: \t pj \t mach\n\n";
        for(int i=0; i < num_of_tasks; i++){
            std::cout<< begin_instance[i].get_j() << " \t "<<begin_instance[i].get_pj()<< " \t "<<begin_instance[i].get_machine()<<std::endl;
        }
    }
    void print_tasks(std::vector<Task> v_print){
        std::cout<<"j: \t pj \t mach\n\n";
        for(int i=0; i < num_of_tasks; i++){
            std::cout<< v_print[i].get_j() << " \t "<<v_print[i].get_pj()<< " \t "<<v_print[i].get_machine()<<std::endl;
        }
    }

    void make_random_solution(std::vector<Task>& solution){
        solution = begin_instance;
        for(int i = 0; i<num_of_tasks; i++){
            int rand_machine = std::rand() % num_of_machines;
            //std::cout<<"rand mach "<<rand_machine<<std::endl;
            solution[i].set_machine(rand_machine);
            //solution.push_back(begin_instance_copy[i]);
        }
    }
    void LSA(std::vector<Task>& solution){
        solution = begin_instance;
        //std::sort(solution.begin(), solution.end(), compareTasksPj);
        print_tasks(solution);
        std::vector<int> times_of_machines(num_of_machines);
        //ustawiamy pierwsze zadanie do pierwszej maszyny a potem juz po kolei do tej ktora na najnizszy czas
        solution[0].set_machine(0);
        times_of_machines[0] = solution[0].get_pj();
        for(int i = 1; i<num_of_tasks; i++){
            auto it = std::min_element(times_of_machines.begin(), times_of_machines.end());
            int index = std::distance(times_of_machines.begin(), it);
            std::cout<<"przypisuje maszynie: "<<index<<" wartosc "<<solution[i].get_pj()<<std::endl;
            
            solution[i].set_machine(index);
            times_of_machines[index] += solution[i].get_pj();
            for(int j = 0; j<num_of_machines; j++){
                std::cout<<"aktuale czasy maszyny "<<j<<": "<<times_of_machines[j]<<std::endl;
            }
        }
        //std::sort(solution.begin(), solution.end(), compareTasksJ);
        
    }
    void LPT(std::vector<Task>& solution){
        solution = begin_instance;
        std::sort(solution.begin(), solution.end(), compareTasksPj);
        print_tasks(solution);
        std::vector<int> times_of_machines(num_of_machines);
        //ustawiamy pierwsze zadanie do pierwszej maszyny a potem juz po kolei do tej ktora na najnizszy czas
        solution[0].set_machine(0);
        times_of_machines[0] = solution[0].get_pj();
        for(int i = 1; i<num_of_tasks; i++){
            auto it = std::min_element(times_of_machines.begin(), times_of_machines.end());
            int index = std::distance(times_of_machines.begin(), it);
            std::cout<<"przypisuje maszynie: "<<index<<" wartosc "<<solution[i].get_pj()<<std::endl;
            
            solution[i].set_machine(index);
            times_of_machines[index] += solution[i].get_pj();
            for(int j = 0; j<num_of_machines; j++){
                std::cout<<"aktuale czasy maszyny "<<j<<": "<<times_of_machines[j]<<std::endl;
            }
        }
        std::sort(solution.begin(), solution.end(), compareTasksJ);
        
    }
   
    int count_Cmax(std::vector<Task> solution){
        std::vector<int> times_of_machines(num_of_machines);
        int Cmax;
        if(solution.empty()){
            std::cout<<"Solution empty\n"<<std::endl;
            return -1;
        }

        for(int i = 0; i<num_of_tasks; i++){
            for(int j = 0; j<num_of_machines; j++){
                if(solution[i].get_machine() == j){
                    times_of_machines[j] += solution[i].get_pj();
                }
            }
        }
            auto max_it = std::max_element(times_of_machines.begin(), times_of_machines.end());

            if (max_it != times_of_machines.end()) {
                Cmax = *max_it;
            } else {
                std::cout << "Wektor jest pusty." << std::endl;
            }
        return Cmax;
    }
    void dynamic_prog_fun(std::vector<Task>& solution){
        solution = begin_instance;
        int total_times = 0;
        for(int i = 0; i < solution.size(); i++){
            total_times += solution[i].get_pj();
        }
    
        int K = total_times / 2 + 1;
    
        // Tablica DP T[j][k] - czy można osiągnąć sumę k przy pierwszych j zadaniach
        std::vector<std::vector<int>> T(solution.size() + 1, std::vector<int>(K, 0));
    
        // Inicjalizacja: dla j=0 (zadanie 0 o czasie 0), suma 0 jest osiągalna
        for (int j = 0; j <= solution.size(); j++){
            T[j][0] = 1;
        }
        for (int j = 1; j <= solution.size(); j++) {
            int pj = solution[j - 1].get_pj();
            for (int k = 0; k < K; k++) {
                if ((T[j-1][k]==1)||((k>=pj)&&(T[j-1][k-pj]==1))){
                    T[j][k]=1;
                }
            }
        }

        // std::cout << "\nTablica T (j = rzędy, k = kolumny):\n    ";
        // for (int k = 0; k < K; k++)
        //     std::cout << k << "\t";
        // std::cout << "\n";

        // for (int j = 0; j <= solution.size(); j++) {
        //     std::cout << "j=" << j << " ";
        //     for (int k = 0; k < K; k++) {
        //         std::cout << T[j][k] << "\t";
        //     }
        // std::cout << "\n";
        // }
        
        int best_k = K-1;
        for (int k = K - 1; k >= 0; k--) {
            if (T[solution.size()][k] == 1) {
                best_k = k;
                break;
            }
        }

        for (int j = solution.size(); j >= 1; j--) {
            int pj = solution[j - 1].get_pj();
            //std::cout<<"Best k: "<<best_k<<std::endl;
            if (best_k >= pj && T[j - 1][best_k - pj]) {
                solution[j - 1].set_machine(1);
                //std::cout<<"przypisuje maszynie 1 zadanie "<<(j-1)<<" o wartosci "<<solution[j-1].get_pj()<<std::endl;
            
                best_k -= pj;
            } else {
                solution[j - 1].set_machine(0); 
                //std::cout<<"przypisuje maszynie 0 zadanie "<<(j-1)<<" o wartosci "<<solution[j-1].get_pj()<<std::endl;
            }
        }
    }
    void next_perm_fun(){
        int best_Cmax = INT_MAX;
        std::vector<Task> best_solution;

        int total_combinations = std::pow(num_of_machines, num_of_tasks);
    
        for(int i=0; i<total_combinations; i++){
            std::vector<Task> current_solution = begin_instance;

            int combination = i;
    
            for(int j=0; j<num_of_tasks; j++){
                int assigned_machine = combination % num_of_machines;
                current_solution[j].set_machine(assigned_machine);
                combination /= num_of_machines;
            }
    
            int current_Cmax = count_Cmax(current_solution);
            if(current_Cmax < best_Cmax){
                best_Cmax = current_Cmax;
                best_solution = current_solution;
            }
        }
    
        std::cout << "\nBest solution:\n";
        print_tasks(best_solution);
        std::cout << "Cmax_best: " << best_Cmax << std::endl;
    }

};

int main(){
    srand(time(NULL));
    Problem p1;
    std::vector<Task> random_solution;
    std::vector<Task> lpt_solution;
    std::vector<Task> lsa_solution;
    std::vector<Task> dynamic_solution;
    p1.generate_instance();
    p1.print_begin_instance();

    //p1.make_random_solution(random_solution);
    // int Cmax_rand = p1.count_Cmax(random_solution);
    // std::cout<< "Cmax_rand: " << Cmax_rand<<std::endl; 
    // p1.print_tasks(random_solution);
    
    //LPT
    // p1.LPT(lpt_solution);
    // int Cmax_lpt = p1.count_Cmax(lpt_solution);
    // std::cout<< "Cmax_lpt: " << Cmax_lpt<<std::endl; 
    // p1.print_tasks(lpt_solution);
    

    //LSA
    // p1.LSA(lsa_solution);
    // int Cmax_lsa = p1.count_Cmax(lsa_solution);
    // std::cout<< "Cmax_lsa: " << Cmax_lsa<<std::endl; 
    // p1.print_tasks(lsa_solution);
    
    p1.dynamic_prog_fun(dynamic_solution);
    int Cmax_dyn = p1.count_Cmax(dynamic_solution);
    std::cout<< "Cmax_dyn: " << Cmax_dyn<<std::endl; 
    p1.print_tasks(dynamic_solution);
    //p1.next_perm_fun();
}
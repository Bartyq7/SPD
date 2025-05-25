#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <limits>

class Task {
    int j;
    std::vector<int> processing_times;

public:
    int get_j()const{ 
        return j; 
    }
    void set_j(int id){
        j = id;
    }

    void set_processing_times(const std::vector<int>& times){
        processing_times = times;
    }

    int get_pj(int machine) const{
        return processing_times[machine];
    }
};

class ProblemFlowShop{
    int num_of_tasks=4;
    int num_of_machines =3;
    std::vector<Task> tasks;

public:
    void generate_instance(){
        for (int i =0; i<num_of_tasks;i++){
            Task t;
            t.set_j(i+ 1);
            std::vector<int> proc_times;
            for (int m =0; m <num_of_machines;m++){
                proc_times.push_back((rand()%20)+1);
            }
            t.set_processing_times(proc_times);
            tasks.push_back(t);
        }
    }

    int calculate_Cmax(const std::vector<Task>& perm){
        std::vector<std::vector<int>> C(num_of_tasks,std::vector<int>(num_of_machines,0));

        for (int i=0; i<num_of_tasks;i++){
            for (int m=0; m< num_of_machines;m++){
                int time_prev_task=(i>0)?C[i-1][m]:0;
                int time_prev_machine=(m>0)?C[i][m-1]:0;

                C[i][m]=std::max(time_prev_task, time_prev_machine)+ perm[i].get_pj(m);
            }
        }

        return C[num_of_tasks-1][num_of_machines-1];
    }

    void exhaustive_search(){
        std::vector<Task> best_perm;
        int best_Cmax=std::numeric_limits<int>::max();

        std::vector<Task>current_perm=tasks;
        int permutation_counter=0;

        do {
            int current_Cmax=calculate_Cmax(current_perm);
            if (current_Cmax<best_Cmax){
                best_Cmax=current_Cmax;
                best_perm=current_perm;
            }
            permutation_counter++;
        } while(std::next_permutation(current_perm.begin(),current_perm.end(),
                 [](const Task& a, const Task& b){return a.get_j()<b.get_j(); }));

        std::cout<<"Best Cmax: "<<best_Cmax<<std::endl;
        std::cout <<"Solution: ";
        for(const auto& task:best_perm){
            std::cout<<task.get_j()<< " ";
        }
        std::cout<<std::endl;
    }

    void print_instance(){
        std::cout<< "BEGIN INSTANCE:"<<std::endl;
        std::cout<<"Nr M1 M2 M3"<<std::endl;
        for(const auto& t:tasks){
            std::cout<<t.get_j()<<"  ";
            for(int m=0; m<num_of_machines;++m){
                std::cout << t.get_pj(m) << " ";
            }
            std::cout <<std::endl;
        }
    }
};

int main(){
    srand(time(NULL));

    ProblemFlowShop problem;
    problem.generate_instance();
    problem.print_instance();

    auto start =std::chrono::high_resolution_clock::now();
    problem.exhaustive_search();
    auto end= std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed=end-start;
    std::cout<<"Czas wykonania algorytmu"<< elapsed.count()<<" sekund\n";

    return 0;
}
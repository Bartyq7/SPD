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
    int num_of_tasks=11;
    int num_of_machines =2;
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
        std::vector<std::vector<int>> C(perm.size(),std::vector<int>(num_of_machines,0));

        for (int i=0; i<perm.size();i++){
            for (int m=0; m< num_of_machines;m++){
                int time_prev_task=(i>0)?C[i-1][m]:0;
                int time_prev_machine=(m>0)?C[i][m-1]:0;

                C[i][m]=std::max(time_prev_task, time_prev_machine)+ perm[i].get_pj(m);
            }
        }

        return C[perm.size()-1][num_of_machines-1];
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
        std::cout << "BEGIN INSTANCE:" << std::endl;
        std::cout << "Nr ";
        for (int m = 0; m < num_of_machines; m++) {
            std::cout << "M" << (m + 1) << " ";
        }
        std::cout << std::endl;

        for (const auto& t : tasks) {
            std::cout << t.get_j() << "  ";
            for (int m = 0; m < num_of_machines; ++m) {
                std::cout << t.get_pj(m) << " ";
            }
            std::cout << std::endl;
        }
    }


    void neh_algorithm() {
    
    std::vector<std::pair<int, Task>> term;
    for (int i = 0; i < tasks.size(); ++i) {
        int suma = 0;
        for (int m = 0; m < num_of_machines; ++m) {
            suma += tasks[i].get_pj(m);
        }
        term.push_back({suma, tasks[i]});
    }



    std::sort(term.begin(), term.end(), [](std::pair<int, Task>& a, std::pair<int, Task>& b) {
        return a.first > b.first;
    });

    
    std::vector<Task> sorted_tasks;
    for (auto& t : term) {
        sorted_tasks.push_back(t.second);
    }

    std::vector<Task> solution;
    std::vector<Task> current_perm;
    //int best_Cmax=std::numeric_limits<int>::max();
    
    for(int i = 0; i < sorted_tasks.size(); i++){
    int best_Cmax = std::numeric_limits<int>::max();
    int best_position = 0;

        for (int pos = 0; pos <= current_perm.size(); ++pos) {
            std::vector<Task> temp = current_perm;
            temp.insert(temp.begin() + pos, sorted_tasks[i]);

            // std::cout << "Temp (pozycja " << pos << "): ";
            // for (const auto& t : temp) std::cout << t.get_j() << " ";
            // std::cout << std::endl;

            int cmax = calculate_Cmax(temp);
            //std::cout<<"cmax dla pos: "<< pos<<"  "<<cmax<<std::endl;
            
            if (cmax < best_Cmax) {
                best_Cmax = cmax;
                best_position = pos;
            }
        }

        current_perm.insert(current_perm.begin() + best_position, sorted_tasks[i]);
    }

    std::cout << "NEH: Cmax = " << calculate_Cmax(current_perm) << "\nPermutacja: ";
    for (const auto& t : current_perm) std::cout << t.get_j() << " ";
    std::cout << std::endl;
}

void fneh_algorithm() {
        std::vector<std::pair<int, Task>> term;
        for (int i = 0; i < tasks.size(); ++i) {
            int suma = 0;
            for (int m = 0; m < num_of_machines; ++m) {
                suma += tasks[i].get_pj(m);
            }
            term.push_back({suma, tasks[i]});
        }

        std::sort(term.begin(), term.end(), [](std::pair<int, Task>& a, std::pair<int, Task>& b) {
            return a.first > b.first;
        });
        
        std::vector<Task> sorted_tasks;
        for (auto& t : term) {
            sorted_tasks.push_back(t.second);
        }

        std::vector<Task> current_perm;
        
        for(int i = 0; i < sorted_tasks.size(); i++){ // k-th task to insert
            int best_Cmax = std::numeric_limits<int>::max();
            int best_position = 0;
            const Task& task_to_insert = sorted_tasks[i];

            // For each possible insertion position
            for (int pos = 0; pos <= current_perm.size(); ++pos) {
                std::vector<std::vector<int>> H_values(current_perm.size(), std::vector<int>(num_of_machines, 0));
                if (!current_perm.empty()) {
                    for (int i = 0; i < current_perm.size(); ++i) {
                        for (int m = 0; m < num_of_machines; ++m) {
                            int time_prev_task = (i > 0) ? H_values[i - 1][m] : 0;
                            int time_prev_machine = (m > 0) ? H_values[i][m - 1] : 0;
                            H_values[i][m] = std::max(time_prev_task, time_prev_machine) + current_perm[i].get_pj(m);
                        }
                    }
                }
                std::vector<std::vector<int>> T_values(current_perm.size(), std::vector<int>(num_of_machines, 0));
                if (!current_perm.empty()) {
                    for (int i = current_perm.size() - 1; i >= 0; --i) {
                        for (int m = num_of_machines - 1; m >= 0; --m) {
                            int time_next_task = (i < current_perm.size() - 1) ? T_values[i + 1][m] : 0;
                            int time_next_machine = (m < num_of_machines - 1) ? T_values[i][m + 1] : 0;
                            T_values[i][m] = std::max(time_next_task, time_next_machine) + current_perm[i].get_pj(m);
                        }
                    }
                }

                int current_inserted_cmax = 0;

                std::vector<int> C_inserted_task_on_machines(num_of_machines, 0);

                for (int m = 0; m < num_of_machines; ++m) {
                    int prev_job_time_on_machine = (pos > 0) ? H_values[pos - 1][m] : 0;
                    int prev_machine_time_on_job = (m > 0) ? C_inserted_task_on_machines[m - 1] : 0;
                    C_inserted_task_on_machines[m] = std::max(prev_job_time_on_machine, prev_machine_time_on_job) + task_to_insert.get_pj(m);
                }

                current_inserted_cmax = 0;
                for (int m = 0; m < num_of_machines; ++m) {
                    int completion_time_at_this_point = C_inserted_task_on_machines[m];
                    if (pos < current_perm.size()) {
                        completion_time_at_this_point += T_values[pos][m];
                    }
                    current_inserted_cmax = std::max(current_inserted_cmax, completion_time_at_this_point);
                }
                    
                if (current_inserted_cmax < best_Cmax) {
                    best_Cmax = current_inserted_cmax;
                    best_position = pos;
                }
            }
            current_perm.insert(current_perm.begin() + best_position, task_to_insert);
        }

        std::cout << "FNEH: Cmax = " << calculate_Cmax(current_perm) << "\nPermutation: ";
        for (const auto& t : current_perm) std::cout << t.get_j() << " ";
        std::cout << std::endl;
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
    std::cout<<"Czas wykonania algorytmu przeglad zupelny "<< elapsed.count()<<" sekund\n";

    ///////////////////////////////NEH/////////////////////////////////////
    
    start =std::chrono::high_resolution_clock::now();
    problem.neh_algorithm();
    end= std::chrono::high_resolution_clock::now();

    elapsed=end-start;
    std::cout<<"Czas wykonania algorytmu NEH "<< elapsed.count()<<" sekund\n";

    ///////////////////////////////FNEH/////////////////////////////////////
    start =std::chrono::high_resolution_clock::now();
    problem.fneh_algorithm();
    end= std::chrono::high_resolution_clock::now();

    elapsed=end-start;
    std::cout<<"Czas wykonania algorytmu FNEH "<< elapsed.count()<<" sekund\n";
    return 0;
}
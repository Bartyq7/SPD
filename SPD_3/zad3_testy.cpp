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
    int num_of_tasks=8;
    int num_of_machines =3;
    int range=20;
    std::vector<Task> tasks;

public:

    void set_machines(int m){
        num_of_machines=m;
    }
    void set_range(int l){
        range=l;
    }
    void set_tasks(int n){
        num_of_tasks=n;
    }
    std::vector<Task> get_tasks()const{
        return tasks;
    }
    void set_tasks_from_copy(const std::vector<Task>& new_tasks){
        tasks=new_tasks;
    }

    void generate_instance(){
        for (int i =0; i<num_of_tasks;i++){
            Task t;
            t.set_j(i+ 1);
            std::vector<int> proc_times;
            for (int m =0; m <num_of_machines;m++){
                proc_times.push_back((rand()%range)+1);
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

    int exhaustive_search() {
        std::vector<Task> best_perm;
        int best_Cmax = std::numeric_limits<int>::max();
    
        std::vector<Task> current_perm = tasks;
    
        std::sort(current_perm.begin(), current_perm.end(), [](const Task& a, const Task& b){
            return a.get_j() < b.get_j();
        });
    
        do {
            int current_Cmax = calculate_Cmax(current_perm);
            if (current_Cmax < best_Cmax) {
                best_Cmax = current_Cmax;
                best_perm = current_perm;
            }
        } while (std::next_permutation(current_perm.begin(), current_perm.end(),
                 [](const Task& a, const Task& b){ return a.get_j() < b.get_j(); }));
    
        return best_Cmax;
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


int neh_algorithm() {
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

    for (int i = 0; i < sorted_tasks.size(); i++) {
        int best_Cmax = std::numeric_limits<int>::max();
        int best_position = 0;

        for (int pos = 0; pos <= current_perm.size(); ++pos) {
            std::vector<Task> temp = current_perm;
            temp.insert(temp.begin() + pos, sorted_tasks[i]);

            int cmax = calculate_Cmax(temp);
            if (cmax < best_Cmax) {
                best_Cmax = cmax;
                best_position = pos;
            }
        }

        current_perm.insert(current_perm.begin() + best_position, sorted_tasks[i]);
    }

    return calculate_Cmax(current_perm);
}

int fneh_algorithm() {
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

    for (int i = 0; i < sorted_tasks.size(); i++) {
        int best_Cmax = std::numeric_limits<int>::max();
        int best_position = 0;
        const Task& task_to_insert = sorted_tasks[i];

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

    return calculate_Cmax(current_perm);
}

int johnson_algorithm() {
    std::vector<Task> left;
    std::vector<Task> right;

    for(const auto& task:tasks) {
        if(task.get_pj(0)<=task.get_pj(1)){
            left.push_back(task);
        }
        else{
            right.push_back(task);
        }
    }


    std::sort(left.begin(),left.end(), [](const Task& a,const Task& b){
        return a.get_pj(0) <b.get_pj(0);
    });

    std::sort(right.begin(),right.end(),[](const Task& a,const Task& b){
        return a.get_pj(1)> b.get_pj(1);
    });


    std::vector<Task> johnson_perm;
    johnson_perm.insert(johnson_perm.end(),left.begin(),left.end());
    johnson_perm.insert(johnson_perm.end(),right.begin(),right.end());

    int cmax=calculate_Cmax(johnson_perm);
    // std::cout<<"Johnson: Cmax = "<<cmax<<"\nPermutacja: ";
    // for(const auto& t:johnson_perm) std::cout<<t.get_j()<<" ";
    // std::cout<<std::endl;
    return cmax;
}

};



// int main() {
//     srand(time(NULL));
//     const int num_tests = 20;

//     struct TestConfig {
//         int test_id;
//         int machines;
//         int pj_range;
//         std::vector<int> tasks_list;
//         bool use_exhaustive;
//         bool use_johnson;
//     };

//     std::vector<TestConfig> configs = {
//         // {1, 2, 10, {6, 8, 10}, true, true},
//         // {2, 2, 100, {6, 8, 10}, true, true},
//         // {3, 2, 10, {12, 14, 16}, false, true},
//         // {4, 2, 100, {12, 14, 16}, false, true},
//         {7, 10, 10, {6, 8, 10}, true, false},
//         {8, 10, 100, {6, 8, 10}, true, false},
//         {9, 4, 10, {6, 8, 10}, true, false},
//         {10, 4, 100, {6, 8, 10}, true, false},
//         {11, 5, 10, {6, 8, 10}, true, false},
//         {12, 5, 100, {6, 8, 10}, true, false}

//     };

//     for (const auto& cfg : configs) {
//         std::cout << "\n================ TEST " << cfg.test_id << " ================\n";
//         std::cout << "Maszyny: " << cfg.machines << ", Range: " << cfg.pj_range << "\n";

//         for (int tasks_count : cfg.tasks_list) {
//             std::cout << "\nZadania: " << tasks_count << "\n";

//             double total_time_exh = 0, total_time_neh = 0, total_time_fneh = 0, total_time_joh=0;
//             double total_err_abs_neh = 0, total_err_rel_neh = 0, total_err_rel_joh=0, total_err_abs_joh=0;
//             double total_err_abs_fneh = 0, total_err_rel_fneh = 0;

//             for (int t = 0; t < num_tests; ++t) {
//                 std::cout<<"Test nr"<<t<<std::endl;
//                 ProblemFlowShop problem;
//                 problem.set_tasks(tasks_count);
//                 problem.set_machines(cfg.machines);
//                 problem.set_range(cfg.pj_range);
//                 problem.generate_instance();
//                 std::vector<Task> original_tasks = problem.get_tasks();

//                 int best_cmax = -1;
//                 if (cfg.use_exhaustive && tasks_count <= 10) {
//                     auto start = std::chrono::high_resolution_clock::now();
//                     best_cmax = problem.exhaustive_search();
//                     auto end = std::chrono::high_resolution_clock::now();
//                     total_time_exh += std::chrono::duration<double>(end - start).count();
//                 } else {
//                     best_cmax = problem.neh_algorithm();  // użyj NEH jako referencji
//                 }

//                 // NEH
//                 problem.set_tasks_from_copy(original_tasks);
//                 auto start = std::chrono::high_resolution_clock::now();
//                 int neh_cmax = problem.neh_algorithm();
//                 auto end = std::chrono::high_resolution_clock::now();
//                 double neh_time = std::chrono::duration<double>(end - start).count();
//                 total_time_neh += neh_time;
//                 total_err_abs_neh += neh_cmax - best_cmax;
//                 total_err_rel_neh += 100.0 * (neh_cmax - best_cmax) / best_cmax;

//                 // FNEH
//                 problem.set_tasks_from_copy(original_tasks);
//                 start = std::chrono::high_resolution_clock::now();
//                 int fneh_cmax = problem.fneh_algorithm();
//                 end = std::chrono::high_resolution_clock::now();
//                 double fneh_time = std::chrono::duration<double>(end - start).count();
//                 total_time_fneh += fneh_time;
//                 total_err_abs_fneh += fneh_cmax - best_cmax;
//                 total_err_rel_fneh += 100.0 * (fneh_cmax - best_cmax) / best_cmax;

//                 // Johnson (tylko dla 2 maszyn)
//                 if (cfg.use_johnson && cfg.machines == 2) {
//                     problem.set_tasks_from_copy(original_tasks);
//                     auto start = std::chrono::high_resolution_clock::now();
//                     int johnson_cmax = problem.johnson_algorithm();
//                     auto end = std::chrono::high_resolution_clock::now();
//                     double johnson_time = std::chrono::duration<double>(end - start).count();
//                     total_time_joh += johnson_time;
//                     total_err_abs_joh += johnson_cmax - best_cmax;
//                     total_err_rel_joh += 100.0 * (johnson_cmax - best_cmax) / best_cmax;
//                 }
//             }

//             std::cout << "\nŚrednie dla " << num_tests << " prób:\n";

//             if (cfg.use_exhaustive && tasks_count <= 10) {
//                 std::cout << "Przegląd zupełny: średni czas = " << total_time_exh / num_tests << " s\n";
//             }

//             std::cout << "NEH:   czas = " << total_time_neh / num_tests << " s, "
//                       << "błąd bezwzględny = " << total_err_abs_neh / num_tests << ", "
//                       << "błąd względny = " << total_err_rel_neh / num_tests << "%\n";

//             std::cout << "FNEH:  czas = " << total_time_fneh / num_tests << " s, "
//                       << "błąd bezwzględny = " << total_err_abs_fneh / num_tests << ", "
//                       << "błąd względny = " << total_err_rel_fneh / num_tests << "%\n";
//                       if (cfg.use_johnson && cfg.machines == 2) {
//                         std::cout << "Johnson: czas = " << total_time_joh / num_tests << " s, "
//                                   << "błąd bezwzględny = " << total_err_abs_joh / num_tests << ", "
//                                   << "błąd względny = " << total_err_rel_joh / num_tests << "%\n";
//                     }
//         }
//     }

//     return 0;
// }
int main() {
    srand(time(NULL));

    std::vector<std::vector<std::vector<int>>> test_data = {
        {   // TEST 1: 5 zadań, 3 maszyny
            {{10, 20, 15}, {12, 18, 22}, {14, 16, 19}, {11, 17, 13}, {13, 15, 17}}
        },
        {   // TEST 2: 3 zadania, 3 maszyny
            {{1, 2, 4}, {5, 3, 2}, {2, 3, 1}}
        },
        {   // TEST 3: 20 zadań, 5 maszyn
            {{54, 79, 16, 66, 58}, {83, 3, 89, 58, 56}, {15, 11, 49, 31, 20},
             {71, 99, 15, 68, 85}, {77, 56, 89, 78, 53}, {36, 70, 45, 91, 35},
             {53, 99, 60, 13, 53}, {38, 60, 23, 59, 41}, {27, 5, 57, 49, 69},
             {87, 56, 64, 85, 13}, {76, 3, 7, 85, 86}, {91, 61, 1, 9, 72},
             {14, 73, 63, 39, 8}, {29, 75, 41, 41, 49}, {12, 47, 63, 56, 47},
             {77, 14, 47, 40, 87}, {32, 21, 26, 54, 58}, {87, 86, 75, 77, 18},
             {68, 5, 77, 51, 68}, {94, 77, 40, 31, 28}}
        }
    };

    for (int test_id = 0; test_id < test_data.size(); ++test_id) {
        std::vector<std::vector<int>>& instance = test_data[test_id];
        int num_tasks = instance.size();
        int num_machines = instance[0].size();

        ProblemFlowShop problem;
        problem.set_tasks(num_tasks);
        problem.set_machines(num_machines);

        std::vector<Task> tasks;
        for (int i = 0; i < num_tasks; ++i) {
            Task t;
            t.set_j(i + 1);
            t.set_processing_times(instance[i]);
            tasks.push_back(t);
        }

        problem.set_tasks_from_copy(tasks);

        std::cout << "\n========== TEST " << (test_id + 1) << " ==========\n";
        problem.print_instance();

        std::vector<Task> original_tasks = problem.get_tasks();

        int best_cmax = -1;
        if (test_id < 2) { // exhaustive search tylko dla testu 1 i 2
            auto start = std::chrono::high_resolution_clock::now();
            best_cmax = problem.exhaustive_search();
            auto end = std::chrono::high_resolution_clock::now();
            double time = std::chrono::duration<double>(end - start).count();
            std::cout << "Przegląd zupełny: Cmax = " << best_cmax << ", czas = " << time << " s\n";
        } else {
            best_cmax = problem.neh_algorithm(); // użyj NEH jako referencji
            std::cout << "(Brak przeglądu zupełnego – NEH jako odniesienie): Cmax = " << best_cmax << "\n";
        }

        // NEH
        problem.set_tasks_from_copy(original_tasks);
        auto start = std::chrono::high_resolution_clock::now();
        int neh_cmax = problem.neh_algorithm();
        auto end = std::chrono::high_resolution_clock::now();
        double neh_time = std::chrono::duration<double>(end - start).count();
        std::cout << "NEH:   Cmax = " << neh_cmax
                  << ", czas = " << neh_time
                  << ", błąd bezwzględny = " << neh_cmax - best_cmax
                  << ", względny = " << 100.0 * (neh_cmax - best_cmax) / best_cmax << "%\n";

        // FNEH
        problem.set_tasks_from_copy(original_tasks);
        start = std::chrono::high_resolution_clock::now();
        int fneh_cmax = problem.fneh_algorithm();
        end = std::chrono::high_resolution_clock::now();
        double fneh_time = std::chrono::duration<double>(end - start).count();
        std::cout << "FNEH:  Cmax = " << fneh_cmax
                  << ", czas = " << fneh_time
                  << ", błąd bezwzględny = " << fneh_cmax - best_cmax
                  << ", względny = " << 100.0 * (fneh_cmax - best_cmax) / best_cmax << "%\n";
    }

    return 0;
}
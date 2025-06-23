#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <limits>
#include <fstream>
#include <string>
#include <iomanip>
#include <locale>

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
    int num_of_tasks;
    int num_of_machines;
    std::vector<Task> tasks;

public:
    ProblemFlowShop() : num_of_tasks(0), num_of_machines(0) {}

    std::vector<Task> get_tasks(){
        return tasks;
    }
    int get_num_machines(){
        return num_of_machines;
    }
    void set_dimensions(int n_tasks, int n_machines) {
        num_of_tasks = n_tasks;
        num_of_machines = n_machines;
        tasks.clear();
    }

    void add_task(const Task& t) {
        tasks.push_back(t);
    }

    void clear_tasks() {
        tasks.clear();
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

    int exhaustive_search(){
        if (tasks.empty()) {
            return -1;
        }

        std::vector<Task> best_perm;
        int best_Cmax=std::numeric_limits<int>::max();

        std::vector<Task>current_perm=tasks;
        std::sort(current_perm.begin(),current_perm.end(),
                 [](const Task& a, const Task& b){return a.get_j()<b.get_j(); });

        do {
            int current_Cmax=calculate_Cmax(current_perm);
            if (current_Cmax<best_Cmax){
                best_Cmax=current_Cmax;
                best_perm=current_perm;
            }
        } while(std::next_permutation(current_perm.begin(),current_perm.end(),
                 [](const Task& a, const Task& b){return a.get_j()<b.get_j(); }));

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
        if (tasks.empty()) {
            return -1;
        }

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

        for(int i = 0; i < sorted_tasks.size(); i++){
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
        if (tasks.empty()) {
            return -1;
        }

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

        for(int i = 0; i < sorted_tasks.size(); ++i){
            int best_Cmax = std::numeric_limits<int>::max();
            int best_position = 0;
            const Task& task_to_insert = sorted_tasks[i];

            std::vector<std::vector<int>> H_values(current_perm.size(), std::vector<int>(num_of_machines, 0));
            if (!current_perm.empty()) {
                for (int current_i = 0; current_i < current_perm.size(); ++current_i) {
                    for (int m = 0; m < num_of_machines; ++m) {
                        int time_prev_task = (current_i > 0) ? H_values[current_i - 1][m] : 0;
                        int time_prev_machine = (m > 0) ? H_values[current_i][m - 1] : 0;
                        H_values[current_i][m] = std::max(time_prev_task, time_prev_machine) + current_perm[current_i].get_pj(m);
                    }
                }
            }

            std::vector<std::vector<int>> T_values(current_perm.size(), std::vector<int>(num_of_machines, 0));
            if (!current_perm.empty()) {
                for (int current_i = current_perm.size() - 1; current_i >= 0; --current_i) {
                    for (int m = num_of_machines - 1; m >= 0; --m) {
                        int time_next_task = (current_i < current_perm.size() - 1) ? T_values[current_i + 1][m] : 0;
                        int time_next_machine = (m < num_of_machines - 1) ? T_values[current_i][m + 1] : 0;
                        T_values[current_i][m] = std::max(time_next_task, time_next_machine) + current_perm[current_i].get_pj(m);
                    }
                }
            }

            for (int pos = 0; pos <= current_perm.size(); ++pos) {
                std::vector<int> C_inserted_task_on_machines(num_of_machines, 0);

                for (int m = 0; m < num_of_machines; ++m) {
                    int prev_job_time_on_machine = (pos > 0) ? H_values[pos - 1][m] : 0;
                    int prev_machine_time_on_job = (m > 0) ? C_inserted_task_on_machines[m - 1] : 0;
                    C_inserted_task_on_machines[m] = std::max(prev_job_time_on_machine, prev_machine_time_on_job) + task_to_insert.get_pj(m);
                }

                int current_inserted_cmax = 0;
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
        if (tasks.empty()) {
            return -1;
        }
        if (num_of_machines != 2) {
            return -1;
        }

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

        return calculate_Cmax(johnson_perm);
    }

    static std::vector<int> read_expected_cmax_from_file(const std::string& filename) {
        std::vector<int> expected_values;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Nie można otworzyć pliku z oczekiwanymi wynikami: " << filename << std::endl;
            return expected_values;
        }

        std::string line;

        while (std::getline(file, line)) {
            try {
                expected_values.push_back(std::stoi(line));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Błąd konwersji linii na int: " << line << " - " << e.what() << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Wartość poza zakresem: " << line << " - " << e.what() << std::endl;
            }
        }
        return expected_values;
    }
};

double calculate_relative_error(int calculated_cmax, int optimal_cmax) {
    if (optimal_cmax == 0) return 0.0;
    return static_cast<double>(std::abs(calculated_cmax - optimal_cmax)) / optimal_cmax;
}

int main(){
    srand(time(NULL));

    std::vector<ProblemFlowShop> instances;
    std::ifstream file("tail.dat");
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku tail.dat" << std::endl;
        return 1;
    }

    int total_num_of_instances;
    file >> total_num_of_instances;
    std::string dummy;
    std::getline(file, dummy);

    for (int instance_idx = 0; instance_idx < total_num_of_instances; ++instance_idx) {
        ProblemFlowShop current_problem;
        int num_of_tasks_in_instance;
        int num_of_machines_in_instance;
        file >> num_of_tasks_in_instance >> num_of_machines_in_instance;
        std::getline(file, dummy);

        current_problem.set_dimensions(num_of_tasks_in_instance, num_of_machines_in_instance);

        for (int i = 0; i < num_of_tasks_in_instance; ++i) {
            Task t;
            t.set_j(i + 1);
            std::vector<int> proc_times(num_of_machines_in_instance);
            int machine_idx;
            int time_val;
            for (int m = 0; m < num_of_machines_in_instance; ++m) {
                file >> machine_idx >> time_val;
                proc_times[machine_idx] = time_val;
            }
            t.set_processing_times(proc_times);
            current_problem.add_task(t);
            std::getline(file, dummy);
        }
        instances.push_back(current_problem);
    }
    file.close();

    std::vector<int> expected_cmax_values = ProblemFlowShop::read_expected_cmax_from_file("TAIL.TXT");

    std::ofstream output_file("results.csv");
    if (!output_file.is_open()) {
        std::cerr << "Nie można otworzyć pliku results.csv do zapisu." << std::endl;
        return 1;
    }

    output_file.imbue(std::locale("C")); // Ustawia lokalizację "C" dla strumienia wyjściowego

    // Nagłówki CSV ze średnikami
    output_file << "Liczba Maszyn;Liczba zadań;Zakres Pj;Optymalny Cmax;NEH - Cmax;NEH - Średni czas;NEH - Błąd względny;FNEH - Cmax;FNEH - Średni czas;FNEH - Błąd względny\n";

    for (int i = 0; i < instances.size(); ++i) {
        int current_num_of_machines = instances[i].get_num_machines();
        int current_num_of_tasks = instances[i].get_tasks().size();
        
        std::cout << "\n--- Przetwarzanie instancji " << (i + 1) << " (Zadań: " << current_num_of_tasks << ", Maszyn: " << current_num_of_machines << ") ---" << std::endl;

        output_file << current_num_of_machines << ";" << current_num_of_tasks << ";"; 
        output_file << "1-100;"; // Zakres Pj

        int optimal_cmax_for_instance = (i < expected_cmax_values.size()) ? expected_cmax_values[i] : -1;
        output_file << optimal_cmax_for_instance << ";";

        // NEH
        auto start_neh = std::chrono::high_resolution_clock::now();
        int neh_cmax = instances[i].neh_algorithm();
        auto end_neh = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_neh = end_neh - start_neh;
        output_file << neh_cmax << ";";
        output_file << std::fixed << std::setprecision(6) << elapsed_neh.count() << ";";
        if (optimal_cmax_for_instance != -1) {
            output_file << std::fixed << std::setprecision(6) << calculate_relative_error(neh_cmax, optimal_cmax_for_instance) << ";";
        } else {
            output_file << ";";
        }

        // FNEH
        auto start_fneh = std::chrono::high_resolution_clock::now();
        int fneh_cmax = instances[i].fneh_algorithm();
        auto end_fneh = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_fneh = end_fneh - start_fneh;
        output_file << fneh_cmax << ";";
        output_file << std::fixed << std::setprecision(6) << elapsed_fneh.count() << ";";
        if (optimal_cmax_for_instance != -1) {
            output_file << std::fixed << std::setprecision(6) << calculate_relative_error(fneh_cmax, optimal_cmax_for_instance) << "\n";
        } else {
            output_file << "\n";
        }
    }

    output_file.close();
    std::cout << "\nWyniki zostały zapisane do pliku: results.csv" << std::endl;

    return 0;
}
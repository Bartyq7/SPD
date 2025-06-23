#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <limits>
#include <fstream> // Do operacji na plikach
#include <string>  // Do obsługi linii z pliku

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
    // Konstruktor domyślny
    ProblemFlowShop() : num_of_tasks(0), num_of_machines(0) {}
    
    std::vector<Task> get_tasks(){
        return tasks;
    }
    int get_num_machines(){
        return num_of_machines;
    }
    // Metoda do ustawiania liczby zadań i maszyn
    void set_dimensions(int n_tasks, int n_machines) {
        num_of_tasks = n_tasks;
        num_of_machines = n_machines;
        tasks.clear(); // Czyścimy zadania przy zmianie wymiarów
    }

    // Metoda do dodawania zadania
    void add_task(const Task& t) {
        tasks.push_back(t);
    }

    // Metoda do czyszczenia zadań
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

    void exhaustive_search(){
        // Sprawdzenie, czy istnieją zadania do przetworzenia
        if (tasks.empty()) {
            std::cout << "Brak zadań do wykonania przeglądu zupełnego." << std::endl;
            return;
        }

        std::vector<Task> best_perm;
        int best_Cmax=std::numeric_limits<int>::max();

        std::vector<Task>current_perm=tasks;
        // Sortowanie początkowej permutacji, aby std::next_permutation działało poprawnie
        std::sort(current_perm.begin(),current_perm.end(),
                 [](const Task& a, const Task& b){return a.get_j()<b.get_j(); });


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

        std::cout<<"Best Cmax (Exhaustive Search): "<<best_Cmax<<std::endl;
        std::cout <<"Solution (Exhaustive Search): ";
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
    if (tasks.empty()) {
        std::cout << "Brak zadań do wykonania algorytmu NEH." << std::endl;
        return;
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

    std::cout << "NEH: Cmax = " << calculate_Cmax(current_perm); //<< "\nPermutacja: ";
    // for (const auto& t : current_perm) std::cout << t.get_j() << " ";
    // std::cout << std::endl;
}

void fneh_algorithm() {
    if (tasks.empty()) {
        std::cout << "Brak zadań do wykonania algorytmu FNEH." << std::endl;
        return;
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

    for(int i = 0; i < sorted_tasks.size(); ++i){ // k-th task to insert
        int best_Cmax = std::numeric_limits<int>::max();
        int best_position = 0;
        const Task& task_to_insert = sorted_tasks[i];

        // WAŻNA ZMIANA: Oblicz H_values i T_values raz dla obecnej current_perm
        // przed rozpoczęciem pętli wstawiania.
        // H_values[j][m] = czas zakończenia zadania j na maszynie m dla permutacji do zadania j
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

        // T_values[j][m] = czas od maszyny m do końca dla zadania j, jeśli idzie się od końca permutacji
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

        // For each possible insertion position
        for (int pos = 0; pos <= current_perm.size(); ++pos) {
            std::vector<int> C_inserted_task_on_machines(num_of_machines, 0);

            // Oblicz czas zakończenia wstawianego zadania
            for (int m = 0; m < num_of_machines; ++m) {
                // Czas zakończenia poprzedniego zadania na tej maszynie (z H_values)
                int prev_job_time_on_machine = (pos > 0) ? H_values[pos - 1][m] : 0;
                // Czas zakończenia wstawianego zadania na poprzedniej maszynie
                int prev_machine_time_on_job = (m > 0) ? C_inserted_task_on_machines[m - 1] : 0;
                C_inserted_task_on_machines[m] = std::max(prev_job_time_on_machine, prev_machine_time_on_job) + task_to_insert.get_pj(m);
            }

            // Oblicz Cmax dla tej konkretnej pozycji wstawienia
            int current_inserted_cmax = 0;
            for (int m = 0; m < num_of_machines; ++m) {
                int completion_time_at_this_point = C_inserted_task_on_machines[m];
                // Dodaj czasy z T_values tylko jeśli są zadania po wstawionym
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

    std::cout << "FNEH: Cmax = " << calculate_Cmax(current_perm); //<< "\nPermutation: ";
    // for (const auto& t : current_perm) std::cout << t.get_j() << " ";
    // std::cout << std::endl;
}

void johnson_algorithm() {
    if (tasks.empty()) {
        std::cout << "Brak zadań do wykonania algorytmu Johnsona." << std::endl;
        return;
    }
    if (num_of_machines != 2) {
        std::cout << "Algorytm Johnsona wymaga 2 maszyn. Obecna liczba maszyn: " << num_of_machines << std::endl;
        return;
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

    int cmax=calculate_Cmax(johnson_perm);
    std::cout<<"Johnson: Cmax = "<<cmax<<"\nPermutacja: ";
    for(const auto& t:johnson_perm) std::cout<<t.get_j()<<" ";
    std::cout<<std::endl;
}


// Funkcja do wczytywania danych instancji z pliku
bool read_instance_from_file(const std::string& filename, ProblemFlowShop& problem_instance) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return false;
    }

    int total_num_of_instances;
    file >> total_num_of_instances;

    std::cout << "Wykryto " << total_num_of_instances << " instancji." << std::endl;

    for (int instance_idx = 0; instance_idx < total_num_of_instances; ++instance_idx) {
        int num_of_tasks_in_instance;
        int num_of_machines_in_instance;
        file >> num_of_tasks_in_instance >> num_of_machines_in_instance;

        problem_instance.set_dimensions(num_of_tasks_in_instance, num_of_machines_in_instance);
        problem_instance.clear_tasks(); // Czyścimy zadania przed wczytaniem nowej instancji

        for (int i = 0; i < num_of_tasks_in_instance; ++i) {
            Task t;
            t.set_j(i + 1); // Indeks zadania (1-based)
            std::vector<int> proc_times(num_of_machines_in_instance);
            for (int m = 0; m < num_of_machines_in_instance; ++m) {
                int machine_id;
                int time;
                file >> machine_id >> time; // Odczytujemy parę: id maszyny i czas
                proc_times[machine_id] = time; // Przypisujemy czas do odpowiedniej maszyny (zakładając 0-based indexing dla wektora)
            }
            t.set_processing_times(proc_times);
            problem_instance.add_task(t);
        }
        // Po wczytaniu jednej instancji, możesz ją tutaj zwrócić lub dodać do wektora instancji
        // W tym przykładzie, aby uprościć, będziemy przetwarzać jedną instancję naraz w main
        // i przekazywać nowo wczytaną instancję do funkcji main.
        // Jeśli chcesz przetwarzać wszystkie instancje, będziesz musiał zmienić strukturę.
        return true; // Na potrzeby tego przykładu, wczytujemy tylko pierwszą instancję
    }
    return true;
}

// Funkcja do wczytywania oczekiwanych wartości Cmax z pliku TAIL.TXT
static std::vector<int> read_expected_cmax_from_file(const std::string& filename) {
    std::vector<int> expected_values;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku z oczekiwanymi wynikami: " << filename << std::endl;
        return expected_values;
    }

    std::string line;
    // Pomijamy linię z 
    std::getline(file, line);

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

}; // Koniec klasy ProblemFlowShop


int main(){
    srand(time(NULL));

    std::vector<ProblemFlowShop> instances;
    // Wczytaj wszystkie instancje
    std::ifstream file("tail.dat");
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku tail.dat" << std::endl;
        return 1;
    }

    int total_num_of_instances;
    file >> total_num_of_instances;
    std::string dummy;
    std::getline(file, dummy); // Konsumuj resztę linii po odczytaniu total_num_of_instances

    for (int instance_idx = 0; instance_idx < total_num_of_instances; ++instance_idx) {
        ProblemFlowShop current_problem;
        int num_of_tasks_in_instance;
        int num_of_machines_in_instance;
        file >> num_of_tasks_in_instance >> num_of_machines_in_instance;
        std::getline(file, dummy); // Konsumuj resztę linii

        current_problem.set_dimensions(num_of_tasks_in_instance, num_of_machines_in_instance);

        for (int i = 0; i < num_of_tasks_in_instance; ++i) {
            Task t;
            t.set_j(i + 1);
            std::vector<int> proc_times(num_of_machines_in_instance);
            int machine_idx;
            int time_val;
            // Odczytujemy pary maszyna czas
            for (int m = 0; m < num_of_machines_in_instance; ++m) {
                file >> machine_idx >> time_val;
                proc_times[machine_idx] = time_val; // Zakładamy, że machine_idx jest 0-based
            }
            t.set_processing_times(proc_times);
            current_problem.add_task(t);
            std::getline(file, dummy); // Konsumuj resztę linii po odczytaniu czasów przetwarzania
        }
        instances.push_back(current_problem);
    }
    file.close();

    std::vector<int> expected_cmax_values = ProblemFlowShop::read_expected_cmax_from_file("TAIL.TXT");

    for (int i = 0; i < instances.size(); ++i) {
        std::cout << "\n--- Przetwarzanie instancji " << (i + 1) << " ---" << std::endl;
        //instances[i].print_instance();

        // Jeśli liczba maszyn jest większa niż 2, pomiń przegląd zupełny i algorytm Johnsona
        if (instances[i].calculate_Cmax(instances[i].get_tasks().empty() ? std::vector<Task>() : instances[i].get_tasks()) == 0 && instances[i].get_tasks().empty()) {
             // Skip exhaustive search if no tasks or if calculate_Cmax returns 0 (which it might for empty)
            std::cout << "Pominięto przegląd zupełny dla tej instancji (zbyt wiele zadań/maszyn lub brak zadań)." << std::endl;
        } else {
            // Przegląd zupełny (może być bardzo długi dla dużych instancji)
            if (instances[i].calculate_Cmax(instances[i].get_tasks().empty() ? std::vector<Task>() : instances[i].get_tasks()) > 0 && instances[i].get_tasks().size() <= 10) { // Ogranicz do 10 zadań dla przeglądu zupełnego
                auto start = std::chrono::high_resolution_clock::now();
                instances[i].exhaustive_search();
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = end - start;
                std::cout << "Czas wykonania algorytmu przegląd zupełny: " << elapsed.count() << " sekund\n";
                if (i < expected_cmax_values.size()) {
                    // Tutaj brakuje zmiennej best_Cmax z exhaustive_search. Musielibyśmy ją zwrócić.
                    // Na potrzeby porównania, po prostu obliczymy Cmax dla najlepszej permutacji (jeśli jest dostępna)
                    // lub wyświetlimy komunikat o braku porównania.
                    std::cout << "Oczekiwany Cmax (Exhaustive Search): " << expected_cmax_values[i] << std::endl;
                }
            } else {
                std::cout << "Pominięto przegląd zupełny dla tej instancji (zbyt wiele zadań)." << std::endl;
            }
        }

        ///////////////////////////////NEH/////////////////////////////////////
        auto start_neh = std::chrono::high_resolution_clock::now();
        instances[i].neh_algorithm();
        auto end_neh = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_neh = end_neh - start_neh;
        std::cout << "Czas wykonania algorytmu NEH: " << elapsed_neh.count() << " sekund\n";
        if (i < expected_cmax_values.size()) {
            std::cout << "Oczekiwany Cmax (NEH): " << expected_cmax_values[i] << std::endl;
        }

        ///////////////////////////////FNEH/////////////////////////////////////
        auto start_fneh = std::chrono::high_resolution_clock::now();
        instances[i].fneh_algorithm();
        auto end_fneh = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_fneh = end_fneh - start_fneh;
        std::cout << "Czas wykonania algorytmu FNEH: " << elapsed_fneh.count() << " sekund\n";
        if (i < expected_cmax_values.size()) {
            std::cout << "Oczekiwany Cmax (FNEH): " << expected_cmax_values[i] << std::endl;
        }

        ///////////////////////////////JOHNSON/////////////////////////////////////
        // Johnsona tylko dla 2 maszyn
        if (instances[i].get_tasks().empty() ? 0 : instances[i].calculate_Cmax(instances[i].get_tasks()) > 0 && instances[i].get_tasks().empty() ? 0 : instances[i].get_num_machines() == 2) {
            auto start_johnson = std::chrono::high_resolution_clock::now();
            instances[i].johnson_algorithm();
            auto end_johnson = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_johnson = end_johnson - start_johnson;
            std::cout << "Czas wykonania algorytmu Johnsona: " << elapsed_johnson.count() << " sekund\n";
            if (i < expected_cmax_values.size()) {
                std::cout << "Oczekiwany Cmax (Johnson): " << expected_cmax_values[i] << std::endl;
            }
        } else {
            std::cout << "Pominięto algorytm Johnsona dla tej instancji (wymaga 2 maszyn)." << std::endl;
        }
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>


class Task{
public:
    int j;
    int pj;
    int rj;
    int qj;
};
class Problem{
    std::vector<Task> task_instance;
    int num_of_tasks;
    int C_max;
    public:
    void set_num_of_tasks(int number){
        num_of_tasks=number;
    }
   

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
        std::cout<<"j: \t pj \t rj \t qj\n\n";
        for(int i=0; i < num_of_tasks; i++){
            std::cout<< task_instance[i].j << " \t "<<task_instance[i].pj << " \t "<<task_instance[i].rj << " \t "<< task_instance[i].qj<<std::endl;
        }
    }

    int calculate_Cmax(){
        int Cmax=0;
        if(num_of_tasks>=1000)
            std::cout<<"Wariacie! Wlasnie obliczasz Cmaxa dla pliku, ktory ma 1000 instancji czy Ciebie nie popierdolilo?"<<std::endl;
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
            file>>t1.pj>>t1.rj>>t1.qj;
            task_instance.push_back(t1);
        }
    }
};


int get_number_of_tasks(std::ifstream& cosik){
    std::string first_lane;
    int numer_of_tasks;

    if(std::getline(cosik, first_lane)){
        numer_of_tasks=stoi(first_lane);
    }
    else{
        std::cout<<"Wariacie! Jesli myslisz, ze mozesz tak o sobie pobrac\npierwsza linijke pliku to jestes w pierdolonym bledzie!"<<std::endl;
        return -1;
    }




    return numer_of_tasks;
}



int main(){
    srand(time(NULL));

    std::ifstream file("SCHRAGE1.dat");
    
    /*#####################################################
       ^ Barteczku, Bartuniu strzel ino obsługę błędów ^
    #####################################################*/


    Problem p1;
    p1.set_num_of_tasks(get_number_of_tasks(file));   // <- dziala igla
    p1.read_from_file(file);
    p1.print_tasks();
    std::cout<<"\nCmax: "<<p1.calculate_Cmax()<<std::endl;
    
    p1.sort_pj();
    p1.print_tasks();
    
    p1.sort_rj();
    p1.print_tasks();



    return 0;
}
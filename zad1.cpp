#include <iostream>
#include <vector>
#include <algorithm>


class Task{
public:
    int j;
    int pj;
    int rj;
    int qj;
};
class Problem{
    std::vector<Task> task_instance;
    const int num_of_tasks = 5;
    int C_max;
    public:
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
    void sort_rj(){
        
    }
};

int main(){
    srand(time(NULL));

    Problem p1;
    p1.generate_instance();
    p1.print_tasks();

}
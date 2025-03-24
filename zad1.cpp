#include <iostream>
#include <vector>



class Task{
public:
    int pj;
    int rj;
    int qj;
};

int main(){
    srand(time(NULL));

    std::vector<Task> task_instance;
    for(int i =0; i < 120; i++){
        Task t1;
        t1.pj = ( std::rand() % 15 )+1;
        t1.rj = ( std::rand() % 15 );
        t1.qj = ( std::rand() % 15 )+1;

        std::cout<<t1.pj<<std::endl;
    }

}
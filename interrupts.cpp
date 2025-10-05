/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * @Rameen Rahman
 */


#include<interrupts.hpp>


int main(int argc, char** argv) {


    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);


    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output


    /******************ADD YOUR VARIABLES HERE*************************/


    std::int time_count = 0;
    std::int ISR_delay;
   
    std::const int SAVE_CONTEXT = 10;
    std::const int EXECUTE_ISR = 40;
    std::const int EXECUTE_IRET = 1;


    /******************************************************************/


    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);
        //input is parsed by ',' and placed into pairs [string, int]


        /******************ADD YOUR SIMULATION CODE HERE*************************/
       
        if (activity == "CPU"){
            execution += time_count + ", " + duration_intr + ", " + "CPU burst\n";
            time_count += duration_intr;
        }


        else if (activity == "SYSCALL"){

            auto [ISR_execution, current_time] = intr_boilerplate(time_count, duration_intr, SAVE_CONTEXT, vectors);
           
            execution += ISR_execution;
            time_count = current_time;

            ISR_delay = delays[duration_intr];
            
            while(ISR_delay != 0){
                if(ISR_delay >= EXECUTE_ISR){
                    execution += time_count + ", " + EXECUTE_ISR + ", " + "call device driver\n";
                    time_count += EXECUTE_ISR;
                    ISR_delay -= EXECUTE_ISR;
                }
                else{
                    execution += time_count + ", " + ISR_delay + ", " + "call device driver\n";
                    time_count += ISR_delay;
                    ISR_delay = 0;
                }
            }

            execution += time_count + ", " + EXECUTE_IRET + ", " + "IRET\n";
            time_count += EXECUTE_IRET;
        }


        else if (activity == "END_IO"){
            ISR_delay = delays[duration_intr];
            
            while(ISR_delay != 0){
                if(ISR_delay >= EXECUTE_ISR){
                    execution += time_count + ", " + EXECUTE_ISR + ", " + "store information in memory\n";
                    time_count += EXECUTE_ISR;
                    ISR_delay -= EXECUTE_ISR;
                }
                else{
                    execution += time_count + ", " + ISR_delay + ", " + "store information in memory\n";
                    time_count += ISR_delay;
                    ISR_delay = 0;
                }
            }
           
        }
       
        /************************************************************************/

    }


    input_file.close();


    write_output(execution);


    return 0;
}

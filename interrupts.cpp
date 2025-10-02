/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
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

    int ms = 0;
    constexpr int SAVE_TIME = 10;
    constexpr int ISR_TIME = 40;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU"){
            execution += std::to_string(ms) + ", " + std::to_string(duration_intr) + ", CPU burst\n";
            ms += duration_intr;
        }

        else if (activity == "SYSCALL"){
            const int device = duration_intr;
            auto [ex, curr_time] = intr_boilerplate(ms, device, SAVE_TIME, vectors);
            execution += ex;
            ms = curr_time;

            int time_left = delays[device];
            while (time_left > 0){
                int time_interval;
                if (time_left >= ISR_TIME){
                    time_interval = ISR_TIME;
                }
                else {
                    time_interval = time_left;
                }
                execution += std::to_string(ms) + ", " + std::to_string(time_interval) + ", " + "call device driver" + "\n";
                ms += time_interval;
                time_left -= time_interval;
            }

            execution += std::to_string(ms) + ", 1, IRET\n";
            ms += 1;
        }
        else if (activity == "END_IO"){
            const int device = duration_intr;
            auto [ex, curr_time] = intr_boilerplate(ms, device, SAVE_TIME, vectors);
            execution += ex;
            ms = curr_time;

            int time_left = delays[device];
            while (time_left > 0){
                int time_interval;
                if (time_left >= ISR_TIME){
                    time_interval = ISR_TIME;
                }
                else {
                    time_interval = time_left;
                }
                execution += std::to_string(ms) + ", " + std::to_string(time_interval) + ", " + "Store information in memory" + "\n";
                ms += time_interval;
                time_left -= time_interval;
            }
            execution += std::to_string(ms) + ", 1, IRET\n";
            ms += 1;
            
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
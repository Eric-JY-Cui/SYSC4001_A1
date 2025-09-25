/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 * source build.sh
 * ./bin/interrupts
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
    int time = 0;


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        //std::cout << execution << std::endl;
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        


        if (activity == "CPU")
        {
            execution.append(std::to_string(time) + ", " + std::to_string(duration_intr) + ", CPU execution\n");
            time = time + duration_intr;
        }
        else if (activity == "SYSCALL")
        {
            execution.append(std::to_string(time) + ", " + "1" + ", Switch to kernel mode\n");
            time = time + 1;
            execution.append(std::to_string(time) + ", " + "4" + ", context saved\n");
            time = time + 4;
            execution.append(std::to_string(time) + ", " + "1" + ", find vector " + std::to_string(duration_intr) + 
                    " in memory " + vectors[duration_intr] + "\n");
            time = time + 1;
            execution.append(std::to_string(time) + ", " + "1" + ", obtain ISR address\n");
            time = time + 1;
            execution.append(std::to_string(time) + ", " + "40" + ", call device driver\n");
            time = time + delays[duration_intr];
            execution.append(std::to_string(time) + ", " + "1" + ", IRET\n");
            time = time + 1;
            
        }
        else if (activity == "END_IO")
        {
            execution.append(std::to_string(time) + ", " + std::to_string(delays[duration_intr]) + ", end of I/O "+ std::to_string(duration_intr)+" interrupt\n");
            time = time + duration_intr;
            
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}

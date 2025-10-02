/**
 *
 * @file interrupts.cpp
 * @author Eric Cui, Aydan Eng
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
        
        
        auto [activity, duration_intr] = parse_trace(trace);
        // std::cout << activity << std::endl;

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        duration_intr = duration_intr - 1;
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

            

            // IO operations
            int remaining_io = delays[duration_intr];
            
            int random_number = rand() % remaining_io - 2;
            execution.append(std::to_string(time) + ", " + std::to_string(random_number) + ", Call device driver\n");
            time = time + random_number;
            remaining_io = remaining_io - random_number;
            
            random_number = rand() % remaining_io - 1;
            execution.append(std::to_string(time) + ", " + std::to_string(random_number) + ", Perform device check\n");
            time = time + random_number;
            remaining_io = remaining_io - random_number;

            random_number = remaining_io;
            execution.append(std::to_string(time) + ", " + std::to_string(random_number) + ", Send device instruction\n");
            time = time + random_number;
            remaining_io = remaining_io - random_number;

            
            //IRET
            execution.append(std::to_string(time) + ", " + "1" + ", IRET\n");
            time = time + 1;
            
        }
        else if (activity == "END_IO")
        {
            execution.append(std::to_string(time) + ", " + "1" + ", Switch to kernel mode\n");
            time = time + 1;
            execution.append(std::to_string(time) + ", " + "4" + ", context saved\n");
            time = time + 4;
            execution.append(std::to_string(time) + ", " + "1" + ", find vector " + std::to_string(duration_intr) + 
                    " in memory " + vectors[duration_intr] + "\n");
            time = time + 1;

            // IO operations
            int remaining_io = delays[duration_intr];
            int random_number = rand() % remaining_io - 2;
            execution.append(std::to_string(time) + ", " + std::to_string(random_number) + ", Store information in memory\n");
            time = time + random_number;
            remaining_io = remaining_io - random_number;
            
            random_number = rand() % remaining_io - 1;
            execution.append(std::to_string(time) + ", " + std::to_string(random_number) + ", reset the io operation\n");
            time = time + random_number;
            remaining_io = remaining_io - random_number;

            random_number = remaining_io;
            execution.append(std::to_string(time) + ", " + std::to_string(random_number) + ", Send standby instruction\n");
            time = time + random_number;
            remaining_io = remaining_io - random_number;

            //IRET
            execution.append(std::to_string(time) + ", " + "1" + ", IRET\n");
            time = time + 1;
            
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
    
    class node {
		public:
        int instructionNumber;
        string operation, baseAddress, location, offset, immediate, Rt, rd, Rs;
        node* next;
        node() {
            instructionNumber = 0;
            operation = "";
            baseAddress = "";
            location = "";
            offset = "";
            immediate = "";
            Rt = "";
            rd = "";
            Rs = "";
            next = nullptr;
        }
        node(int in, string op, string ba, string lo, string offset, string im, string rtr, string rdr, string rsr, node* ne) {
            instructionNumber = in;
            operation = op;
            baseAddress = ba;
            location = lo;
            offset = offset;
            immediate = im;
            Rt = rtr;
            rd = rdr;
            Rs = rsr;
            next = ne;
        }
        ~node() {
            delete next;
        }
    };
    ifstream File(argv[1]); 
	
    vector<string> instructions; 
string currentLine; 
	
    while(getline(File, currentLine)) { // Read in the file while it still has lines to be read
        instructions.push_back(currentLine.substr(0,32)); // Push the read line onto the end of the vector
    }
    
    int currentInstructionNumber = 128; // The number of the current instruction
    
    string currentLineOrInstruction, opcode, operation, previous_operation, output, decodedOutput = ""; // Initialize variables 
	//to hold the current line we are parsing through's contents, the instruction's opcode, the instruction's operation, the 
	//previous instruction's operation, the current instruction's dissambler output, and the decoded instructions (to be read 
	//later to perform operations), respectively
    
    vector<string> decodedInstructions; // A vector to hold our decoded instructions, to be read later to perform operations
    vector<int> registerValues; // A vector to hold the register values
    
    ofstream disassemblyFile; // A variable to hold the disassembly write-to file
    disassemblyFile.open("disassembly.txt"); // Open the disassembly write-to file
    
    node* root = new node();
    node* currentNode = root;
        
    /*
     The following for-loop does the following:
        1. Reads the instructions (from user specified .txt file), decodes them, and stores the decoded instructions into a vector
        2. Reads the register values (from the user specified .txt file) and stores them into a vector
        3. Writes to/creates the disassembly.txt file
    */
    
    for(vector<string>::iterator iter = instructions.begin(); iter != instructions.end(); iter++) { // Loop through the vector
        
        currentLineOrInstruction = *iter; // Save the current line to our variable
        
        string categoryNumber = currentLineOrInstruction.substr(0,2); // Get the current instruction's category number
        
        if(previous_operation != "BREAK") {
        
            if(categoryNumber == "00") { // The instruction is a category 1
                
                opcode = currentLineOrInstruction.substr(2,4); // Get the current instruction's opcode
                
                if(opcode == "0000") { // J instruction
                    
                    operation = "J";
                    
                    int jump_addr = stoi(currentLineOrInstruction.substr(6, 26)); // Get the jump's instruction location
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(jump_addr > 0) {remainder = jump_addr%10; decimal=decimal+(remainder*base); base=base*2; jump_addr=jump_addr/10;} // Convert jump_addr to decimal format
                    string jump_addr_decimal = to_string(4*decimal);
                    string jump_addr_printable = "#" + jump_addr_decimal;
                    
                    decodedOutput = "J " + jump_addr_printable;
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");

                    node* tempNode = new node(currentInstructionNumber, "J", "", jump_addr_decimal, "", "", "", "", "", nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;

                } else if(opcode == "0010") { // BEQ instruction
                    
                    operation = "BEQ";
                    
                    int Rs = stoi(currentLineOrInstruction.substr(6,5)); // Get the Rs register in binary format
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(Rs > 0) {remainder = Rs%10; decimal=decimal+(remainder*base); base=base*2; Rs=Rs/10;} // Convert Rs to decimal format
                    string Rs_string = to_string(decimal);
                    string Rs_printable = "R" + Rs_string; // Full, printable Rs register name
                    
                    int Rt = stoi(currentLineOrInstruction.substr(11,5)); // Get the Rt register in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(Rt > 0) {remainder = Rt%10; decimal=decimal+(remainder*base); base=base*2; Rt=Rt/10;} // Convert Rt to decimal format
                    string Rt_string = to_string(decimal);
                    string Rt_printable = "R" + Rt_string; // Full, printable Rt register name
                    
                    int offset = stoi(currentLineOrInstruction.substr(16,16)); // Get the offset in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(offset > 0) {remainder = offset%10; decimal=decimal+(remainder*base); base=base*2; offset=offset/10;} // Convert offset to decimal format
                    string of_string = to_string(4*decimal);
                    string of_printable = "#" + of_string; // Printable offset value
                    
                    decodedOutput = "BEQ " + Rs_printable + ", " + Rt_printable + ", " + of_printable;
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    node* tempNode = new node(currentInstructionNumber, "BEQ", "", "", of_string, "", Rt_string, "", Rs_string, nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;

                } else if(opcode == "0100") { // BGTZ instruction
                    
                    operation = "BGTZ";
                    
                    int Rs = stoi(currentLineOrInstruction.substr(6,5)); // Get the Rs register in binary format
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(Rs > 0) {remainder = Rs%10; decimal=decimal+(remainder*base); base=base*2; Rs=Rs/10;} // Convert Rs to decimal format
                    string Rs_string = to_string(decimal);
                    string Rs_printable = "R" + Rs_string; // Full, printable Rs register name
                    
                    int offset = stoi(currentLineOrInstruction.substr(16,16)); // Get the offset in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(offset > 0) {remainder = offset%10; decimal=decimal+(remainder*base); base=base*2; offset=offset/10;} // Convert offset to decimal format
                    string of_string = to_string(4*decimal);
                    string of_printable = "#" + of_string; // Printable offset value
                    
                    decodedOutput = "BGTZ " + Rs_printable + ", " + of_printable;
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    node* tempNode = new node(currentInstructionNumber, "BGTZ", "", "", of_string, "", "", "", Rs_string, nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;
                    
                } else if(opcode == "0101") { // BREAK instruction

                    operation = "BREAK";
                    
                    decodedOutput = "BREAK";
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    node* tempNode = new node(currentInstructionNumber, "BREAK", "", "", "", "", "", "", "", nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;
                    
                } else if(opcode == "0110") { // SW instruction

                    operation = "SW";
                    
                    int br = stoi(currentLineOrInstruction.substr(6,5)); // Get the base register in binary format
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(br > 0) {remainder = br%10; decimal=decimal+(remainder*base); base=base*2; br=br/10;} // Convert br to decimal format
                    string br_string = to_string(decimal);
                    string br_printable = "R" + br_string; // Full, printable Rs register name
                    
                    int Rt = stoi(currentLineOrInstruction.substr(11,5)); // Get the Rt register in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(Rt > 0) {remainder = Rt%10; decimal=decimal+(remainder*base); base=base*2; Rt=Rt/10;} // Convert Rt to decimal format
                    string Rt_string = to_string(decimal);
                    string Rt_printable = "R" + Rt_string; // Full, printable Rt register name
                    
                    int offset = stoi(currentLineOrInstruction.substr(16,16)); // Get the offset in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(offset > 0) {remainder = offset%10; decimal=decimal+(remainder*base); base=base*2; offset=offset/10;} // Convert offset to decimal format
                    string of_string = to_string(decimal);
                    string of_printable = of_string; // Printable offset value
                    
                    decodedOutput = "SW " + Rt_printable + ", " + of_printable + "(" + br_printable + ")";
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    node* tempNode = new node(currentInstructionNumber, "SW", br_string, "", of_string, "", Rt_string, "", "", nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;
                    
                } else if(opcode == "0111") { // LW instruction

                    operation = "LW";
                    
                    int br = stoi(currentLineOrInstruction.substr(6,5)); // Get the base register in binary format
                    int decimal = 0; // Initialize decimal number
                    int remainder, base = 1; // Initialize remainder and base
                    while(br > 0) {remainder = br%10; decimal=decimal+(remainder*base); base=base*2; br=br/10;} // Convert br to decimal format
                    string br_string = to_string(decimal);
                    string br_printable = "R" + br_string; // Full, printable Rs register name
                    
                    int Rt = stoi(currentLineOrInstruction.substr(11,5)); // Get the Rt register in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(Rt > 0) {remainder = Rt%10; decimal=decimal+(remainder*base); base=base*2; Rt=Rt/10;} // Convert Rt to decimal format
                    string Rt_string = to_string(decimal);
                    string Rt_printable = "R" + Rt_string; // Full, printable Rt register name
                    
                    int offset = stoi(currentLineOrInstruction.substr(16,16)); // Get the offset in binary format
                    decimal = 0; // Reset decimal number
                    base = 1; // Reset base
                    while(offset > 0) {remainder = offset%10; decimal=decimal+(remainder*base); base=base*2; offset=offset/10;} // Convert offset to decimal format
                    string of_string = to_string(decimal);
                    string of_printable = of_string; // Printable offset value
                    
                    decodedOutput = "LW " + Rt_printable + ", " + of_printable + "(" + br_printable + ")";
                    output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                    
                    node* tempNode = new node(currentInstructionNumber, "LW", br_string, "", of_string, "", Rt_string, "", "", nullptr);
                    if(currentNode != nullptr) {
                        while(currentNode->next != nullptr) {
                            currentNode = currentNode->next;
                        }
                    }
                    currentNode->next = tempNode;
                    currentNode = currentNode->next;
                    
                }

            } else if(categoryNumber == "01") { // The instruction is a category 2
                
                opcode = currentLineOrInstruction.substr(12,4); // Get the current instruction's opcode
                
                if(opcode == "0000") {
                    operation = "ADD";
                } else if(opcode == "0001") {
                    operation = "SUB";
                } else if(opcode == "0010") {
                    operation = "MUL";
                } else if(opcode == "0011") {
                    operation = "AND";
                } else if(opcode == "0100") {
                    operation = "OR";
                } else if(opcode == "0101") {
                    operation = "XOR";
                } else if(opcode == "0110") {
                    operation = "NOR";
                }
                
                int Rs = stoi(currentLineOrInstruction.substr(2,5)); // Rs register number in binary format
                int decimal = 0; // Initialize decimal number
                int remainder, base = 1; // Initialize remainder and base
                while(Rs > 0) {remainder = Rs%10; decimal=decimal+(remainder*base); base=base*2; Rs=Rs/10;} // Convert Rs to decimal format
                string Rs_string = to_string(decimal);
                string Rs_printable = "R" + Rs_string; // Full, printable Rs register name
                
                int Rt = stoi(currentLineOrInstruction.substr(7,5)); // Rt register number in binary format
                decimal = 0; // Reset decimal number
                base = 1; // Reset base
                while(Rt > 0) {remainder = Rt%10; decimal=decimal+(remainder*base); base=base*2; Rt=Rt/10;} // Convert Rt to decimal format
                string Rt_string = to_string(decimal);
                string Rt_printable = "R" + Rt_string; // Full, printable Rt register name
            
                int rd = stoi(currentLineOrInstruction.substr(16,5)); // rd register number in binary format
                decimal = 0; // Reset decimal number
                base = 1; // Reset base
                while(rd > 0) {remainder = rd%10; decimal=decimal+(remainder*base); base=base*2; rd=rd/10;} // Convert rd to decimal format
                string rd_naked = to_string(decimal);
                string rd_printable = "R" + rd_naked; // Full, printable rd register name
                
                decodedOutput = operation + " " + rd_printable + ", " + Rs_printable + ", " + Rt_printable;
                output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                
                node* tempNode = new node(currentInstructionNumber, operation, "", "", "", "", Rt_string, rd_naked, Rs_string, nullptr);
                if(currentNode != nullptr) {
                    while(currentNode->next != nullptr) {
                        currentNode = currentNode->next;
                    }
                }
                currentNode->next = tempNode;
                currentNode = currentNode->next;
                
            } else if(categoryNumber == "10") { // The instruction is a category 3
                
                opcode = currentLineOrInstruction.substr(12,4); // Get the current instruction's opcode
                
                if(opcode == "0000") {
                    operation = "ADDI";
                } else if(opcode == "0001") {
                    operation = "ANDI";
                } else if(opcode == "0010") {
                    operation = "ORI";
                } else if(opcode == "0011") {
                    operation = "XORI";
                }
                
                int Rt = stoi(currentLineOrInstruction.substr(7,5)); // Rt register number in binary format
                //cout << "cat3, Rt: " << currentLineOrInstruction.substr(7,5) << ". int form: " << stoi(currentLineOrInstruction.substr(7,5)) << endl;
                int decimal = 0; // Initialize decimal number
                int remainder, base = 1; // Initialize remainder and base
                while(Rt > 0) {remainder = Rt%10; decimal=decimal+(remainder*base); base=base*2; Rt=Rt/10;} // Convert Rt to decimal format
                string Rt_string = to_string(decimal);
                string Rt_printable = "R" + Rt_string; // Full, printable Rt register name
                
                int Rs = stoi(currentLineOrInstruction.substr(2,5)); // Rs register number in binary format
                //cout << "cat3, Rs: " << currentLineOrInstruction.substr(2,5) << endl;
                decimal = 0; // Reset decimal number
                base = 1; // Reset base
                while(Rs > 0) {remainder = Rs%10; decimal=decimal+(remainder*base); base=base*2; Rs=Rs/10;} // Convert Rs to decimal format
                string Rs_string = to_string(decimal);
                string Rs_printable = "R" + Rs_string; // Full, printable Rs register name
                
                int i_value = stoi(currentLineOrInstruction.substr(16,16)); // immediate value in binary format
                //cout << "cat3, i_value: " << currentLineOrInstruction.substr(16,16) << endl;
                decimal = 0; // Reset decimal number
                base = 1; // Reset base
                while(i_value > 0) {remainder = i_value%10; decimal=decimal+(remainder*base); base=base*2; i_value=i_value/10;} // Convert rd to decimal format
                string i_value_string = to_string(decimal);
                string iv_printable = "#" + i_value_string; // Printable immediate value
                
                decodedOutput = operation + " " + Rt_printable + ", " + Rs_printable + ", " + iv_printable;
                output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + decodedOutput + "\n");
                
                node* tempNode = new node(currentInstructionNumber, operation, "", "", "", i_value_string, Rt_string, "", Rs_string, nullptr);
                if(currentNode != nullptr) {
                    while(currentNode->next != nullptr) {
                        currentNode = currentNode->next;
                    }
                }
                currentNode->next = tempNode;
                currentNode = currentNode->next;
                
            }

            previous_operation = operation;
            
        } else { // The previous operation was BREAK, so we are now reading in the registers' values
            
            signed int registerValue;
            
            // Convert two's complement string to decimal

            if(currentLineOrInstruction.substr(0,1) == "1") { // If the number is negative
                
                string bvs = currentLineOrInstruction.substr(1,31); // Get the two's complement binary value of the number
                
                // Swap the zeros and ones
                replace(bvs.begin(), bvs.end(), '0', 'x');
                replace(bvs.begin(), bvs.end(), '1', '0');
                replace(bvs.begin(), bvs.end(), 'x', '1');
                
                int bv = stoi(bvs) + 1; // Convert the binary string to an integer (we add one because the two's complement was negative)
                
                int decimal = 0;
                int remainder, base = 1;
                while(bv > 0) {remainder = bv%10; decimal=decimal+(remainder*base); base=base*2; bv=bv/10;} // Convert the binary value to its decimal value
                registerValue = decimal*(-1);
                
            } else { // The number is positive
                
                string bvs = currentLineOrInstruction.substr(1,31); // Get the binary value of the number
                
                int bv = stoi(bvs); // Conver the binary string to an integer
                
                int decimal = 0;
                int remainder, base = 1;
                while(bv > 0) {remainder = bv%10; decimal=decimal+(remainder*base); base=base*2; bv=bv/10;} // Convert the binary value to its decimal value
                
                registerValue = decimal;
                
            }
            
            registerValues.push_back(registerValue); // Store the just-computed register value into the registerValues vector for later access
            
            output.append(currentLineOrInstruction + "\t" + to_string(currentInstructionNumber) + "\t" + to_string(registerValue) + "\n");
            
        }
        
        decodedInstructions.push_back(decodedOutput); // Save our decoded instructions to the decodedInstructions vector
        
        currentInstructionNumber = currentInstructionNumber + 4; // Increment the instruction number
        
    }
    
    disassemblyFile << output; // Write to the dissasembly.txt file
    
    ofstream simFile; // A variable to hold the simulation write-to file
    simFile.open("simulation.txt"); // Open the simulation write-to file
    
    // Find which instruction number the register values start (it will be located in the instrNum variable after we find it)
    vector<string> decodedInstructionsCopy = decodedInstructions;
    int instrNum = 128; // Instruction number of where the register values start
    string previousInstructionString = "";
    for(vector<string>::iterator iter = decodedInstructions.begin(); iter != decodedInstructions.end() && previousInstructionString != "BREAK"; iter++) {
        previousInstructionString = *iter;
        instrNum = instrNum + 4;
    }
    
    vector<int> modRegisterValues; // Vector of register values whose contents will be modified as we proceed through the instructions
    
    // Fill the modRegistersValues vector with 0's for every register that exists
    for(int i=0; i<=31; i++) {
        modRegisterValues.push_back(0);
    }
    
    // Set the starting node (instruction)
    currentNode = root->next;
    
    /*
     The loop contained in the if-statement below does the following:
        1. Reads each instruction sequentially
        2. Retrieves the necessary data for the specified operation
        3. Performs the specified operation
        4. Saves the operation's results
        5. Writes (prints) the process to simulation.txt
    */
    if(currentNode->next != nullptr) {
        int cycle = 1;
        while(currentNode != nullptr) {
            
            string currentOperation = currentNode->operation; // Get the current instruction's operation
            
            int currentInstructionNumber = currentNode->instructionNumber; // Get the current instruction number
            string output = "Cycle:" + to_string(cycle) + "\t" + to_string(currentInstructionNumber) + "\t"; // Create the output string
            
            if(currentOperation == "J") {
            
                string instructionLocation = currentNode->location;
                int iil = stoi(instructionLocation); // Instruction location in integer form
                
                node* startingNode = root;
                while(startingNode->next->instructionNumber != iil) { // Find the jump location
                    startingNode = startingNode->next;
                }
                currentNode = startingNode; // Set the current node to the jump location
                
                output.append("J #" + instructionLocation + "\n\n");
                
            } else if(currentOperation == "BEQ") {
                
                string offset = currentNode->offset;
                int io = stoi(offset); // offset in integer form
                string rtReg = currentNode->Rt;
                int irt = modRegisterValues[stoi(rtReg)]; // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = modRegisterValues[stoi(rsReg)]; // Rs register in integer form
                
                int nextInstructionNumber = currentNode->instructionNumber + io; // Get the next instruction number (to be used if Rt == Rs)
                
                if(irt == irs) {
                    node* startingNode = root;
                    while(startingNode->next->instructionNumber != nextInstructionNumber) { // Find the instruction at the offsetted location
                        startingNode = startingNode->next;
                    }
                    currentNode = startingNode->next; // Set the current node to the offsetted location
                }
                
                output.append("BEQ R" + rsReg + ", R" + rtReg + ", #" + offset + "\n\n");

            } else if(currentOperation == "BGTZ") {
                
                string offset = currentNode->offset;
                int io = stoi(offset); // offset in integer form
                string rsReg = currentNode->Rs;
                int irs = modRegisterValues[stoi(rsReg)]; // Rs register in integer form
                
                int nextInstructionNumber = currentNode->instructionNumber + io; // Get the next instruction number (to be used if Rs > 0)
                
                if(irs > 0) {
                    node* startingNode = root;
                    while(startingNode->next->instructionNumber != nextInstructionNumber) { // Find the instruction at the offsetted location
                        startingNode = startingNode->next;
                    }
                    currentNode = startingNode->next; // Set the current node to the offsetted location
                }
                
                output.append("BGTZ R" + rsReg + ", #" + offset + "\n\n");
                
            } else if(currentOperation == "BREAK") {
                
                output.append("BREAK\n\n");
                
            } else if(currentOperation == "SW") {
                
                string baseAddress = currentNode->baseAddress;
                int iba = stoi(baseAddress); // base address in integer form
                string offset = currentNode->offset;
                int io = stoi(offset); // offset in integer form
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                
                // Store the value from the local register specified into the memory register specified
                registerValues[(io - instrNum + modRegisterValues[iba])/4] = modRegisterValues[irt];
                
                output.append("SW R" + rtReg + ", " + offset + "(R" + baseAddress + ")\n\n");
                
            } else if(currentOperation == "LW") {
                
                string baseAddress = currentNode->baseAddress;
                int iba = stoi(baseAddress); // base address in integer form
                string offset = currentNode->offset;
                int io = stoi(offset); // offset in integer form
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                
                // Store the value from the memory register specified into the local register specified
                modRegisterValues[irt] = registerValues[(io - instrNum + modRegisterValues[iba])/4];
                
                output.append("LW R" + rtReg + ", " + offset + "(R" + baseAddress + ")\n\n");
                
            } else if(currentOperation == "ADD") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irs] + modRegisterValues[irt]; // Compute the sum Rs + Rt
                
                output.append("ADD R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "SUB") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irs] - modRegisterValues[irt]; // Compute the difference Rs - Rt
                
                output.append("SUB R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "MUL") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irt]*modRegisterValues[irs]; // Compute the product of Rt and Rs
                
                output.append("MUL R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "AND") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irt]&modRegisterValues[irs]; // Compute the bitwise AND of registers Rt and Rs
                
                output.append("AND R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "OR") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irt]|modRegisterValues[irs]; // Compute the bitwise OR of registers Rt and Rs
                
                output.append("OR R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "XOR") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = modRegisterValues[irt]^modRegisterValues[irs]; // Compute the bitwise XOR of registers Rt and Rs
                
                output.append("XOR R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "NOR") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string rdReg = currentNode->rd;
                int ird = stoi(rdReg); // rd register in integer form
                
                modRegisterValues[ird] = ~(modRegisterValues[irt]|modRegisterValues[irs]); // Compute the bitwise NOR of registers Rt and Rs
                
                output.append("NOR R" + rdReg + ", R" + rsReg + ", R" + rtReg + "\n\n");
                
            } else if(currentOperation == "ADDI") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string immediate = currentNode->immediate;
                int io = stoi(immediate); // immediate value in integer form
                
                modRegisterValues[irt] = modRegisterValues[irs] + io;
                
                output.append("ADDI R" + rtReg + ", R" + rsReg + ", #" + immediate + "\n\n");
                
            } else if(currentOperation == "ANDI") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string immediate = currentNode->immediate;
                int io = stoi(immediate); // immediate value in integer form
                
                modRegisterValues[irt] = modRegisterValues[irs]&io;
                
                output.append("AND R" + rtReg + ", R" + rsReg + ", #" + immediate + "\n\n");
                
            } else if(currentOperation == "ORI") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string immediate = currentNode->immediate;
                int io = stoi(immediate); // immediate value in integer form
                
                modRegisterValues[irt] = modRegisterValues[irs]|io;
                
                output.append("ORI R" + rtReg + ", R" + rsReg + ", #" + immediate + "\n\n");
                
            } else if(currentOperation == "XORI") {
                
                string rtReg = currentNode->Rt;
                int irt = stoi(rtReg); // Rt register in integer form
                string rsReg = currentNode->Rs;
                int irs = stoi(rsReg); // Rs register in integer form
                string immediate = currentNode->immediate;
                int io = stoi(immediate); // immediate value in integer form
                
                modRegisterValues[irt] = modRegisterValues[irs]^io;
                
                output.append("XORI R" + rtReg + ", R" + rsReg + ", #" + immediate + "\n\n");
                
            }
            
            // Create the "Registers" section
            string registersSection = "Registers\n";
            registersSection.append("R00:\t" + to_string(modRegisterValues[0]) + "\t" + to_string(modRegisterValues[1]) + "\t");
            registersSection.append(to_string(modRegisterValues[2]) + "\t" + to_string(modRegisterValues[3]) + "\t");
            registersSection.append(to_string(modRegisterValues[4]) + "\t" + to_string(modRegisterValues[5]) + "\t");
            registersSection.append(to_string(modRegisterValues[6]) + "\t" + to_string(modRegisterValues[7]) + "\n");
            registersSection.append("R08:\t" + to_string(modRegisterValues[8]) + "\t" + to_string(modRegisterValues[9]) + "\t");
            registersSection.append(to_string(modRegisterValues[10]) + "\t" + to_string(modRegisterValues[11]) + "\t");
            registersSection.append(to_string(modRegisterValues[12]) + "\t" + to_string(modRegisterValues[13]) + "\t");
            registersSection.append(to_string(modRegisterValues[14]) + "\t" + to_string(modRegisterValues[15]) + "\n");
            registersSection.append("R16:\t" + to_string(modRegisterValues[16]) + "\t" + to_string(modRegisterValues[17]) + "\t");
            registersSection.append(to_string(modRegisterValues[18]) + "\t" + to_string(modRegisterValues[19]) + "\t");\
            registersSection.append(to_string(modRegisterValues[20]) + "\t" + to_string(modRegisterValues[21]) + "\t");
            registersSection.append(to_string(modRegisterValues[22]) + "\t" + to_string(modRegisterValues[23]) + "\n");
            registersSection.append("R24:\t" + to_string(modRegisterValues[24]) + "\t" + to_string(modRegisterValues[25]) + "\t");
            registersSection.append(to_string(modRegisterValues[26]) + "\t" + to_string(modRegisterValues[27]) + "\t");\
            registersSection.append(to_string(modRegisterValues[28]) + "\t" + to_string(modRegisterValues[29]) + "\t");
            registersSection.append(to_string(modRegisterValues[30]) + "\t" + to_string(modRegisterValues[31]) + "\n\n");
            
            // Create the "Data" section
            string dataSection = "Data\n";
            dataSection.append(to_string(instrNum) + ":\t" + to_string(registerValues[0]) + "\t" + to_string(registerValues[1]) + "\t");
            dataSection.append(to_string(registerValues[2]) + "\t" + to_string(registerValues[3]) + "\t");
            dataSection.append(to_string(registerValues[4]) + "\t" + to_string(registerValues[5]) + "\t");
            dataSection.append(to_string(registerValues[6]) + "\t" + to_string(registerValues[7]) + "\n");
            dataSection.append(to_string(instrNum + 32) + ":\t" + to_string(registerValues[8]) + "\t" + to_string(registerValues[9]) + "\t");
            dataSection.append(to_string(registerValues[10]) + "\t" + to_string(registerValues[11]) + "\t");
            dataSection.append(to_string(registerValues[12]) + "\t" + to_string(registerValues[13]) + "\t");
            if(currentOperation == "BREAK") {
                dataSection.append(to_string(registerValues[14]) + "\t" + to_string(registerValues[15]) + "\n");
            } else {
                dataSection.append(to_string(registerValues[14]) + "\t" + to_string(registerValues[15]) + "\n\n");
            }
            
            // Print the output
            simFile << "--------------------\n";
            simFile << output;
            simFile << registersSection;
            simFile << dataSection;

            currentNode = currentNode->next;
            ++cycle;
        }
    }
    
    return 0;
}

#include <iostream>
#include <stdint.h>

//placeholder ptr for functions that have not been implemented yet
void placeholder(uint32_t instruction){
    return;
}
void placeholder(uint16_t instruction){
    return;
}

//Used to return functions
typedef void (* Func)(uint32_t param);
typedef void (* ThumbFunc)(uint16_t param);

class Instruction {
    public:
        enum instructionFormat {DATAPROCESSING_MISC, LOAD_STORE_WORD_UNSIGNED,
            BRANCH_LINK_OR_TRANSFER, COPROCESSOR_INSTRUCTION, UNCONDITIONAL, UNDEFINED};
        Instruction(uint32_t instruction);
        instructionFormat getSelfFormat();
        uint32_t getData();
        instructionFormat getFormat();
        uint8_t getCondition();
        virtual uint8_t getOp();
        virtual uint8_t getOp1();
        //this decode calls a separate factory function which returns the
        //specific subclass and calls its decode method
        virtual Func decode();
        virtual ~Instruction(){};
    protected:
        Func func;
        instructionFormat format;
        uint32_t data;
};
class InstructionProcessingFunctions {
    public: 
        static Instruction* generateSubClassedInstrct(Instruction* instruction);
};
class InstructionTests {
    public:
        static void testDecode(char* strInstruction);
        static void testThumbDecode(char* strInstruction);
        static void runTests(int argc, char** argv);
};
/*
* BEGIN DATA PROCESSING INSTRUCTIONS:
*   First: Determine if the instruction is a Multiply or a Regular Data Processing instruction
*       Regions:
*           op: 25
*           op1 24 -> 21
*           op2: 7 -> 4
*       Determination: 
*           if !op
*              if op1 == 10xx and op2 == 1xx0
*                   Call function to decode multiply
*              if op1 == 0xxx and op2 == 1001
*                   Call function to decode multiply
*           default:
*               Call function to decode generic data processing instruction
*       Instructions:
*           Multiply:
*               MUL
*               MLA
*               UMAAL
*               UMULL
*               UMLAL
*               SMULL
*               SMLAL
*               SMLAxy
*               SMLAWy
*               SMULWy
*               SMLALxy
*               SMULxy
*           Miscellaneous Load Store Instructions:
*               // Believe me I hate this as much as humanly possible...
*               LDRH
*               STRH
*               LDRSB
*               LDRSH
*           Generic:
*               AND
*               EOR
*               SUB
*               RSB
*               ADD 
*               ADC 
*               SBC
*               TST 
*               TEQ 
*               CMP 
*               CMN 
*               ORR
*               BIC
*               MVN
*                  
*/
class DataProcessingInstrct : public Instruction {
    public:
        DataProcessingInstrct(uint32_t instruction) : Instruction(instruction) {}  
        uint8_t getOp() override;
        uint8_t getOp1() override;
        uint8_t getOp2();
        Func decode() override;
        Func getGenericFuncPtr();
        Func getMultiplyFuncPtr();
        Func getMiscLoadStorePtr();
        ~DataProcessingInstrct() override {};
};
class DataProcessingFunctions {
    public:
        static void bitwiseAnd(uint32_t data);
        static void bitwiseExclusiveOr(uint32_t data);
        static void formPCaddress(uint32_t data);
        static void subtract(uint32_t data);
        static void reverseSubtract(uint32_t data);
        static void addImmeadiate(uint32_t data);
        static void addWithCarry(uint32_t data);
        static void subtractWithCarry(uint32_t data);
        static void testImmeadiate(uint32_t data);
        static void testEquivalence(uint32_t data);
        static void compare(uint32_t data);
        static void compareNegative(uint32_t data);
        static void bitwiseOr(uint32_t data);
        static void move(uint32_t data);
        static void bitwiseBitClear(uint32_t data);
        static void bitwiseNot(uint32_t data);
};
/*
* LOAD STORE WORD UNSIGNED POSSIBLE INSTRUCTIONS
*   LDR
*   LDRB
*   LDRBT
*   LDRT
*   STR
*   STRB
*   STRBT
*   STRT
*/
class LoadStoreWordUnsignedInstrct : public Instruction {
    public:
        LoadStoreWordUnsignedInstrct(uint32_t instruction) : Instruction(instruction){}
        uint8_t getA();
        uint8_t getB();
        uint8_t getOp1() override;
        Func decode() override;  
        ~LoadStoreWordUnsignedInstrct() override {};
};
/*
* BEGIN BRANCH LINK TRANSFER INSTRUCTION
*   Instructions:
*       STMDA
*       LDMDA
*       STM
*       LDMIA
*       POP
*       STMDB
*       PUSH
*       LDMDB
*       STMIB
*       LDMIB
*       STM(user)
*       LDM(user)
*       B
*       BL
*/
class BranchLinkTransferInstrct : public Instruction {
    public:
        BranchLinkTransferInstrct(uint32_t instruction) : Instruction(instruction){}
        uint8_t getOp() override;
        uint8_t getRn();
        uint8_t getR();
        Func decode() override;
        ~BranchLinkTransferInstrct() override {};
};
/*
* BEGIN COPROCESSOR INSTRUCTIONS
* NOT USED
*   STC
*   LDC
*   CPD 
*/
class CoprocessorInstrct : public Instruction {
    public:
        CoprocessorInstrct(uint32_t instruction) : Instruction(instruction){}
        uint8_t getOp() override;
        uint8_t getOp1() override;
        Func decode() override;
        ~CoprocessorInstrct() override {};
};
/*
* THUMB INSTRUCTIONS
*   MOV
*   MVN
*   AND
*   TST
*   BIC
*   ORR
*   EOR
*   LSL
*   LSR
*   ASR
*   ROR
*   ADD
*   SUB
*   SBC
*   NEG
*   CMP
*   CMN
*   MUL
*   B
*   BL
*   B
*   BX
*   SWI
*   BLX
*   POP
*   LDR
*   LDRB
*   LDRH
*   LDSB
*   LDRH
*   STR
*   STRB
*   STRH
*   PUSH
*   STMIA
*   LDMIA
*/
class ThumbInstruction {
    public:
        ThumbInstruction(uint16_t instruction);
        uint16_t getData();
        uint8_t getOp();
        uint8_t getOp1();
        ThumbFunc decode();
    protected:
        uint16_t data;
        ThumbFunc Func;
};
class CPU {
    public:
        //Move to register file class
        enum instructionState  {ARM, THUMB};
        void decode(uint32_t instruction, instructionState mode);
        void decodeArm(uint32_t instruction);
        void decodeThumb(uint16_t instruction);
};
/* CPU CLASS:
*   getMemory(16 bit address, len)
* MEMORY:
*   Load(address width)
*   Store(address value)
*   ByteArray
* REGISTER FILE:
*   getRegister()
*   
* Notes:
*   The device mode can be read from the CPSR (current program status register)
*   it is the first 5 bits
*   program counter is 0b1111
*   stack pointer is 0b1101
*/
void CPU::decode(uint32_t instruction, instructionState mode){
    if (mode == THUMB){
        //decodeThumb(instruction);
    } else {
        decodeArm(instruction);
    }   
}
void CPU::decodeArm(uint32_t instruction){

}
/*
* BEGIN INSTRUCTION METHODS
*   important sectors:
*       condition 28 -> 31 (APPLIES TO ALL)
*       Op1       25 -> 27
*       Op        4
*   First we must look at the condition bits the op1 bits and the op bit
*   , this determines what primary format the instruction is. 
*   DATA PROCESSING DETERMINATIONS:
*      important sectors:
*           Op    25
*           Op1   20 -> 24
*           Op2   4  -> 7
*   LOAD STORE DETERMINATIONS:
*      important sectors:
*           A     25
*           Op1   20 -> 24
*           Rn    16 -> 19
*           B     4
*   MEDIA DETERMINATIONS:
*       important sectors:
*           Op1   20 -> 24
*           Rd    12 -> 15
*           Op2   5  -> 7
*           Rn    0  -> 3
*   COPROCESSOR DETERMINATIONS:
*       important sectors:
*           Op1   20 -> 25
*           Rn    16 -> 19
*           coproc 8 -> 11
*           op     4
*   UNCONDITIONAL DETERMINATIONS:
*       important sectors:
*           Op1   20 -> 27
*           Rn    16 -> 19
*           op     4
*/
Instruction::Instruction(uint32_t instruction){
    this->data = instruction;
    this->format = getFormat();
    std::cout << "Format: " << format << "\n";
}
Func Instruction::decode(){
    Instruction* subclassedInstrct = InstructionProcessingFunctions::generateSubClassedInstrct(this);
    std::cout << "Got subclassed instruction" << "\n";
    return subclassedInstrct->decode();
}
Instruction::instructionFormat Instruction::getSelfFormat(){
    return this->format;
}
uint32_t Instruction::getData(){
    return this->data;
}
uint8_t Instruction::getCondition(){
    //condition is bits 28 -> 31
    uint32_t instruction = this->data;
    uint8_t condition = instruction >> 28;
    return condition;
}
uint8_t Instruction::getOp1(){
    //op1 is is bits 25 -> 27
    uint32_t instruction = this->data;
    uint8_t op1 = instruction >> 25 & 0x08;
    return op1;
}
uint8_t Instruction::getOp(){
    //op is bit 4
    uint32_t instruction = this->data;
    uint8_t op = instruction >> 4 & 1;
    return op; 
}
Instruction::instructionFormat Instruction::getFormat(){
    uint8_t op = (this->data >> 26) & 0b11;
    switch (op){
        case 0b00:
            return DATAPROCESSING_MISC;
        case 0b01:
            return LOAD_STORE_WORD_UNSIGNED;
        case 0b10:
            return BRANCH_LINK_OR_TRANSFER;
        default:
            return UNDEFINED;
    }
}
/*
* BEGIN INSTRUCTIONPROCESSINGFUNCTIONS METHODS
*   A collection of functions used to process instructions into
*   their subclasses based off their format
*/
Instruction* InstructionProcessingFunctions::generateSubClassedInstrct(Instruction* instruction){
    Instruction::instructionFormat format = instruction->getFormat();
    switch (format){
        case Instruction::DATAPROCESSING_MISC:
            return new DataProcessingInstrct(instruction->getData());
        case Instruction::LOAD_STORE_WORD_UNSIGNED:
            return new LoadStoreWordUnsignedInstrct(instruction->getData());
        case Instruction::BRANCH_LINK_OR_TRANSFER:
            return new BranchLinkTransferInstrct(instruction->getData());
        case Instruction::COPROCESSOR_INSTRUCTION:
            return new CoprocessorInstrct(instruction->getData());
        default:
            std::cout << "Throwing" << "\n";
            throw;
    }
}
/*
* BEGIN DATAPROCESSINGINSTRCT METHODS
*/
uint8_t DataProcessingInstrct::getOp(){
    return this->getData() >> 25 & 1;
}
uint8_t DataProcessingInstrct::getOp1(){
    return this->getData() >> 21 & 0b1111;
}
uint8_t DataProcessingInstrct::getOp2(){
    return this->getData() >> 4 & 0b1111;
}
Func DataProcessingInstrct::decode(){
/*
*   DATA PROCESSING DETERMINATIONS:
*      important sectors:
*           Op    25
*           Op1   20 -> 24
*           Op2   4  -> 7
*/
    uint32_t data = this->getData();
    uint8_t op = this->getOp();
    uint8_t op1 = this->getOp1();
    uint8_t op2 = this->getOp2();
    if ((op2 == 0b1011 || op2 == 0b1101 || op2 == 0b1111) && !op){
        return getMiscLoadStorePtr();
    }
    switch (op1){
        case 0b1000:
        case 0b1001:
        case 0b1010:
        case 0b1011:
            if ((op2 & 0b1000) >> 3 && !(op2 & 0b0001)){
                return getMultiplyFuncPtr();
            }
        case 0b0000:
        case 0b0001:
        case 0b0010:
        case 0b0011:
        case 0b0100:
        case 0b0101:
        case 0b0110:
        case 0b0111:
            if (op2 == 0b1001){
                return getMultiplyFuncPtr();
            }
        default:
            return getGenericFuncPtr();
    }
}
Func DataProcessingInstrct::getGenericFuncPtr(){
/*
* POSSIBLE INSTRUCTIONS
*   bitwise AND immeadiate Page 322
*   bitwise Exclusive OR immeadiate page 383
*   Subtract Immeadiate ARM Page 711
*   Reverse Subtract Page 575
*   ADD immeadiate ARM Page 306
*   Add with Carry Page 298
*   Subtract with Carry Page 593
*   Miscellaneous instructions page 205
*   Halfword multiply and multiply accumulate Page 200
*   Test Immeadiate Page 745
*   Test Equivalence Page 739
*   Compare CMP immediate Page 368
*   Compare Negative
*   Bitwise OR immeadiate Page 517
*   Move Immeadiate Page 485
*   Bitwise Bit Clear Page 338
*   Bitwise Not Page 505
*/
    uint32_t data = this->getData();
    uint8_t op = this -> getOp1();
    uint8_t rn = (data >> 16) & 0b1111;
    std::bitset<4> opBits(op);
    std::bitset<4> rnBits(rn);
    std::cout << "op" << opBits << "\n";
    std::cout << "rn" << rnBits << "\n";  
    //switch is faster and somewhat managable here
    switch (op){
        case 0b0000:
            //bitwise AND immeadiate Page 322
            std::cout << "AND" << "\n";
            return &DataProcessingFunctions::bitwiseAnd;
        case 0b0001:
            //bitwise Exclusive OR immeadiate page 383
            std::cout << "EOR" << "\n";
            return &DataProcessingFunctions::bitwiseExclusiveOr;
        case 0b0010:
            //Subtract Immeadiate ARM Page 711
            std::cout << "SUB" << "\n";
            return &DataProcessingFunctions::subtract;
        case 0b0011:
            //Reverse Subtract Page 575
            std::cout << "RSB" << "\n";
            return &DataProcessingFunctions::reverseSubtract;
        case 0b0100:
            //ADD immeadiate ARM Page 306
            std::cout << "ADD" << "\n";
            return &DataProcessingFunctions::addImmeadiate;
        case 0b0101:
            //Add with Carry Page 298
            std::cout << "ADC" << "\n";
            return &DataProcessingFunctions::addWithCarry;
        case 0b0110:
            //Subtract with Carry Page 593
            std::cout << "SBC" << "\n";
            return &DataProcessingFunctions::subtractWithCarry;
        case 0b0111:
            //Test Immeadiate Page 745
            std::cout << "RSC" << "\n";
            return &DataProcessingFunctions::reverseSubtract;
        case 0b1000:
            //Test Immeadiate Page 745
            std::cout << "TST" << "\n";
            return &DataProcessingFunctions::testImmeadiate;
        case 0b1001:
            //Test Equivalence Page 739
            std::cout << "TEQ" << "\n";
            return &DataProcessingFunctions::testEquivalence;
        case 0b1010:
            //Compare CMP immediate Page 368
            std::cout << "CMP" << "\n";
            return &DataProcessingFunctions::compare;
        case  0b1011:
            //Compate Negative 
            std::cout << "CMN" << "\n";
            return &DataProcessingFunctions::compareNegative;
        case 0b1100:
            //Bitwise OR immeadiate Page 517
            std::cout << "ORR" << "\n";
            return &DataProcessingFunctions::bitwiseOr;
        case 0b1101:
            //Move Immeadiate Page 485
            std::cout << "MOV" << "\n";
            return &DataProcessingFunctions::move;
        case 0b1110:
            //Bitwise Bit Clear Page 338
            std::cout << "BIC" << "\n";
            return &DataProcessingFunctions::bitwiseBitClear;
        case 0b1111:
            //Bitwise Not Page 505
            std::cout << "MVN" << "\n";
            return &DataProcessingFunctions::bitwiseNot;
    }
    std::cout << "PLACEHOLDER" << "\n";
    return placeholder;
}
Func DataProcessingInstrct::getMultiplyFuncPtr(){
/*
*   Opcode:
*       bits 24 -> 21
*/
    uint32_t data = this->getData();
    uint8_t op1 = this->getOp1();
    uint8_t x = (data >> 5) & 1;
    switch (op1){
        case 0b0000:
            //Multiply Page 80
            std::cout << "MUL" << "\n";
            return placeholder;
        case 0b0001:
            //Multiply accumulate Page 80
            std::cout << "MLA" << "\n";
            return placeholder;
        case 0b0010:
            //Unsigned Multiply Accumulate significant Long Page 247
            std::cout << "UMAAL" << "\n";
            return placeholder;
        case 0b0100:
            //Unsigned Multiply Long Page 247
            std::cout << "UMULL" << "\n";
            return placeholder;
        case 0b0101:
            //Unsigned Multiply Accumulate Long Page 249
            std::cout << "UMLAL" << "\n";
            return placeholder;
        case 0b0110:
            //Signed Mutliply Long Page 168
            std::cout << "SMULL" << "\n";
            return placeholder;
        case 0b0111:
            //Signed Multiply Accumulate Long Page 247
            std::cout << "SMLAL" << "\n";
            return placeholder;
        case 0b1000:
            //Signed Halfword Multiply Accumulate Long Page 148
            std::cout << "SMLAxy" << "\n";
            return placeholder;
        case 0b1001:
            if (x){
                //Signed Halfword by Word Multiply Long Page 170
                std::cout << "SMULWy" << "\n";
                return placeholder;
            }
            //Signed Halfword by Word Multiply Accumulate Long Page 152
            std::cout << "SMLAWy" << "\n";
            return placeholder;
        case 0b1010:
            //Signed halfword Multiply Accumulate Long Page 148.
            std::cout << "SMLALxy" << "\n";
            return placeholder;
        case 0b1011:
            //Signed halfword Multiply Page 166
            std::cout << "SMULxy" << "\n";
            return placeholder;
        default:
            std::cout << "could not match pattern" << "\n";
            return placeholder;
    }
}
Func DataProcessingInstrct::getMiscLoadStorePtr(){
    uint32_t data = this->getData();
    uint8_t op1 = (data >> 20) & 1;
    uint8_t op2 = (data >> 5) & 0b11;
    switch (op2){
        case 0b01:
            if (op1){
                std::cout << "LDRH" << "\n";
                return placeholder;
            } else {
                std::cout << "STRH" << "\n";
                return placeholder;
            }
        case 0b10:
            std::cout << "LDRSB" << "\n";
            return placeholder;
        case 0b11:
            std::cout << "LDRSH" << "\n";
            return placeholder;
        default:
            std::cout << "PLACEHOLDER" << "\n";
            return placeholder;
    }
}
/*
* BEGIN DATA PROCESSING FUNCTIONS METHODS
*/
void DataProcessingFunctions::bitwiseAnd(uint32_t data){
    return;
}
void DataProcessingFunctions::bitwiseExclusiveOr(uint32_t data){
    return;
}
void DataProcessingFunctions::formPCaddress(uint32_t data){
    return;
}
void DataProcessingFunctions::subtract(uint32_t data){
    return;
}
void DataProcessingFunctions::reverseSubtract(uint32_t data){
    return;
}
void DataProcessingFunctions::addImmeadiate(uint32_t data){
    return;
}
void DataProcessingFunctions::addWithCarry(uint32_t data){
    return;
}
void DataProcessingFunctions::subtractWithCarry(uint32_t data){
    return;
}
void DataProcessingFunctions::testImmeadiate(uint32_t data){
    return;
}
void DataProcessingFunctions::testEquivalence(uint32_t data){
    return;
}
void DataProcessingFunctions::compare(uint32_t data){
    return;
}
void DataProcessingFunctions::compareNegative(uint32_t data){
    return;
}
void DataProcessingFunctions::bitwiseOr(uint32_t data){
    return;
}
void DataProcessingFunctions::move(uint32_t data){
    return;
}
void DataProcessingFunctions::bitwiseBitClear(uint32_t data){
    return;
}
void DataProcessingFunctions::bitwiseNot(uint32_t data){
    return;
}
/*
* BEGIN LOAD STORE WORD UNSIGNED METHODS
*/
uint8_t LoadStoreWordUnsignedInstrct::getA(){
    uint32_t data = this->getData();
    return (data >> 25) & 1;
}
uint8_t LoadStoreWordUnsignedInstrct::getB(){
    uint32_t data = this->getData();
    return (data >> 4) & 1;
}
uint8_t LoadStoreWordUnsignedInstrct::getOp1(){
    uint32_t data = this->getData();
    return (data >> 20) & 0b11111;
}
Func LoadStoreWordUnsignedInstrct::decode(){
    uint8_t a = this->getA();
    uint8_t b = this->getB();
    uint8_t op1 = this->getOp1();
    switch (op1){
        case 0b00000:
        case 0b01000:
        case 0b10000:
        case 0b10010:
        case 0b11000:
        case 0b11010:
            std::cout << "STR" << "\n";
            return placeholder;
        case 0b00010:
        case 0b01010:
            std::cout << "STRT" << "\n";
            return placeholder;
        case 0b00001:
        case 0b01001:
        case 0b10001:
        case 0b10011:
        case 0b11001:
        case 0b11011: 
            std::cout << "LDR" << "\n";
            return placeholder;
        case 0b00011:
        case 0b01011:
            std::cout << "LDRT" << "\n";
            return placeholder;
        case 0b00100:
        case 0b01100:
        case 0b10100:
        case 0b10110:
        case 0b11100:
        case 0b11110:
            std::cout << "STRB" << "\n";
            return placeholder;
        case 0b00110:
        case 0b01110: 
            std::cout << "STRBT" << "\n";
            return placeholder;
        case 0b00101:
        case 0b01101:
        case 0b10101:
        case 0b10111:
        case 0b11101:
        case 0b11111:
            std::cout << "LDRB" << "\n";
            return placeholder;
        case 0b00111:
        case 0b01111:
            std::cout << "LDRBT" << "\n";
            return placeholder;
        default:
            std::cout << "PLACEHOLDER" << "\n";
            return placeholder;
    }
}
/*
* BEGIN BRANCH LINK TRANSFER INSTRUCTION METHODS
*   Instructions:
*       STMDA
*       LDMDA
*       STM
*       LDMIA
*       POP
*       STMDB
*       PUSH
*       LDMDB
*       STMIB
*       LDMIB
*       STM(user)
*       LDM(user)
*       B
*       BL
*/
uint8_t BranchLinkTransferInstrct::getOp(){
    return (this->data >> 20) & 0b111111;
}
uint8_t BranchLinkTransferInstrct::getRn(){
    return (this->data >> 16) & 0b1111;
}
uint8_t BranchLinkTransferInstrct::getR(){
    return (this->data >> 15) & 1;
}
Func BranchLinkTransferInstrct::decode(){
    uint32_t op = this->getOp();
    uint8_t rn = this->getRn();
    uint8_t r = this->getR();
    switch (op){
        case 0b000000:
        case 0b000010:
            std::cout << "STMDA" << "\n";
            return placeholder;
        case 0b000001:
        case 0b000011:
            std::cout << "LDMDA" << "\n";
            return placeholder;
        case 0b001000:
        case 0b001010:
            std::cout << "STM" << "\n";
            return placeholder;
        case 0b001001:
            std::cout << "LDMIA" << "\n";
            return placeholder;
        case 0b001011:
            if (rn == 0b1101){
                std::cout << "POP" << "\n";
                return placeholder;
            }
            std::cout << "LDMIA" << "\n";
            return placeholder;
        case 0b010000:
            std::cout << "STMDB" << "\n";
            return placeholder;
        case 0b010010:
            if (rn == 0b1101){
                std::cout << "PUSH" << "\n";
                return placeholder; 
            }
            std::cout << "STMDB" << "\n";
            return placeholder;
        case 0b010001:
        case 0b010011:
            std::cout << "LDMDB" << "\n";
            return placeholder;
        case 0b011000:
        case 0b011010:
            std::cout << "STMIB" << "\n";
            return placeholder;
        case 0b011001:
        case 0b011011:
            std::cout << "LDMIB" << "\n";
            return placeholder;
        //0b0xx1x0
        case 0b000100:
        case 0b000110:
        case 0b001100:
        case 0b001110:
        case 0b010100:
        case 0b010110:
        case 0b011100:
        case 0b011110:
            std::cout << "STM" << "\n";
            return placeholder;
        //0b0xx1x1
        case 0b000101:
        case 0b000111:
        case 0b001101:
        case 0b001111:
        case 0b010101:
        case 0b010111:
        case 0b011101:
        case 0b011111:
            std::cout << "LDM(user)" << "\n";
            return placeholder;
        default:
            if (((op >> 4) & 0b11) == 0b10){
                std::cout << "B" << "\n";
                return placeholder;
            }
            if (((op >> 4) & 0b11) == 0b11){
                std::cout << "BL" << "\n";
                return placeholder;
            }
            std::cout << "PLACEHOLDER" << "\n";
            return placeholder;
    }
}
/*
* BEGIN COPROCESSOR INSTRUCTION METHODS
* NOT USED
*/
uint8_t CoprocessorInstrct::getOp(){
    return (this->data >> 20) & 1;
}
uint8_t CoprocessorInstrct::getOp1(){
    return (this->data >> 25) & 0b111;
}
Func CoprocessorInstrct::decode(){
    uint8_t op = getOp();
    uint8_t op1 = getOp1();
    if (op1 == 0b111){
        if (op){
            std::cout << "STC" <<  "\n";
            return placeholder;
        }
        std::cout << "LDC" <<  "\n";
        return placeholder;
    }
    if (op1 == 0b110){
        std::cout << "CDP" <<  "\n";
        return placeholder;
    }
    return placeholder;
}
/*
* BEGIN THUMB INSTRUCTION METHODS
*   IMPORTANT REGISTERS:
*       NOTE: I am not going by normal op codes
*       OP: 15 -> 11
*       OP1: 10 -> 8
*       //technically bit 9 also makes some determinants for load store
*   THUMB OP CODES:
*       000xx move shifted register
*           00011x add/subtract
*       001xx move/compare/add/subtract immeadiate
*       01000
*           01000110b CPY
*           else ALU
*       01001 Hi register operations branch exchange
*
*       0101x and bit 9 is 0 load store
*       0101x and bit 9 is 1 load store halfword
*       011xx load store immeadiate offset
*       1000x load store halfword
*       1001x load store halfword 
*       1010x get relative address add
*       10110
*           10110000 add offset to stack pointer
*           else Push
*       1100x multiple load store
*       1101x conditional branch
*               11011111b software interrupt
*               10111110b breakpoint
*       11100 unconditional branch
*       11110 long branch with link
*/
ThumbInstruction::ThumbInstruction(uint16_t instruction){
    this->data = instruction;
}
uint8_t ThumbInstruction::getOp(){
    return (this->data >> 11) & 0b11111;
}
uint8_t ThumbInstruction::getOp1(){
    return (this->data >> 8) & 0b111;
}
ThumbFunc ThumbInstruction::decode(){
    uint8_t op = getOp();
    uint8_t op1 = getOp1();
    uint8_t op2, op3;
    //yes this table is BIG
    switch (op){
        //0b000xx
        case 0b00000:
            std::cout << "LSL" << "\n";
            return placeholder;
        case 0b00001:
            std::cout << "LSR" << "\n";
            return placeholder;
        case 0b00010:
            std::cout << "ASR" << "\n";
            return placeholder;
        case 0b00011:
            op2 = (op1 >> 2) & 1;
            switch (op2){
                case 0:
                    std::cout << "ADD" << "\n";
                    return placeholder;
                case 1:
                    std::cout << "SUB" << "\n";
                    return placeholder;
            }
        //0b001xx
        case 0b00100:
            std::cout << "MOV" << "\n";
            return placeholder;
        case 0b00101:
            std::cout << "CMP" << "\n";
            return placeholder;
        case 0b00110:
            std::cout << "ADD" << "\n";
            return placeholder;
        case 0b00111:
            std::cout << "SUB" << "\n";
            return placeholder;
        //0b01000
        case 0b01000:
            op2 = (this->data >> 6) & 0b1111;
            op3 = (op1 >> 2) & 1;
            if (op3){
                //Hi register operations
                switch (op1){
                    case 0b000:
                    case 0b100:
                        std::cout << "ADD" << "\n";
                        return placeholder;
                    case 0b001:
                    case 0b101:
                        std::cout << "CMP" << "\n";
                        return placeholder;
                    case 0b010:
                    case 0b110:
                        std::cout << "MOV" << "\n";
                        std::cout << "NOP" << "\n";
                        return placeholder;
                    case 0b011:
                    case 0b111:
                        //VERY IMPORTANT: WHEN BIT ZERO OF THE RS VALUE THIS SWITCHES INTO ARM MODE
                        std::cout << "BX" << "\n";
                        std::cout << "BLX" << "\n";
                        return placeholder;
                }
            }
            switch (op2){
                //we use hex here, only time
                case 0x0:
                    std::cout << "AND" << "\n";
                    return placeholder;
                case 0x1:
                    std::cout << "EOR" << "\n";
                    return placeholder;
                case 0x2:
                    std::cout << "LSL" << "\n";
                    return placeholder;
                case 0x3:
                    std::cout << "LSR" << "\n";
                    return placeholder;
                case 0x4:
                    std::cout << "ASR" << "\n";
                    return placeholder;
                case 0x5:
                    std::cout << "ADC" << "\n";
                    return placeholder;
                case 0x6:
                    std::cout << "SBC" << "\n";
                    return placeholder;
                case 0x7:
                    std::cout << "ROR" << "\n";
                    return placeholder;
                case 0x8:
                    std::cout << "TST" << "\n";
                    return placeholder;
                case 0x9:
                    std::cout << "NEG" << "\n";
                    return placeholder;
                case 0xA:
                    std::cout << "CMP" << "\n";
                    return placeholder;
                case 0xB:
                    std::cout << "CMN" << "\n";
                    return placeholder;
                case 0xC:
                    std::cout << "ORR" << "\n";
                    return placeholder;
                case 0xD:
                    std::cout << "MUL" << "\n";
                    return placeholder;
                case 0xE:
                    std::cout << "BIC" << "\n";
                    return placeholder;
                case 0xF:
                    std::cout << "BIC" << "\n";
                    return placeholder;
            }
        case 0b01001:
            std::cout << "LDR" << "\n";
            return placeholder;
        case 0b01010:
            op2 = (op1 >> 2) & 1;
            //determines if its halfword or not
            op3 = (op1 >> 1) & 1;
            if (op2){
                if (op3){
                    std::cout << "LDSB" << "\n";
                    return placeholder;
                }
                std::cout << "STRB" << "\n";
                return placeholder;
            }
            if (op3){
                std::cout << "STRH" << "\n";
                return placeholder;
            }
            std::cout << "STR" << "\n";
            return placeholder;
        case 0b01011:
            op2 = (op1 >> 2) & 1;
            //determines if its halfword or not
            op3 = (op1 >> 1) & 1;
            if (op2){
                if (op3){
                    std::cout << "LDSH" << "\n";
                    return placeholder;
                }
                std::cout << "LDRB" << "\n";
                return placeholder;
            }
            if (op3){
                std::cout << "LDRH" << "\n";
                return placeholder;
            }
            std::cout << "LDR" << "\n";
            return placeholder;
        case 0b01100:
            std::cout << "STR" << "\n";
            return placeholder;
        case 0b01101:
            std::cout << "LDR" << "\n";
            return placeholder;
        case 0b01110:
            std::cout << "STRB" << "\n";
            return placeholder;
        case 0b01111:
            std::cout << "LDRB" << "\n";
            return placeholder;
        case 0b10000:
            std::cout << "STRH" << "\n";
            return placeholder;
        case 0b10001:
            std::cout << "LDRH" << "\n";
            return placeholder;
        case 0b10010:
            std::cout << "STR" << "\n";
            return placeholder;
        case 0b10011:
            std::cout << "LDR" << "\n";
            return placeholder;
        case 0b10110:
            op2 = (op1 >> 1) & 0b11;
            if (op2 == 0b10){
                std::cout << "PUSH" << "\n";
                return placeholder;
            }
            //Add to stack pointer
            std::cout << "ADD" << "\n";
            return placeholder;
        case 0b10111:
            //get relative address
            op2 = (op1 >> 1) & 0b11;
            if (op2 == 0b10){
                std::cout << "POP" << "\n";
                return placeholder;
            }
            std::cout << "ADD" << "\n";
            return placeholder;
        case 0b11000:
            std::cout << "STMIA" << "\n";
            return placeholder;
        case 0b11001:
            std::cout << "LDMIA" << "\n";
            return placeholder;
        case 0b11010:
        case 0b11011:
            op2 = (this->data >> 8) & 0b1111;
            switch (op2){
                case 0x0:
                    std::cout << "BEQ" << "\n";
                    return placeholder;
                case 0x1:
                    std::cout << "BNE" << "\n";
                    return placeholder;
                case 0x2:
                    std::cout << "BCS" << "\n";
                    std::cout << "BHS" << "\n";
                    return placeholder;
                case 0x3:
                    std::cout << "BCC" << "\n";
                    std::cout << "BLO" << "\n";
                    return placeholder;
                case 0x4:
                    std::cout << "BMI" << "\n";
                    return placeholder;
                case 0x5:
                    std::cout << "BPL" << "\n";
                    return placeholder;
                case 0x6:
                    std::cout << "BVS" << "\n";
                    return placeholder;
                case 0x7:
                    std::cout << "BVC" << "\n";
                    return placeholder;
                case 0x8:
                    std::cout << "BHI" << "\n";
                    return placeholder;
                case 0x9:
                    std::cout << "BLS" << "\n";
                    return placeholder;
                case 0xA:
                    std::cout << "BGE" << "\n";
                    return placeholder;
                case 0xB:
                    std::cout << "BLT" << "\n";
                    return placeholder;
                case 0xC:
                    std::cout << "BGT" << "\n";
                    return placeholder;
                case 0xD:
                    std::cout << "BLE" << "\n";
                    return placeholder;
                case 0xF:
                    std::cout << "SWI" << "\n";
                    return placeholder;
            }
        case 0b11100:
            std::cout << "B" << "\n";
            return placeholder;
        case 0b11110:
            //IMPORTANT this is 2 instructions, 32 bit
            //First half
            std::cout << "BL" << "\n";
            std::cout << "BLX" << "\n";
            return placeholder;
        case 0b11111:
            std::cout << "BL" << "\n";
            return placeholder;
        case 0b11101:
            std::cout << "BLX" << "\n";
            return placeholder;
        default:
            std::cout << "PLACEHOLDER" << "\n";
            return placeholder;
    }
}
/*
* BEGIN INSTRUCTION TEST METHODS
*   testDecode: used when a python test module spawns a process using a integer
*   instruction. Converts the instruction to a uint32_t and passes it through the instructions
*   decode methods. On the python side it expects the proper instruction and asserts the correct
*   instruction is printed.
*/
void InstructionTests::testDecode(char* strInstruction){
    std::cout << "String instruction of: " << strInstruction <<"\n";
    uint32_t instruction = (uint32_t)std::strtol(strInstruction, NULL, 10);
    std::bitset<32> y(instruction);
    std::cout << "Recieved instruction of: " << y <<"\n";
    Instruction instrct = Instruction(instruction);
    instrct.decode();
}
void InstructionTests::testThumbDecode(char* strInstruction){
    std::cout << "String instruction of: " << strInstruction <<"\n";
    uint16_t instruction = (uint16_t)std::strtol(strInstruction, NULL, 10);
    std::bitset<16> y(instruction);
    std::cout << "Recieved instruction of: " << y <<"\n";
    ThumbInstruction instrct = ThumbInstruction(instruction);
    instrct.decode();
}
void InstructionTests::runTests(int argc, char** argv){
    if (strcmp( argv[1], "-t") == 0){
        std::cout << "Decoding Thumb Instruction" <<"\n";
        testThumbDecode(argv[2]);
        return;
    }
    testDecode(argv[1]);
}
int main(int argc, char** argv){
    std::cout << "Starting" << "\n";
    InstructionTests::runTests(argc, argv);
    while(1){};
}





//UNUSED

/*
    if (op){
        if (!(op1 >> 3 == 0b10 && op1 & 1)) {
            //Data-processing (immediate) on Page 197
            std::cout << "Instruction is data processing" << "\n";
            return getPtrDataProcessingImmeadiate();
        }
        if (op1 == 0b10000){
            //16-bit immediate load, MOV (immediate) on Page 485
            return placeholder;
        }
        if (op1 ==  0b10100){
            //High halfword 16-bit immediate load, MOVT on Page 492
            return placeholder;
        }
        if (op1 == 0b10110 || op1 == 0b10010){
            //MSR (immediate), and hints on Page 204
            return placeholder;
        }
    } else {
        if (!(op1 >> 3 == 0b10 && op1 & 1)) {
            if (!(op2 & 1)){
                //data processing register Page 195
                return placeholder;
            }
            if (!(op2 & 0b1000) && (op2 & 1)){
                //data processing register shifted gester Page 195
                return placeholder;
            }
        }
        if (op1 >> 3 == 2 && !(op1 & 1)){
            if (!(op2 >> 3)){
                //Miscellaneous instructions page 205
                return placeholder;
            }
            if ((op2 >> 3) && !(op2 & 1)){
                //Halfword multiply and multiply accumulate Page 200
                return placeholder;
            }
        }
        if (op2 == 0b1001){
            if (op1 >> 4){
                //Synchronization primitives on Page 203
                return placeholder;
            } else {
                //Multiply and multiply accumulate on Page 200
                return placeholder;
            }
        }
        if (!(op1 >> 4 && op1 & 0b00010)){
            if (op2 == 0b1011){
                //Extra load/store instructions on page A5-201
                return placeholder;
            }
            if (op2 == 0b1111 || op2 == 0b1101){
                //Extra load/store instructions on page A5-201
                return placeholder;
            }
        }
        if (!(op1 >> 4) && (op1 & 0b11) == 0b10){
            if (op2 == 0b1111 || op2 == 0b1101){
                //Extra load/store instructions on page A5-201
                return placeholder;
            }
        }
        if (!(op1 >> 4) && op1 & 0b10){
            if (op2 == 0b1011){
                //Extra load/store instructions, unprivileged on Page 202
                return placeholder;
            }
        }
        if (!(op1 >> 4) && (op1 & 0b11) == 0b11){
            if (op2 == 0b1111 || op2 == 0b1101){
                //Extra load/store instructions, unprivileged on Page 202
                return placeholder;
            }
        }
    }
    return placeholder;
    */
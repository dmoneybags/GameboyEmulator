import pexpect
from tensorflow.keras.utils import Progbar
'''
The codes used to test the CPU,the test starts a proocess with the code and expects the
correct instruction
'''
class ArmCodes:
    #Instructions are big endian format
    DataProcessingCodes = {
        #Generic instructions

        #AND r7,r3, 9
        "AND" : 0xE2037009,
        #EOR r2,r6, 93;
        "EOR" : 0xE226205D ,
        #SUB r10, r14, 1
        "SUB": 0xE24EA001 ,
        #RSB r3, r4, 10
        "RSB": 0xE264300A ,
        #ADD r3, r4, 10
        "ADD": 0xE284300A ,
        #ADC r3, r4, 10
        "ADC": 0xE2A4300A ,
        #SBC r3, r4, 10
        "SBC": 0xE2C4300A ,
        #TST r3, 10
        "TST": 0xE313000A ,
        #TEQ r3, 10
        "TEQ": 0xE333000A ,
        #CMP r3, 10
        "CMP": 0xE353000A ,
        #CMN r3, 10
        "CMN": 0xE373000A ,
        #ORR r3, 10
        "ORR": 0xE383300A ,
        #MOV R0,#840
        "MOV": 0xE3A00FD2 ,
        #BIC r3, 10
        "BIC": 0xE3C3300A ,
        #MVN r3, 10
        "MVN": 0xE3E0300A ,

        #Multiply instructions

        #MUL r9, r10, r11
        "MUL": 0xE0090B9A ,
        #MLA R7, R8, R9, R3
        "MLA": 0xE0273998 ,
        #UMAAL R7, R8, R9, R3
        "UMAAL": 0xE0487399 ,
        #UMULL R7, R8, R9, R3
        "UMULL": 0xE0887399 ,
        #UMLAL R7, R8, R9, R3
        "UMLAL": 0xE0A87399 ,
        #SMULL R7, R8, R9, R3
        "SMULL": 0xE0C87399 ,
        #SMLAL R7, R8, R9, R3
        "SMLAL": 0xE0E87399 ,
        #SMLABT R7, R8, R9, R3
        "SMLAxy": 0xE10739C8 ,
        #SMLAWT R7, R8, R9, R3
        "SMLAWy": 0xE12739C8 ,
        #SMULWT R7, R8, R9
        "SMULWy": 0xE12709E8 ,
        #SMLALBT R7, R8, R9, R10
        "SMLALxy": 0xE1487AC9 ,
        #SMULBT R7, R8, R9
        "SMULxy": 0xE16709C8
    }
    LoadStoreCodes = {
        #LDR R2, [R0]
        "LDR": 0xE5902000 ,
        #LDRB R2, [R0]
        "LDRB": 0xE5D02000 ,
        #LDRBT R2, [R0]
        "LDRBT": 0xE4F02000 ,
        #LDRH R2, [R0]
        "LDRH": 0xE1D020B0 ,
        #LDRSB R2, [R0]
        "LDRSB": 0xE1D020D0 ,
        #LDRSH R2, [R0]
        "LDRSH": 0xE1D020F0 ,
        #LDRT R2, [R0]
        "LDRT": 0xE4B02000 ,
        #STR R14, [R9]
        "STR": 0xE589E000 ,
        #STRB R11, [R5]
        "STRB": 0xE5C5B000 ,
        #STRBT R6, [R11]
        "STRBT": 0xE4EB6000 ,
        #STRH R5, [R1]
        "STRH": 0xE1C150B0 ,
        #STRT R6, [R2]
        "STRT": 0xE4A26000
    }
    def runDecodeTests(dict, name):
        print(f"Beginning {name} Codes Tests")
        total = len(dict)
        passed = 0
        for key in dict.keys():
            print(key)
            print(dict[key])
            try:
                p = pexpect.spawn("./CPUtest " + str(dict[key]))
                p.expect(key, timeout=1)
                p.close()
                print("Passed!")
                passed += 1   
            except:
                print("Failed!")
        bar = Progbar(total)
        bar.update(passed)
        print(f"we passed {passed/total * 100}% of {name} tests")


if __name__ == "__main__":
    ArmCodes.runDecodeTests(ArmCodes.DataProcessingCodes, "Data Processing")
    ArmCodes.runDecodeTests(ArmCodes.LoadStoreCodes, "Load Store")
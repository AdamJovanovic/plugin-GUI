#pragma once

#include <string>
#include <fstream>
#include "okFrontPanelDLL.h"

//Value for USB BUFFER SIZE comes from rhd2000evalboard header
#define USB_BUFFER_SIZE 2560000
#define USB3_BLOCK_SIZE 1024

class BraingridBoard
{
    //Using rhd2000evalboard as a guide
public:
    BraingridBoard();
    ~BraingridBoard();

    int setup();
    void resetFPGA();
    /**
     * @brief Resets the FPGA. Stops all running processes within FPGA.
     */

    bool uploadFPGABitfile(std::string bitfilePath);
    
    /**
     * @brief Function to reset the FPGA's process to the initial state.
     *          This will clear all recorded data and timers, in addition
     *          to performing a safe shut  down of components such as 
     *          the ADCs, Compressors, etc.
     * 
     * @param toReset If this bool is TRUE, then reset flag is CLEARED in FPGA.
     *                  Make sure this is set to FALSE before running the process.
     * @return true if the subroutine was successful.
     */
    bool resetProcess(bool toReset);
    /**
     * @brief Function to trigger the process start in the FPGA. NOTE: Make sure
     *                  all calibrations have been performed prior to this call.
     * 
     * @param toStart If this bool is TRUE, the start trigger will be executed
     *                  in the FPGA.
     * @return true if the subroutine was successful.
     */
    bool startProcess(bool toStart);

    bool is_file_empty(std::fstream& pFile);
    bool readDataBlock(float** bufferPtr, int numSamples);
    bool isCalibrated();

    void SendStart();
    void SendStop();
    void SendByte(char c);
    void RecvByte();

    void run();
    void stop();
    bool ParseError(okCFrontPanel::ErrorCode error);
private:
    virtual bool calibrateADCs(void){return true;}
    
    okCFrontPanel *dev;  
    std::fstream rawData;
    std::string rawFileName;
    unsigned char usbBuffer[USB_BUFFER_SIZE];

    enum OkEndPointAddresses {
        //TODO: Add more addresses as modules are instantiated
        WireIn_01 = 0x00,
        WireIn_Control_Commands = 0x02,
        WireOut_01 = 0x20,
        WireOut_02 = 0x21,
        WireOut_Debug_Sigs = 0x22,
        TriggerIn_01 = 0x40,
        BlockPipeOut_01 = 0xA0
    };
    
    enum TriggerBits {
        TRIGIN01_START = 0,
        TRIGIN01_STOP = 1,
        RecvReady = 30,
        ByteReady = 31
    };
};

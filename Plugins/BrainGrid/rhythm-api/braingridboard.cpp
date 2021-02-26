#include "braingridboard.h"
#include <iostream>

#include "okFrontPanelDLL.h"

BraingridBoard::BraingridBoard()
{
    rawFileName = "data_record.csv";
    rawData.open(rawFileName,std::ios::out);
    rawData << "Data\n";
    rawData.close();
    //TODO
}

BraingridBoard::~BraingridBoard()
{
    delete dev;
}

int BraingridBoard::setup()
{
    std::string serial = "";
    uint32_t numDevices;
    std::string bitFilePath;
    char dll_date[32], dll_time[32];

    printf("---------- BrainGrid ---------- \n");
    okFrontPanelDLL_GetVersion(dll_date, dll_time);
    printf("Opal Kelly FrontPanel DLL loaded. Built: %s\n", dll_date);

    dev = new okCFrontPanel;    
    numDevices = dev->GetDeviceCount();

    for(size_t i = 0; i < numDevices; ++i)
    {
        okCFrontPanel::BoardModel boardModel = dev->GetDeviceListModel(i);
        if(boardModel == OK_PRODUCT_XEM6310LX45)
        {
            serial = dev->GetDeviceListSerial(i);
            printf("Trying to connect FPGA with serial: %s.\n",serial.c_str());
            if(dev->OpenBySerial(serial) == okCFrontPanel::NoError)
            {
                printf("FPGA connected successfully.\n");
                break;
            }
        }
    }

    if(!dev->IsOpen())
    {
        delete dev;
        std::cerr << "No device could be opened. Is one connected" << std::endl;
    }

    dev->ResetFPGA();
    dev->LoadDefaultPLLConfiguration();

    okCPLL22393 pll;
    dev->GetEepromPLL22393Configuration(pll);

    std::cout << "FPGA system clock: " << pll.GetOutputFrequency(0) << " MHz" << std::endl; // Should indicate 100 MHz
    std::cout << "Opal Kelly device firmware version: " << dev->GetDeviceMajorVersion() << "." <<
            dev->GetDeviceMinorVersion() << std::endl;
    std::cout << "Opal Kelly device serial number: " << dev->GetSerialNumber().c_str() << std::endl;
    std::cout << "Opal Kelly device ID string: " << dev->GetDeviceID().c_str() << std::endl << std::endl;
    return 1;
}

void BraingridBoard::resetFPGA()
{
    ParseError(dev->ResetFPGA());
}

bool BraingridBoard::uploadFPGABitfile(std::string bitfilePath)
{
    okCFrontPanel::ErrorCode configErrorCode = dev->ConfigureFPGA(bitfilePath);
    if(configErrorCode != okCFrontPanel::NoError)
    {
        printf("Bitfile upload failed.\n");
        ParseError(configErrorCode);
        return false;
    }

    //Check that the FPGA has Frontpanel Support (i.e. okHost module instantiated correctly)
    if(dev->IsFrontPanelEnabled())
    {
        printf("FrontPanel support configured correctly.\n");
    }
    else
    {
        printf("FrontPanel not supported.\n");
        return false;
    }

    return true;
}

bool BraingridBoard::resetProcess(bool toReset)
{
    //Reset is an active low. Therefore, to RESET the device, send
    //          1 - (bool)
    std::cout << "Reset set to: " << +(1 - toReset, (1<<0)) << std::endl;
    okCFrontPanel::ErrorCode myError = dev->SetWireInValue(OkEndPointAddresses::WireIn_01, 1 - toReset, (1<<0));
    if(myError == okCFrontPanel::ErrorCode::NoError)
    {
        dev->UpdateWireIns();
        return true;
    }
    else
    {
        ParseError(myError);
        return false;
    }
}

bool BraingridBoard::startProcess(bool toStart)
{
    std::cout << "Start state set to: " << +(toStart==true? TriggerBits::TRIGIN01_START : TriggerBits::TRIGIN01_STOP) << std::endl;
    okCFrontPanel::ErrorCode myError = dev->ActivateTriggerIn(OkEndPointAddresses::TriggerIn_01, (toStart==true? TriggerBits::TRIGIN01_START : TriggerBits::TRIGIN01_STOP));
    if(myError == okCFrontPanel::ErrorCode::NoError)
    {
        return true;

    }
    else
    {
        ParseError(myError);
        return false;
    }
}

bool is_file_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

bool BraingridBoard::readDataBlock(float** bufferPtr, int numSamples)
{
    int blockSize = 8192;//USB3_BLOCK_SIZE;
    long length = numSamples <= USB_BUFFER_SIZE ? numSamples : USB_BUFFER_SIZE;
    float* usbBuff;
    usbBuff = (float*)malloc(sizeof(float)*length);
    long error = dev->ReadFromBlockPipeOut(BlockPipeOut_01, blockSize, length, usbBuffer);
    ParseError(static_cast<okCFrontPanel::ErrorCode>(error));
    
    rawData.open(rawFileName, std::ios::out | std::ios::app);

    for(size_t i = 0; i < length; ++i)
    {
        usbBuff[i] = static_cast<float>(usbBuffer[i]);///4.0);
        if(i < length-1)
        {
            rawData << usbBuff[i] << '\n';
        }
        //UNCOMMENT TO DEBUG INCOMING DATA FROM FPGA: 
        //if(usbBuff[i]) std::cout << "Number is: " << usbBuff[i] << std::endl;
    }
    rawData.close();
    *bufferPtr = usbBuff;
    free(usbBuff);
    return true;
}

bool BraingridBoard::isCalibrated()
{
    //TODO: IMPLEMENT THIS
    return true;
}

void BraingridBoard::SendStart()
{
    dev->SetWireInValue(0x02, 0xFFFFFFFF, 0xFFFFFFFF);
    dev->UpdateWireIns(); 
}

void BraingridBoard::SendStop()
{
    dev->SetWireInValue(0x02, 0x00000000, 0xFFFFFFFF);
    dev->UpdateWireIns();
}

void BraingridBoard::SendByte(char c)
{
    dev->SetWireInValue(0x03, c, 0xFF);
    dev->UpdateWireIns();
    dev->ActivateTriggerIn(OkEndPointAddresses::TriggerIn_01, TriggerBits::ByteReady); 
}

void BraingridBoard::RecvByte()
{
    dev->UpdateWireOuts();
    unsigned long outValue = dev->GetWireOutValue(OkEndPointAddresses::WireOut_Debug_Sigs);
    std::cout << outValue << std::endl;
}

bool BraingridBoard::ParseError(okCFrontPanel::ErrorCode error)
{
	switch (error) {
	case okCFrontPanel::DeviceNotOpen:
		printf("Device no longer available.\n");
		exit(EXIT_FAILURE);
	case okCFrontPanel::Failed:
		std::cerr << "Transfer failed." <<std::endl;
		exit(EXIT_FAILURE);
	case okCFrontPanel::Timeout:
		std::cerr << "   ERROR: Timeout." << std::endl;
		return false;
	case okCFrontPanel::TransferError:
		std::cerr << "   ERROR: TransferError." << std::endl;
		return false;
	case okCFrontPanel::UnsupportedFeature:
        std::cout << "   ERROR: UnsupportedFeature." << std::endl;
		std::cerr << "   ERROR: UnsupportedFeature." << std::endl;
		return false;
		case okCFrontPanel::InvalidEndpoint:
		printf("	ERROR: Invalid Endpoint address.\n");
		return false;
	case okCFrontPanel::InvalidBlockSize:
		printf("	ERROR: Invalid Block Size.\n");
		return false;
	case okCFrontPanel::DataAlignmentError:
		printf("	ERROR: Invalid Block Size.\n");
		return false;
//TODO: Fill in the remaining error codes
	case okCFrontPanel::DoneNotHigh:
	case okCFrontPanel::CommunicationError:
	case okCFrontPanel::InvalidBitstream:
	case okCFrontPanel::FileError:
	case okCFrontPanel::I2CRestrictedAddress:
	case okCFrontPanel::I2CBitError:
	case okCFrontPanel::I2CUnknownStatus:
	case okCFrontPanel::I2CNack:
	case okCFrontPanel::FIFOUnderflow:
	case okCFrontPanel::FIFOOverflow:
	case okCFrontPanel::InvalidParameter:
		std::cerr << "   ERROR: " << error << std::endl;
		return false;
	default:
		return true;
	}
}

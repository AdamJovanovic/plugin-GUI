/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "BrainGridThread.h"
#include "BrainGridEditor.h"
#include "rhythm-api/braingridboard.h"

using namespace BrainGrid;

#if defined(_WIN32)
#define okLIB_NAME "okFrontPanel.dll"
#define okLIB_EXTENSION "*.dll"
#elif defined(__APPLE__)
#define okLIB_NAME "libokFrontPanel.dylib"
#define okLIB_EXTENSION "*.dylib"
#elif defined(__linux__)
#define okLIB_NAME "./libokFrontPanel.so"
#define okLIB_EXTENSION "*.so"
#endif

//#define INIT_STEP ( evalBoard->isUSB3() ? 256 : 60)

DataThread* BrainGridThread::createDataThread(SourceNode *sn)
{
    return new BrainGridThread(sn);
}

BrainGridThread::BrainGridThread(SourceNode *sn): DataThread(sn),
    deviceFound(false),
    sampleRate(16384.0), 
    currentBufferUpdate(0)
{  
    std::cout << "BrainGridThread started: " << std::endl;
    braingridBoard = new BraingridBoard;
    sourceBuffers.add(new DataBuffer(3, 30000)); // start with 2 channels and automatically resize
    File executable = File::getSpecialLocation(File::currentExecutableFile);
    const String executableDirectory = executable.getParentDirectory().getFullPathName();
    
    String dirName = executableDirectory;
    libraryFilePath = dirName;
    libraryFilePath += File::separatorString;
    libraryFilePath += okLIB_NAME;

    if(openBoard())
    {
        initializeBoard();
    } 

    //evalBoard->setSampleRate(Rhd2000EvalBoard::SampleRate30000Hz);
    //Initialize FPGA
}

BrainGridThread::~BrainGridThread()
{
    std::cout << "BrainGrid interface destroyed." << std::endl;
    if(deviceFound)
    {
        braingridBoard->resetFPGA();
    }
    //TODO:
}

bool BrainGridThread::foundInputSource()
{
    return deviceFound;
}

int BrainGridThread::getNumDataOutputs(DataChannel::DataChannelTypes type, int subProcessor) const
{
    return 1;
}

unsigned int BrainGridThread::getNumSubProcessors() const
{
    return 1;
}

int BrainGridThread::getNumTTLOutputs(int subprocessor) const
{
    if (subprocessor > 0) return 0;
    return 0;
}

bool BrainGridThread::usesCustomNames() const
{
    return true;
}

float BrainGridThread::getSampleRate(int subprocessor) const
{
    //TODO
    return sampleRate;
}

bool BrainGridThread::setSampleRate(float aRate)
{
    sampleRate = aRate;
    return true;
}

float BrainGridThread::getBitVolts(const DataChannel* chan) const
{
    return 0.195f;
}

int BrainGridThread::getNumChannels() const
{
    return 1;
}

void BrainGridThread::enableBoardLeds(bool enable)
{
    //evalBoard->enableBoardLeds(enable);
}

void BrainGridThread::resetBoard(bool toReset)
{
    braingridBoard->resetProcess(toReset);
}

void BrainGridThread::startBoard(bool toStart)
{
    braingridBoard->startProcess(toStart);
}

bool BrainGridThread::isChipCalibrated(void)
{
    return braingridBoard->isCalibrated();
}

GenericEditor* BrainGridThread::createEditor(SourceNode *sn)
{
    return new BrainGridEditor(sn, this, true);
}

bool BrainGridThread::openBoard()
{
    int return_code = braingridBoard->setup();

    if (return_code == 1)
    {
        deviceFound = true;
    }
    else if (return_code == -1) // dynamic library not found
    {
        bool response = AlertWindow::showOkCancelBox(AlertWindow::NoIcon,
                                                     "Opal Kelly library not found.",
                                                     "The Opal Kelly library file was not found in the directory of the executable. Would you like to browse for it?",
                                                     "Yes", "No", 0, 0);
        if (response)
        {
            // browse for file
            FileChooser fc("Select the library file...",
                           File::getCurrentWorkingDirectory(),
                           okLIB_EXTENSION,
                           true);

            if (fc.browseForFileToOpen())
            {
                File currentFile = fc.getResult();
                libraryFilePath = currentFile.getFullPathName();
                openBoard(); // call recursively
            }
            else
            {
                //sendActionMessage("No configuration selected.");
                deviceFound = false;
            }

        }
        else
        {
            deviceFound = false;
        }
    }
    else if (return_code == -2)   // board could not be opened
    {
        bool response = AlertWindow::showOkCancelBox(AlertWindow::NoIcon,
                                                     "Acquisition board not found.",
                                                     "An acquisition board could not be found. Please connect one now.",
                                                     "OK", "Cancel", 0, 0);

        if (response)
        {
            openBoard(); // call recursively
        }
        else
        {
            deviceFound = false;
        }

    }

    return deviceFound;

}

bool BrainGridThread::uploadBitfile(String pathToBitfile)
{

    deviceFound = true;

    if (!braingridBoard->uploadFPGABitfile(pathToBitfile.toStdString()))
    {
        std::cout << "Couldn't upload bitfile from " << pathToBitfile << std::endl;

        /*bool response = AlertWindow::showOkCancelBox(AlertWindow::NoIcon,
                                                     "FPGA bitfile not found.",
                                                     (evalBoard->isUSB3() ?
                                                     "The rhd2000_usb3.bit file was not found in the directory of the executable. Would you like to browse for it?" :
                                                     "The rhd2000.bit file was not found in the directory of the executable. Would you like to browse for it?"),
                                                     "Yes", "No", 0, 0);
        if (response)
        {
            // browse for file
            FileChooser fc("Select the FPGA bitfile...",
                           File::getCurrentWorkingDirectory(),
                           "*.bit",
                           true);

            if (fc.browseForFileToOpen())
            {
                File currentFile = fc.getResult();

                uploadBitfile(currentFile.getFullPathName()); // call recursively
            }
            else
            {
                //sendActionMessage("No configuration selected.");
                deviceFound = false;
            }

        }
        else
        {
            deviceFound = false;
        }
        */

    }

    return deviceFound;

}

void BrainGridThread::initializeBoard()
{
    String bitfilename;

    File executable = File::getSpecialLocation(File::currentExecutableFile);
    const String executableDirectory = executable.getParentDirectory().getFullPathName();

    bitfilename = executableDirectory;
    bitfilename += File::separatorString;
    bitfilename += "braingrid.bit";

    if (!uploadBitfile(bitfilename))
    {
        return;
    }

    //braingridBoard->initialize();
}

//
void BrainGridThread::scanSPIPorts()
{

}

void BrainGridThread::setDefaultChannelNames()
{

}

bool BrainGridThread::updateBuffer()
{
    float* usbBuffer;
    int numSamples = 16384;
    
    braingridBoard->readDataBlock(&usbBuffer, numSamples);
    
    for(auto i=0; i<numSamples; ++i)
    {
        std::cout << usbBuffer[i] << ", ";
        if(i%4 == 0)
        {
            std::cout << std::endl;
        }
    }
    

    int numWritten = sourceBuffers[0]->addToBuffer(usbBuffer,&timestamps.getReference(0),&ttlEventWords.getReference(0),numSamples,numSamples);
    
    /*
    if(numWritten != numSamples)
    {
        std::cout << "Numwritten: " << +numWritten << ", NumSamples: " << +numSamples << std::endl;
        //std::raise(SIGTERM);
    }
    else
    {
        std::cout << "This is fine" << std::endl;
    }
    */
    //std::cout << currentBufferUpdate++ << std::endl;
    
    
    //BrainGridThread::stopAcquisition();
    return true;
}

void BrainGridThread::timerCallback()
{
    stopTimer();
}

bool BrainGridThread::startAcquisition()
{
    
   
    //std::cout << "Expecting " << getNumChannels() << " channels." << std::endl;

    //memset(filter_states,0,256*sizeof(double));

    //std::cout << "Setting max timestep." << std::endl;
    //evalBoard->setMaxTimeStep(100);
    braingridBoard->startProcess(true);
    startThread();
    isTransmitting = true;

    return true;
}

bool BrainGridThread::stopAcquisition()
{
/* /*  */
        //  isTransmitting = false;
    std::cout << "Stopping acquisition." << std::endl;
    braingridBoard->startProcess(false);
    if (isThreadRunning())
    {
        signalThreadShouldExit();

    }

    if (waitForThreadToExit(500))
    {
        std::cout << "Thread exited." << std::endl;
    }
    else
    {
        std::cout << "Thread failed to exit, continuing anyway..." << std::endl;
    }
    sourceBuffers[0]->clear();
    isTransmitting = false;
    //dacOutputShouldChange = false;
    return true;
}

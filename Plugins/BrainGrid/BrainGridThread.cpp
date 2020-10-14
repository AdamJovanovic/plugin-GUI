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

#define INIT_STEP ( evalBoard->isUSB3() ? 256 : 60)

DataThread* BrainGridThread::createDataThread(SourceNode *sn)
{
    return new BrainGridThread(sn);
}

BrainGridThread::BrainGridThread(SourceNode *sn): DataThread(sn),
    deviceFound(false)
{  
     std::cout << "BrainGridThread started: \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    \
    " << std::endl;
    evalBoard = new Rhd2000EvalBoard;
    sourceBuffers.add(new DataBuffer(2, 10000)); // start with 2 channels and automatically resize
    File executable = File::getSpecialLocation(File::currentExecutableFile);
    const String executableDirectory = executable.getParentDirectory().getFullPathName();
    
    String dirName = executableDirectory;
    libraryFilePath = dirName;
    libraryFilePath += File::separatorString;
    libraryFilePath += okLIB_NAME;

    if(openBoard(libraryFilePath))
    {
        dataBlock = new Rhd2000DataBlock(1, evalBoard->isUSB3());

        initializeBoard();

        if (evalBoard->isUSB3())
        std::cout << "USB3 board mode enabled" << std::endl;
    } 
    evalBoard->setSampleRate(Rhd2000EvalBoard::SampleRate30000Hz);
    //Initialize FPGA
}

BrainGridThread::~BrainGridThread()
{
    std::cout << "BrainGrid interface destroyed." << std::endl;

    //TODO:
}

bool BrainGridThread::foundInputSource()
{
    return deviceFound;
}

int BrainGridThread::getNumDataOutputs(DataChannel::DataChannelTypes type, int subProcessor) const
{
    return DataChannel::DataChannelTypes::HEADSTAGE_CHANNEL;
}

unsigned int BrainGridThread::getNumSubProcessors() const
{
    return 8;
}

int BrainGridThread::getNumTTLOutputs(int subprocessor) const
{
    if (subprocessor > 0) return 0;
    return 8;
}

bool BrainGridThread::usesCustomNames() const
{
    return true;
}

float BrainGridThread::getSampleRate(int subprocessor) const
{
    return evalBoard->getSampleRate();
}

float BrainGridThread::getBitVolts(const DataChannel* chan) const
{
    return 1.0;
}

void BrainGridThread::enableBoardLeds(bool enable)
{
    evalBoard->enableBoardLeds(enable);
}

GenericEditor* BrainGridThread::createEditor(SourceNode *sn)
{
    return new BrainGridEditor(sn, this, true);
}

bool BrainGridThread::openBoard(String pathToLibrary)
{
    int return_code = evalBoard->open(pathToLibrary.getCharPointer());

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
                openBoard(libraryFilePath); // call recursively
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
            openBoard(libraryFilePath.getCharPointer()); // call recursively
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

    if (!evalBoard->uploadFpgaBitfile(pathToBitfile.toStdString()))
    {
        std::cout << "Couldn't upload bitfile from " << pathToBitfile << std::endl;

        bool response = AlertWindow::showOkCancelBox(AlertWindow::NoIcon,
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
    bitfilename += evalBoard->isUSB3() ? "rhd2000_usb3.bit" : "rhd2000.bit";

    if (!uploadBitfile(bitfilename))
    {
        return;
    }

    evalBoard->initialize();

    //setSampleRate(Rhd2000EvalBoard::SampleRate30000Hz);

    evalBoard->setMaxTimeStep(INIT_STEP);
    evalBoard->setContinuousRunMode(false);

    // Start SPI interface
    evalBoard->run();

    // Wait for the 60-sample run to complete
    while (evalBoard->isRunning())
    {
        ;
    }

    int ledArray[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    evalBoard->setLedDisplay(ledArray);
}

void BrainGridThread::setDefaultChannelNames()
{

}

bool BrainGridThread::updateBuffer()
{
    return true;
}

void BrainGridThread::timerCallback()
{

}

bool BrainGridThread::startAcquisition()
{
    
    //impedanceThread->waitSafely();
    dataBlock = new Rhd2000DataBlock(evalBoard->getNumEnabledDataStreams(), evalBoard->isUSB3());

    //std::cout << "Expecting " << getNumChannels() << " channels." << std::endl;

    //memset(filter_states,0,256*sizeof(double));

    int ledArray[8] = {1, 1, 0, 0, 0, 0, 0, 0};
    evalBoard->setLedDisplay(ledArray);

    cout << "Number of 16-bit words in FIFO: " << evalBoard->numWordsInFifo() << endl;
    cout << "Is eval board running: " << evalBoard->isRunning() << endl;


    //std::cout << "Setting max timestep." << std::endl;
    //evalBoard->setMaxTimeStep(100);


    std::cout << "Starting acquisition." << std::endl;
    if (1)
    {
        // evalBoard->setContinuousRunMode(false);
        //  evalBoard->setMaxTimeStep(0);
        std::cout << "Flushing FIFO." << std::endl;
        evalBoard->flush();
        evalBoard->setContinuousRunMode(true);
        //evalBoard->printFIFOmetrics();
        evalBoard->run();
        //evalBoard->printFIFOmetrics();
    }

    blockSize = dataBlock->calculateDataBlockSizeInWords(evalBoard->getNumEnabledDataStreams(), evalBoard->isUSB3());
    std::cout << "Expecting blocksize of " << blockSize << " for " << evalBoard->getNumEnabledDataStreams() << " streams" << std::endl;
    //evalBoard->printFIFOmetrics();
    startThread();


    isTransmitting = true;

    return true;
}

bool BrainGridThread::stopAcquisition()
{
/* /*  */
        //  isTransmitting = false;
    std::cout << "RHD2000 data thread stopping acquisition." << std::endl;

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

    if (deviceFound)
    {
        evalBoard->setContinuousRunMode(false);
        evalBoard->setMaxTimeStep(0);
        std::cout << "Flushing FIFO." << std::endl;
        evalBoard->flush();
        //   evalBoard->setContinuousRunMode(true);
        //   evalBoard->run();

    }

    sourceBuffers[0]->clear();

    if (deviceFound)
    {
        cout << "Number of 16-bit words in FIFO: " << evalBoard->numWordsInFifo() << endl;

        // std::cout << "Stopped eval board." << std::endl;


        int ledArray[8] = {1, 0, 0, 0, 0, 0, 0, 0};
        evalBoard->setLedDisplay(ledArray);
    }

    isTransmitting = false;
    //dacOutputShouldChange = false;
    return true;
}

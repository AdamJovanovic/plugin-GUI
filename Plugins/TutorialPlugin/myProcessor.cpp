#include "myProcessor.h"
#include <time.h>

using namespace myProcessorSpace;

//Change all names for the relevant ones, including "Processor Name"
myProcessor::myProcessor() : GenericProcessor("Processor Name")
{
    std::cout << "Processor created" << std::endl;
    myTimer.start();
}

myProcessor::~myProcessor()
{

}

void myProcessor::process(AudioSampleBuffer& buffer)
{
	/** 
	If the processor needs to handle events, this method must be called onyl once per process call
	If spike processing is also needing, set the argument to true
	*/
	//checkForEvents(false);
	int numChannels = getNumOutputs();

	for (int chan = 0; chan < numChannels; chan++)
	{
		int numSamples = getNumSamples(chan);
		int64 timestamp = getTimestamp(chan);
        if((static_cast<uint64_t>(myTimer.elapsedSeconds())%2) < 1)
        {
            buffer.clear(chan, 10000);
        }
		//Do whatever processing needed
	}
	 
}

//This prevents include loops. We recommend changing the macro to a name suitable for your plugin
#ifndef MYPROCESSOR_H_DEFINED
#define MYPROCESSOR_H_DEFINED

#include <ProcessorHeaders.h>
#include <chrono>
//namespace must be an unique name for your plugin
namespace myProcessorSpace
{
    class aTimer
    {
    public:
        void start()
        {
            m_StartTime = std::chrono::system_clock::now();
            m_bRunning = true;
        }
        
        void stop()
        {
            m_EndTime = std::chrono::system_clock::now();
            m_bRunning = false;
        }
        
        double elapsedMilliseconds()
        {
            std::chrono::time_point<std::chrono::system_clock> endTime;
            
            if(m_bRunning)
            {
                endTime = std::chrono::system_clock::now();
            }
            else
            {
                endTime = m_EndTime;
            }
            
            return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
        }
        
        double elapsedSeconds()
        {
            return elapsedMilliseconds() / 1000.0;
        }

    private:
        std::chrono::time_point<std::chrono::system_clock> m_StartTime;
        std::chrono::time_point<std::chrono::system_clock> m_EndTime;
        bool                                               m_bRunning = false;
    };
    
	class myProcessor : public GenericProcessor
	{
	public:
		/** The class constructor, used to initialize any members. */
		myProcessor();

		/** The class destructor, used to deallocate memory */
		~myProcessor();

		/** Indicates if the processor has a custom editor. Defaults to false */
		//bool hasEditor() const { return true; }

		/** If the processor has a custom editor, this method must be defined to instantiate it. */
		//AudioProcessorEditor* createEditor() override;

		/** Optional method that informs the GUI if the processor is ready to function. If false acquisition cannot start. Defaults to true */
		//bool isReady();

		/** Defines the functionality of the processor.
		The process method is called every time a new data buffer is available.
		Processors can either use this method to add new data, manipulate existing
		data, or send data to an external target (such as a display or other hardware).
		Continuous signals arrive in the "buffer" variable, event data (such as TTLs
		and spikes) is contained in the "events" variable.
		*/
		void process(AudioSampleBuffer& buffer) override;

		/** Handles events received by the processor
		Called automatically for each received event whenever checkForEvents() is called from process()		
		*/
		//void handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition) override;

		/** Handles spikes received by the processor
		Called automatically for each received event whenever checkForEvents(true) is called from process()
		*/
		//void handleSpike(const SpikeChannel* spikeInfo, const MidiMessage& event, int samplePosition) override;

		/** The method that standard controls on the editor will call.
		It is recommended that any variables used by the "process" function
		are modified only through this method while data acquisition is active. */
		//void setParameter(int parameterIndex, float newValue) override;

		/** Saving custom settings to XML. */
		//void saveCustomParametersToXml(XmlElement* parentElement) override;

		/** Load custom settings from XML*/
		//void loadCustomParametersFromXml() override;

		/** Optional method called every time the signal chain is refreshed or changed in any way.
		Allows the processor to handle variations in the channel configuration or any other parameter
		passed down the signal chain. The processor can also modify here the settings structure, which contains
		information regarding the input and output channels as well as other signal related parameters. Said
		structure shouldn't be manipulated outside of this method.
		*/
		//void updateSettings() override;
    private:
        aTimer myTimer;

	};


}

#endif
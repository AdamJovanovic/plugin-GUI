/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2014 Open Ephys

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

#include "BrainGridEditor.h"
#include <cmath>

#include "BrainGridThread.h"

using namespace BrainGridNode;

#ifdef WIN32
#if (_MSC_VER < 1800) //round doesn't exist on MSVC prior to 2013 version
inline double round(double x)
{
    return floor(x+0.5);
}
#endif
#endif

BrainGridEditor::BrainGridEditor(GenericProcessor* parentNode,
                             BrainGridThread* board_,
                             bool useDefaultParameterEditors
                            )
    : VisualizerEditor(parentNode, useDefaultParameterEditors)
{
    canvas = nullptr;
    desiredWidth = 340;
    tabText = "BrainGrid";
/*     measureWhenRecording = false;
    saveImpedances = false;

    impedanceData = new ImpedanceData();
    impedanceData->valid = false; */

/* 
    // add headstage-specific controls (currently just a toggle button)
    for (int i = 0; i < 4; i++)
    {
        HeadstageOptionsInterface* hsOptions = new HeadstageOptionsInterface(board, this, i);
        headstageOptionsInterfaces.add(hsOptions);
        addAndMakeVisible(hsOptions);
        hsOptions->setBounds(3, 28+i*20, 70, 18);
    }
 */
    // add rescan button
    rescanButton = new UtilityButton("RESCAN", Font("Small Text", 13, Font::plain));
    rescanButton->setRadius(3.0f);
    rescanButton->setBounds(6, 108, 65, 18);
    rescanButton->addListener(this);
    rescanButton->setTooltip("Check for connected headstages");
    addAndMakeVisible(rescanButton);

/*     // add sample rate selection
    sampleRateInterface = new SampleRateInterface(board, this);
    addAndMakeVisible(sampleRateInterface);
    sampleRateInterface->setBounds(80, 20, 80, 50);
 */
/* 
    // add Bandwidth selection
    bandwidthInterface = new BandwidthInterface(board, this);
    addAndMakeVisible(bandwidthInterface);
    bandwidthInterface->setBounds(80, 55, 80, 50);
 */
    auxButton = new UtilityButton("AUX", Font("Small Text", 13, Font::plain));
    auxButton->setRadius(3.0f);
    auxButton->setBounds(80, 108, 32, 18);
    auxButton->addListener(this);
    auxButton->setClickingTogglesState(true);
    auxButton->setTooltip("Toggle AUX channels (3 per headstage)");
    addAndMakeVisible(auxButton);

    adcButton = new UtilityButton("ADC", Font("Small Text", 13, Font::plain));
    adcButton->setRadius(3.0f);
    adcButton->setBounds(80+32+1, 108, 32, 18);
    adcButton->addListener(this);
    adcButton->setClickingTogglesState(true);
    adcButton->setTooltip("Toggle 8 external HDMI ADC channels");
    addAndMakeVisible(adcButton);
/* 
    audioLabel = new Label("audio label", "Audio out");
    audioLabel->setBounds(170, 20, 75, 15);
    audioLabel->setFont(Font("Small Text", 10, Font::plain));
    audioLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(audioLabel);
 */
    for (int i = 0; i < 2; i++)
    {
        ElectrodeButton* button = new ElectrodeButton(-1);
        electrodeButtons.add(button);

        button->setBounds(174+i*30, 35, 30, 15);
        button->setChannelNum(-1);
        button->setToggleState(false, dontSendNotification);
        button->setRadioGroupId(999);

        addAndMakeVisible(button);
        button->addListener(this);

        if (i == 0)
        {
            button->setTooltip("Audio monitor left channel");
        }
        else
        {
            button->setTooltip("Audio monitor right channel");
        }
    }
/* 
    // add HW audio parameter selection
    audioInterface = new AudioInterface(board, this);
    addAndMakeVisible(audioInterface);
    audioInterface->setBounds(174, 55, 70, 50);

    clockInterface = new ClockDivideInterface(board, this);
    addAndMakeVisible(clockInterface);
    clockInterface->setBounds(174, 80, 70, 50);
 */
    // add DSP Offset Button
    dspoffsetButton = new UtilityButton("DSP:", Font("Small Text", 13, Font::plain));
    dspoffsetButton->setRadius(3.0f); // sets the radius of the button's corners
    dspoffsetButton->setBounds(174, 108, 32, 18); // sets the x position, y position, width, and height of the button
    dspoffsetButton->addListener(this);
    dspoffsetButton->setClickingTogglesState(true); // makes the button toggle its state when clicked
    dspoffsetButton->setTooltip("Toggle DSP offset removal");
    addAndMakeVisible(dspoffsetButton); // makes the button a child component of the editor and makes it visible
    dspoffsetButton->setToggleState(true, dontSendNotification);
/* 
    // add DSP Frequency Selection field
    dspInterface = new DSPInterface(board, this);
    addAndMakeVisible(dspInterface);
    dspInterface->setBounds(174+32, 108, 40, 50);
 */
    dacTTLButton = new UtilityButton("DAC TTL", Font("Small Text", 13, Font::plain));
    dacTTLButton->setRadius(3.0f);
    dacTTLButton->setBounds(260, 25, 60, 18);
    dacTTLButton->addListener(this);
    dacTTLButton->setClickingTogglesState(true);
    dacTTLButton->setTooltip("Toggle DAC Threshold TTL Output");
    addAndMakeVisible(dacTTLButton);
/* 
    dacHPFlabel = new Label("DAC HPF", "DAC HPF");
    dacHPFlabel->setFont(Font("Small Text", 10, Font::plain));
    dacHPFlabel->setBounds(255, 40, 60, 20);
    dacHPFlabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(dacHPFlabel);
 */
    dacHPFcombo = new ComboBox("dacHPFCombo");
    dacHPFcombo->setBounds(260, 55, 60, 18);
    dacHPFcombo->addListener(this);
    dacHPFcombo->addItem("OFF", 1);
    int HPFvalues[10] = {50, 100, 200, 300, 400, 500, 600, 700, 800, 900};
    for (int k=0; k<10; k++)
    {
        dacHPFcombo->addItem(String(HPFvalues[k])+" Hz", 2+k);
    }
    dacHPFcombo->setSelectedId(1, sendNotification);
    addAndMakeVisible(dacHPFcombo);
/* 
    ttlSettleLabel = new Label("TTL Settle", "TTL Settle");
    ttlSettleLabel->setFont(Font("Small Text", 10, Font::plain));
    ttlSettleLabel->setBounds(255, 70, 70, 20);
    ttlSettleLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(ttlSettleLabel);
 */
    ttlSettleCombo = new ComboBox("FastSettleComboBox");
    ttlSettleCombo->setBounds(260, 85, 60, 18);
    ttlSettleCombo->addListener(this);
    ttlSettleCombo->addItem("-",1);
    for (int k=0; k<8; k++)
    {
        ttlSettleCombo->addItem("TTL"+String(1+k), 2+k);
    }
    ttlSettleCombo->setSelectedId(1, sendNotification);
    addAndMakeVisible(ttlSettleCombo);

    ledButton = new UtilityButton("LED", Font("Small Text", 13, Font::plain));
    ledButton->setRadius(3.0f);
    ledButton->setBounds(288, 108, 32, 18);
    ledButton->addListener(this);
    ledButton->setClickingTogglesState(true);
    ledButton->setTooltip("Toggle board LEDs");
    addAndMakeVisible(ledButton);
    ledButton->setToggleState(true, dontSendNotification);

}

Visualizer* BrainGridEditor::createNewCanvas()
{
    GenericProcessor* processor = (GenericProcessor*) getProcessor();
    canvas= new FPGAcanvas(processor);
    //ActionListener* listener = (ActionListener*) canvas;
    //getUIComponent()->registerAnimatedComponent(listener);
    return canvas;
}
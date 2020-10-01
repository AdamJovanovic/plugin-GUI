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

#ifndef __BrainGridEditor_H__
#define __BrainGridEditor_H__

#include <VisualizerEditorHeaders.h>
#include "BrainGridEditor.h"
#include "BrainGridThread.h"

namespace BrainGridNode
{
    class BrainGridEditor;
	class FPGAcanvas;

    class FPGAcanvas : public Visualizer, public Button::Listener
	{   //TO BE CHANGED LATER
	public:
		FPGAcanvas(GenericProcessor* n);
		~FPGAcanvas();

		void paint(Graphics& g);

		void refresh();

		void beginAnimation();
		void endAnimation();

		void refreshState();
		void update();

		void setParameter(int, float);
		void setParameter(int, int, int, float);

		void updateImpedance(Array<int> streams, Array<int> channels, Array<float> magnitude, Array<float> phase);

		void resized();
		void buttonClicked(Button* button);
		ScopedPointer<Viewport> channelsViewport;
		SourceNode* processor;
	};


    class BrainGridEditor: public VisualizerEditor, public ComboBox::Listener, public AsyncUpdater
    {
    public:
        BrainGridEditor(GenericProcessor* parentNode, BrainGridThread* , bool useDefaultParameterEditors);
        ~BrainGridEditor(){}


        void combBoxChanged(ComboBox* comboBox);
        Visualizer* createNewCanvas();

        void handleAsyncUpdate();

    private:
    //		OwnedArray<HeadstageOptionsInterface> headstageOptionsInterfaces;
            OwnedArray<ElectrodeButton> electrodeButtons;

    //		ScopedPointer<SampleRateInterface> sampleRateInterface;
    //		ScopedPointer<BandwidthInterface> bandwidthInterface;
    //		ScopedPointer<DSPInterface> dspInterface;

    //		ScopedPointer<AudioInterface> audioInterface;
    //		ScopedPointer<ClockDivideInterface> clockInterface;

            ScopedPointer<UtilityButton> rescanButton, dacTTLButton;
            ScopedPointer<UtilityButton> auxButton;
            ScopedPointer<UtilityButton> adcButton;
            ScopedPointer<UtilityButton> ledButton;

            ScopedPointer<UtilityButton> dspoffsetButton;
            ScopedPointer<ComboBox> ttlSettleCombo, dacHPFcombo;


            ScopedPointer<Label> audioLabel, ttlSettleLabel, dacHPFlabel;

    };

}
#endif  // __BrainGridEditor_H__

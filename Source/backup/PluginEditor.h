/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class ChorusFxAudioProcessorEditor  : public AudioProcessorEditor, 
    private Slider::Listener,
    public Button::Listener,
    public Timer

{
public:
    ChorusFxAudioProcessorEditor (ChorusFxAudioProcessor&);
    ~ChorusFxAudioProcessorEditor();

    CustomLookAndFeel knobStyle;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback();
    void sliderValueChanged(Slider* slider) override;
    void buttonClicked(Button*);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChorusFxAudioProcessor& processor;
    Label delayLabel_, sweepWidthLabel_, depthLabel_;
    Slider delaySlider_, sweepWidthSlider_, depthSlider_;
    ToggleButton stereoToggleButton_;
    ScopedPointer<ResizableCornerComponent> resizer_;
    ComponentBoundsConstrainer resizeLimits_;    

    ChorusFxAudioProcessor* getProcessor() const
    {
        return static_cast <ChorusFxAudioProcessor*> (getAudioProcessor());
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusFxAudioProcessorEditor)
};

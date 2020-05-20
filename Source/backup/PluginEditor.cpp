/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusFxAudioProcessorEditor::ChorusFxAudioProcessorEditor (ChorusFxAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Set up the sliders
    delaySlider_.setSliderStyle(Slider::Rotary);
    delaySlider_.addListener(this);
    delaySlider_.setRange(0.00, 0.03);
    addAndMakeVisible(&delaySlider_);

    sweepWidthSlider_.setSliderStyle(Slider::Rotary);
    sweepWidthSlider_.addListener(this);
    sweepWidthSlider_.setRange(0.00, 0.005);
    addAndMakeVisible(&sweepWidthSlider_);

    depthSlider_.setSliderStyle(Slider::Rotary);
    depthSlider_.addListener(this);
    depthSlider_.setRange(0.0, 1.0, 0.01);
    addAndMakeVisible(&depthSlider_);
    
/*
    delayLabel_.attachToComponent(&delaySlider_, false);
    delayLabel_.setText("Delay amount", dontSendNotification);
    delayLabel_.setFont(Font(11.0f));

    sweepWidthLabel_.attachToComponent(&sweepWidthSlider_, false);
    sweepWidthLabel_.setText("Sweep width", dontSendNotification);
    sweepWidthLabel_.setFont(Font(11.0f));

    depthLabel_.attachToComponent(&depthSlider_, false);
    depthLabel_.setText("Depth", dontSendNotification);
    depthLabel_.setFont(Font(11.0f));
    */
    addAndMakeVisible(resizer_ = new ResizableCornerComponent(this, &resizeLimits_));
    resizeLimits_.setSizeLimits(550, 200, 600, 300);

    setSize(600, 200);
    // startTimer(50);
}

ChorusFxAudioProcessorEditor::~ChorusFxAudioProcessorEditor()
{
}

//==============================================================================
void ChorusFxAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void ChorusFxAudioProcessorEditor::resized() //POSITIONING
{
    delaySlider_.setBounds(20, 20, 140, 40);
    sweepWidthSlider_.setBounds(200, 20, 140, 40);
    depthSlider_.setBounds(380, 20, 140, 40);
    resizer_->setBounds(getWidth() - 16, getHeight() - 16, 16, 16);
}

void ChorusFxAudioProcessorEditor::sliderValueChanged(Slider* slider) 
{
    if (slider == &delaySlider_)
    {
        //getProcessor()->setParameterNotifyingHost(ChorusFxAudioProcessor::kDelayParam, (float)delaySlider_.getValue());
       processor.set_amt_delay(delaySlider_.getValue());
    }
    else if (slider == &sweepWidthSlider_)
    {
        // getProcessor()->setParameterNotifyingHost(ChorusFxAudioProcessor::kSweepWidthParam, (float)sweepWidthSlider_.getValue());
        processor.set_sweep_width(sweepWidthSlider_.getValue());
    }
    else if (slider == &depthSlider_)
    {
        // getProcessor()->setParameterNotifyingHost(ChorusFxAudioProcessor::kDepthParam, (float)depthSlider_.getValue());
        processor.set_wet(depthSlider_.getValue());
    }
}

// This timer periodically checks whether any of the filter's parameters have changed...
void ChorusFxAudioProcessorEditor::timerCallback()
{
    //stereoToggleButton_.setToggleState((ourProcessor->stereo_ != 0), dontSendNotification);
}


// Callback for toggle button
void ChorusFxAudioProcessorEditor::buttonClicked(Button* button)
{
   
}
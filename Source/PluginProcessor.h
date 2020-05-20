/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <math.h>

//==============================================================================
/**
*/
class ChorusFxAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ChorusFxAudioProcessor();
    ~ChorusFxAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


private:
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChorusFxAudioProcessor)

    AudioSampleBuffer delaybuf;
    AudioProcessorValueTreeState kparam;
    int writepos;
    int readpos;
    foleys::MagicProcessorState GUI{ *this, kparam };


    float depth;
    float LFO_freq;
    int Gdpw; //global state variable
    int Gphi;// global state vriable
    const float amt_delay = 0.020f; 
    float sweep_width;
    float phase;
    float offset;

    //********************************************************************************************//
    //getters
    float get_next_phase(float phase);
    float linear_int(float delay, int delaylen);
    float read_LFO(float phase);

};

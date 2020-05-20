/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include <math.h>

//==============================================================================
ChorusFxAudioProcessor::ChorusFxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    writepos = 0;
    readpos = 0;
    wet = 0.5;
    dry = 1 - wet;
    LFO_freq = 2.0;
    sweep_width = 0.005f; //5ms
    phase = 0;

    //global variables
    Gdpw = 0;
    offset = 0.25f;
    Gphi = 0;

}

ChorusFxAudioProcessor::~ChorusFxAudioProcessor()
{
}

//==============================================================================
const String ChorusFxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChorusFxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChorusFxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChorusFxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChorusFxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChorusFxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChorusFxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChorusFxAudioProcessor::setCurrentProgram (int index)
{
}

const String ChorusFxAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChorusFxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ChorusFxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    delaybuf.setSize(getTotalNumInputChannels(), sampleRate + samplesPerBlock + 3);
    delaybuf.clear();


}

void ChorusFxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChorusFxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ChorusFxAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int numSamples = buffer.getNumSamples();
    int delaylen = delaybuf.getNumSamples();

    float wet_now = wet;
    float dry_now = 1-wet;
    float sw_now = sweep_width;
    float frequency_now = LFO_freq;
    float delayL = 0.0;
    float delayR = 0.0;

    float* channelOutDataL = buffer.getWritePointer(0);
    float* channelOutDataR = buffer.getWritePointer(1);

        for (int i = 0; i < numSamples; ++i) {
             float inputL = buffer.getSample(0,i);
             float inputR = buffer.getSample(1, i);

            //current delay for both channels
            delayL = amt_delay + sw_now * read_LFO(phase);
            delayR = amt_delay + sw_now * read_LFO(phase+offset);

            float outL = dry_now * inputL/2 + linear_int(delayL, delaylen) * (1 - dry_now);
            float outR = dry_now * inputR/2 + linear_int(delayR, delaylen) * (1 - dry_now);

            channelOutDataL[i] = outL;
            channelOutDataR[i] = outR;

            phase = get_next_phase(phase);

            delaybuf.setSample(0, writepos, inputL);
            delaybuf.setSample(1, writepos, inputR);
            writepos = (writepos + 1) % delaylen;
        }

        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear(i, 0, buffer.getNumSamples());

    }


//==============================================================================
bool ChorusFxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChorusFxAudioProcessor::createEditor()
{
    return new ChorusFxAudioProcessorEditor (*this);
}

//==============================================================================
void ChorusFxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ChorusFxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float ChorusFxAudioProcessor::linear_int(float delay, int delaylen) {
    float dpr = fmodf((float)writepos - (float)(delay * getSampleRate()) + (float)delaylen - 3.0, (float)delaylen);
    float diff = dpr - floorf(dpr);
    int prev = floorf(dpr);
    int nxt = (prev + 1) % delaylen;

     return diff * delaybuf.getSample(0, nxt) + (1.0 - diff) * delaybuf.getSample(0, prev);
}

float ChorusFxAudioProcessor::get_next_phase(float phi) {
    float isr = 1.0 / getSampleRate(); // better to multiply rather than divide!
    float frequency_now = LFO_freq;
    phi += static_cast<float>(frequency_now * isr);
    if (phi >= 1.0)
        phi -= 1.0;
    return phi;
}

float ChorusFxAudioProcessor::read_LFO(float phase) {
    return sinf(0.5 + 0.5 * sinf(2.0f * (float)M_PI * phase));
}
void ChorusFxAudioProcessor::set_wet(float val) {
    wet = val;
}
void ChorusFxAudioProcessor::set_amt_delay(float val)
{
}

void ChorusFxAudioProcessor::set_sweep_width(float val)
{
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusFxAudioProcessor();
}

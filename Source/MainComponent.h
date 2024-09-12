#pragma once

#include <JuceHeader.h>
#include "SynthAudioSource.h"
#include "WaveformComponent.h"

class MainComponent : public juce::AudioAppComponent, public juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    void timerCallback();
    void setMidiInput(int index);

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    double currentSampleRate;
    int bufferSize;
    int phase = 0;
    juce::Random random;
    juce::AudioDeviceManager audioDeviceManager;
    float level = 0.0f;
    //juce::OwnedArray<WaveTableOscillator> oscillators;
    juce::AudioSampleBuffer waveTable;
    const unsigned int tableSize = 1 << 7;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    SynthAudioSource synthAudioSource;
    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;
    WaveformComponent waveformComponent;
    juce::Slider dutyCycleSlider;
    juce::Label dutyCycleLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

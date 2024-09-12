#pragma once

#include <JuceHeader.h>

class WaveformComponent  : public juce::Component, public juce::Timer
{
public:
    WaveformComponent();
    ~WaveformComponent() override;
    
    void paint(juce::Graphics& g) override;
    void setWaveform(const std::vector<float>& newWaveform);

    void timerCallback() override;

private:
    std::vector<float> waveform;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformComponent)
};

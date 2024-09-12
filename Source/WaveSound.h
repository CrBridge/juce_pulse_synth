#pragma once

#include <JuceHeader.h>

struct WaveSound : public juce::SynthesiserSound
{
    WaveSound();

    bool appliesToNote(int) override;
    bool appliesToChannel(int) override;
};

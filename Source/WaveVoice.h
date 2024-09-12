#pragma once

#include <JuceHeader.h>
#include "WaveSound.h"

class WaveVoice : public juce::SynthesiserVoice
{
public:
    WaveVoice();

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override;
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    void stopNote(float /*velocity*/, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void setDutyCycle(float newDutyCycle);

private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
    enum
    {
        gainIndex
    };
    juce::dsp::ProcessorChain < juce::dsp::Gain<float>> processorChain;
    float dutyCycle = 0.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveVoice)
};

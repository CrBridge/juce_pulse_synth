#include <JuceHeader.h>
#include "SynthAudioSource.h"

SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState& keyState, WaveformComponent& wfComponent)
    : keyboardState(keyState), waveformComponent(wfComponent)
{
    // set polyphony
    for (auto i = 0; i < 5; ++i)
        synth.addVoice(new WaveVoice());

    synth.addSound(new WaveSound());
}

void SynthAudioSource::setUsingWaveSound()
{
    synth.clearSounds();
}

void SynthAudioSource::prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    midiCollector.reset(sampleRate);
}

void SynthAudioSource::releaseResources() {}

void SynthAudioSource::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    juce::MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);

    keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
        bufferToFill.numSamples, true);

    synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
        bufferToFill.startSample, bufferToFill.numSamples);

    if (bufferToFill.buffer->getNumChannels() > 0)
    {
        auto* channelData = bufferToFill.buffer->getReadPointer(0);
        std::vector<float> audioData(channelData, channelData + bufferToFill.numSamples);
        waveformComponent.setWaveform(audioData);
    }
}

juce::MidiMessageCollector* SynthAudioSource::getMidiCollector()
{
    return &midiCollector;
}

void SynthAudioSource::setDutyCycle(float newDutyCycle)
{
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WaveVoice*>(synth.getVoice(i)))
        {
            voice->setDutyCycle(newDutyCycle);
        }
    }
}

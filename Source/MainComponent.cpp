#include<JuceHeader.h>
#include "MainComponent.h"

MainComponent::MainComponent()
    : synthAudioSource(keyboardState, waveformComponent),
    keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    addAndMakeVisible(keyboardComponent);
    addAndMakeVisible(waveformComponent);
    setSize(1000, 220);
    setAudioChannels(0, 2);
    startTimer(400);

    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText("MIDI Input:", juce::dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);

    auto midiInputs = juce::MidiInput::getAvailableDevices();
    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");

    juce::StringArray midiInputNames;
    for (auto input : midiInputs)
        midiInputNames.add(input.name);

    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };

    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled(input.identifier))
        {
            setMidiInput(midiInputs.indexOf(input));
            break;
        }
    }

    if (midiInputList.getSelectedId() == 0)
        setMidiInput(0);

    addAndMakeVisible(dutyCycleSlider);
    dutyCycleSlider.setRange(0.0, 1.0, 0.01);
    dutyCycleSlider.setValue(0.5);
    dutyCycleSlider.onValueChange = [this]() 
    {
        synthAudioSource.setDutyCycle(dutyCycleSlider.getValue());
    };

    addAndMakeVisible(dutyCycleLabel);
    dutyCycleLabel.setText("Duty Cycle", juce::dontSendNotification);
    dutyCycleLabel.attachToComponent(&dutyCycleSlider, false);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

void MainComponent::setMidiInput(int index)
{
    auto list = juce::MidiInput::getAvailableDevices();

    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier,
        synthAudioSource.getMidiCollector());

    auto newInput = list[index];

    if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

    deviceManager.addMidiInputDeviceCallback(newInput.identifier, synthAudioSource.getMidiCollector());
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);

    lastInputIndex = index;
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    synthAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    synthAudioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    synthAudioSource.releaseResources();
    //
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey);
    g.setColour(juce::Colours::white);
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    midiInputList.setBounds(area.removeFromTop(30).reduced(10, 0));
    keyboardComponent.setBounds(area.removeFromBottom(getHeight() / 3).reduced(10, 10));

    waveformComponent.setBounds(area.removeFromTop(60).reduced(10, 0));

    dutyCycleSlider.setBounds(area.removeFromTop(60).reduced(10, 0));
}
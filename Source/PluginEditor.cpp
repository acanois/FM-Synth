//
// Created by David Richter on 12/31/24.
//

#include "PluginEditor.h"

FMSynthEditor::FMSynthEditor(FMSynthProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p), mKeyboardComponent(
          mKeyboardState,
          juce::MidiKeyboardComponent::horizontalKeyboard
      ) {
    juce::ignoreUnused(processorRef);

    mKeyboardState.addListener(this);

    addAndMakeVisible(mKeyboardComponent);

    setSize(1000, 600);
}

FMSynthEditor::~FMSynthEditor() = default;

void FMSynthEditor::paint(juce::Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    auto area = getLocalBounds();

    juce::Rectangle<int> componentBounds;
    componentBounds.setBounds(100, 100, 100, 100);
    mKeyboardComponent.setBounds(area.removeFromBottom(80).reduced(8));
}

void FMSynthEditor::resized() {
}

void FMSynthEditor::handleNoteOn(
    juce::MidiKeyboardState *source,
    int midiChannel,
    int midiNoteNumber,
    float velocity
) {
    // DBG("FMSynthEditor::handleNoteOn - midi note: " << midiNoteNumber);
}

void FMSynthEditor::handleNoteOff(
    juce::MidiKeyboardState *source,
    int midiChannel,
    int midiNoteNumber,
    float velocity
) {
}

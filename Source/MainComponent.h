#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// The JUCE LookAndFeel class alows the engineer to modify elements of the GUI
// OtherLookAndFeel is an edited copy of the original LookAndFeel class
// that replaces elements of the GUI with external .png images
class OtherLookAndFeel : public LookAndFeel_V2
{
public:
	OtherLookAndFeel()
	{
	}

	// This function modifies the Slider Class
	void drawLinearSlider(Graphics& g, //Graphics& a,
			int x, int y, int w, int h,
			float sliderPos, float minSliderPos, float maxSliderPos,
			const Slider::SliderStyle style,
			Slider& slider)
		{
				// Image variables
				Image faderKnob;
				Image onMeter;
				Image offmeter;

				// Retrieve .pnf files from the project and convert to Image Class
				faderKnob = ImageCache::getFromMemory(BinaryData::cap_png, BinaryData::cap_pngSize);
				onMeter = ImageCache::getFromMemory(BinaryData::on_png, BinaryData::on_pngSize);
				offmeter= ImageCache::getFromMemory(BinaryData::off_png, BinaryData::off_pngSize);
			
				// Set the fader knob and meter dimensions and positions
				g.drawImage(faderKnob, // image    
					faderKnob.getWidth() / 2 - onMeter.getWidth(), sliderPos - faderKnob.getHeight() / 2, faderKnob.getWidth() * 2, faderKnob.getHeight(), //dest
					0, 0, faderKnob.getHeight(), faderKnob.getHeight()); // source
				g.drawImage(offmeter, // image    
					slider.getWidth() - onMeter.getWidth(), slider.getHeight() - minSliderPos, onMeter.getWidth(), slider.getHeight()*0.95,   // dest
					0, 0, onMeter.getWidth(), onMeter.getHeight()); // source
				
				// Reveals or excludes meter-on image
				exclude(g, onMeter, sliderPos, maxSliderPos, minSliderPos, slider);
				
				// Draw slider text
				g.drawFittedText(slider.getTextValueSuffix(),
					5, -20,
					slider.getWidth(), slider.getHeight(),
					Justification::centredTop, 10);
	}
	// This function creates an image and then excludes certain regions of the image 
	// from being displayed
	void exclude(Graphics& a, Image onMeter, float sliderPos, float maxSliderPos, float minSliderPos, Slider& slider)
	{
		// Set area to be excluded
		int widthExtend = 10;
		Rectangle<int> area = onMeter.getBounds();
		area.setHeight((int)(sliderPos));
		area.setWidth(slider.getWidth() + widthExtend);
		area.setPosition(slider.getWidth() - onMeter.getWidth()-5, 0);
		
		// Exclude area of image and draw remaining section
		a.excludeClipRegion(area);
		a.drawImage(onMeter, // image    
			slider.getWidth() - onMeter.getWidth(), slider.getHeight() - minSliderPos, onMeter.getWidth(), slider.getHeight()*0.95,   // dest
			0, 0, onMeter.getWidth(), onMeter.getHeight()); // source
	}

	// Function modified from TickBox to create toggled On/Off Mute Button
	void drawMuteOn(Graphics& g, Component& component,
		float x, float y, float w, float h,
		const bool ticked,
		const bool isEnabled,
		const bool isMouseOverButton,
		const bool isButtonDown)
	{

		// Define and retreive Mute Off Image
		Image muteOff;
		muteOff = ImageCache::getFromMemory(BinaryData::normal2x_png, BinaryData::normal2x_pngSize);

		g.drawImage(muteOff, // image    
			0, 0, muteOff.getWidth(), muteOff.getHeight(),   // dest
			0, 0, muteOff.getHeight(), muteOff.getHeight()); // source

		// If button is selected draw muteSelected Image
		if (ticked)
		{
			Image muteSelected;
			muteSelected = ImageCache::getFromMemory(BinaryData::selected2x_png, BinaryData::selected2x_pngSize);
			g.drawImage(muteSelected, // image    
				0, 0, muteSelected.getWidth(), muteSelected.getHeight(),   // dest
				0, 0, muteSelected.getHeight(), muteSelected.getHeight()); // source
		}

	}

	// Function modified from Toggle button to draw the Mute button
	void drawToggleButton(Graphics& g, ToggleButton& button,
			bool isMouseOverButton, bool isButtonDown)
		{
			// Draw eitehr selected or deselected button
			drawMuteOn(g, button, 0, 0,
				button.getWidth(), button.getHeight(),
				button.getToggleState(),
				button.isEnabled(),
				isMouseOverButton,
				isButtonDown);

			// Draw text
			g.setColour(Colours::black);
			g.setFont(15.0);
			g.drawFittedText(button.getButtonText(),
				5, 7,
				button.getWidth(), button.getHeight(),
				Justification::centredTop, 10);		
		}
};


class MainContentComponent   : public Component
{
public:
    MainContentComponent()
    {
		// Set slider values and make visible
		slider.setSliderStyle(Slider::LinearVertical);
		slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
		slider.setTextValueSuffix("1");
		addAndMakeVisible(slider);
        
		slider2.setSliderStyle(Slider::LinearVertical);
		slider2.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
		slider.setTextValueSuffix("2");
		addAndMakeVisible(slider2);

		// Set custom LookAndFeel
		slider.setLookAndFeel(&otherLookAndFeel);
		slider2.setLookAndFeel(&otherLookAndFeel);

		slider.setAlwaysOnTop(true);
		slider2.setAlwaysOnTop(true);
		
		// Set Button Values and maje Visible
		Mute.setButtonText("MUTE");
		addAndMakeVisible(Mute);
		Mute.setLookAndFeel(&otherLookAndFeel);

		Mute2.setButtonText("MUTE");
		addAndMakeVisible(Mute2);
		Mute2.setLookAndFeel(&otherLookAndFeel);

		// Set window size
        setSize (250, 750);

    }
    
    ~MainContentComponent()
    {
    }

	void paint(Graphics& g) override
	{
		// x and y coordinates, line thickness and font size
		int lineX = fader.getWidth() / 2 - meter.getWidth() + fader.getWidth() + 1;
		int lineYTop = Channel.getY() * 2 + 10 + chanNum;
		int lineYBot = slider.getHeight() + chanNum;
		int fontSize = 30, lineOffset = 10;
		float lineThick = 2.0F;
		float borerThick = 1.0F;

		// All Background images including Background and channel strip are set here
		g.setColour(Colours::darkslategrey);
		g.fillRect(Channel);
		g.fillRect(Channel2);
		g.drawRect(Channel, borerThick);
		g.drawRect(Channel2, borerThick);
		g.setColour(Colours::dimgrey);
		g.fillRect(faderStrip);
		g.fillRect(faderStrip2);
		g.setColour(Colours::dimgrey);
		g.fillRect(muteBack);
		g.fillRect(muteBack2);
		g.setColour(Colours::black);
		g.drawLine(lineX, lineYTop, lineX, lineYBot, lineThick);
		g.drawLine(lineX + Channel.getWidth() + lineOffset, lineYTop, lineX + Channel.getWidth() + lineOffset, lineYBot, lineThick);
		g.drawRect(faderStrip, borerThick);
		g.drawRect(muteBack, borerThick);
		g.drawRect(Channel, borerThick);
		g.drawRect(faderStrip2, borerThick);
		g.drawRect(muteBack2, borerThick);
		g.drawRect(Channel2, borerThick);
		g.setFont(fontSize);
		g.drawText("1", Channel, Justification::centredTop);
		g.drawText("2", Channel2, Justification::centredTop);

    }
    
    void resized() override
    {
		// Load images, used for dimensions
		muteButton = ImageCache::getFromMemory(BinaryData::normal2x_png, BinaryData::normal2x_pngSize);
		fader = ImageCache::getFromMemory(BinaryData::cap_png, BinaryData::cap_pngSize);
		meter = ImageCache::getFromMemory(BinaryData::on_png, BinaryData::on_pngSize);
		
		// Calculate area in which to place Sliders and Buttons
		Rectangle<int> area;
		Rectangle<int> areaButton = muteButton.getBounds();
		int channelBaseHeight = meter.getHeight()+muteButton.getHeight();
		int channelBaseWidth = muteButton.getWidth()*0.8;
		Channel.setHeight(channelBaseHeight + border * 2 + chanNum);
		Channel.setWidth(channelBaseWidth + border * 2);

		Channel2.setHeight(channelBaseHeight + border * 2 + chanNum);
		Channel2.setWidth(channelBaseWidth + border * 2);

		Channel.setPosition(border, border);
		Channel2.setPosition(border*2 + Channel.getWidth(), border);

		// Set sizes inclduing borders and adjustments
		int channelX = Channel.getX() + border;
		int channelY = Channel.getY() + border + chanNum;
		int channelWid = Channel.getWidth() + border;
		int channelHei = Channel.getWidth() + border;
		int MuteWid = muteButton.getWidth()*0.8;
		int MuteHei = muteButton.getHeight()*0.8;
		int halfBorder = border / 2, borderExt = border*1.5;

		// Place Sliders and Buttons, added X adjustments for second channel strip
		area.setHeight(channelBaseHeight);
		area.setWidth(muteButton.getWidth()+border);
		slider.setBounds(channelX, channelY, MuteWid, meter.getHeight() + border);
		slider2.setBounds(channelX + channelWid, Channel.getY() + border + chanNum, MuteWid, meter.getHeight() + border);

		faderStrip.setBounds(slider.getX() - halfBorder, slider.getY() - halfBorder, slider.getWidth() + border, slider.getHeight() + border);
		muteBack.setBounds(faderStrip.getX(), channelY + faderStrip.getHeight(), faderStrip.getWidth(), Channel.getHeight() - chanNum - faderStrip.getHeight() - borderExt);
		
		faderStrip2.setBounds(slider.getX() - halfBorder + channelWid, slider.getY() - halfBorder, slider.getWidth() + border, slider.getHeight() + border);
		muteBack2.setBounds(faderStrip.getX() + channelWid, channelY + faderStrip.getHeight(), faderStrip.getWidth(), Channel.getHeight() - chanNum - faderStrip.getHeight() - borderExt);
        
		Mute.setBounds(slider.getX() - border, muteBack.getY(), MuteWid, MuteHei);
		Mute2.setBounds((slider.getX() - border) + Channel.getWidth() + border, muteBack.getY(), MuteWid, MuteHei);
		
    }

private:
	OtherLookAndFeel otherLookAndFeel;
	Slider slider, slider2;
	ToggleButton Mute, Mute2;
	LookAndFeel_V3 buttonLookAndFeel;
	Image meter, muteButton, fader;
	Rectangle<int> Channel, Channel2;
	Rectangle<int> faderStrip, faderStrip2;
	Rectangle<int> muteBack, muteBack2;
	const int chanNum = 30, border = 10;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

#endif  // MAINCOMPONENT_H_INCLUDED

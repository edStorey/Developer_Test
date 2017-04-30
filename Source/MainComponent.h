#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class OtherLookAndFeel : public LookAndFeel_V2
{
public:
	OtherLookAndFeel()
	{

	}

	void drawLinearSlider(Graphics& g, //Graphics& a,
			int x, int y, int w, int h,
			float sliderPos, float minSliderPos, float maxSliderPos,
			const Slider::SliderStyle style,
			Slider& slider)
		{
			g.fillAll(slider.findColour(Slider::backgroundColourId));

			if (style == Slider::LinearVertical)
			{

				Image myStrip;
				Image onMeter;
				Image offmeter;

				myStrip = ImageCache::getFromMemory(BinaryData::cap_png, BinaryData::cap_pngSize);
				onMeter = ImageCache::getFromMemory(BinaryData::on_png, BinaryData::on_pngSize);
				offmeter= ImageCache::getFromMemory(BinaryData::off_png, BinaryData::off_pngSize);

				Rectangle<int> area = onMeter.getBounds();
				//area.setHeight(onMeter.getHeight()/2);
				area.setHeight((int)(onMeter.getHeight()*(sliderPos / maxSliderPos)));
				double perc = sliderPos / maxSliderPos;
				int watchArea = (int)(onMeter.getHeight()*(sliderPos / maxSliderPos));
				area.setPosition(0, 0);
				
				const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
				const int nFrames = myStrip.getHeight() / myStrip.getWidth(); // number of frames for vertical film strip
				const int frameIdx = (int)ceil(fractRotation * ((double)nFrames - 1.0)); // current index from 0 --&gt; nFrames-1

				const float radius = jmin(w / 2.0f, h / 2.0f);
				const float centreX = x + w * 0.5f;
				const float centreY = y + h * 0.5f;
				const float rx = centreX - radius - 1.0f;
				const float ry = centreY - radius /* - 1.0f*/;
				
				int frIDxWid = frameIdx*myStrip.getWidth();
				int watch = (int)(myStrip.getHeight() - myStrip.getHeight()*(sliderPos / maxSliderPos));
				g.drawImage(myStrip, // image    
					//slider.getWidth()/2 - myStrip.getWidth()/3.3, sliderPos- myStrip.getHeight()/2, myStrip.getWidth(), myStrip.getHeight(),   // dest
					myStrip.getWidth()/2 - onMeter.getWidth(), sliderPos - myStrip.getHeight() / 2, myStrip.getWidth()*2, myStrip.getHeight(),
					0, 0, myStrip.getHeight(), myStrip.getHeight()); // source
				g.drawImage(offmeter, // image    
					slider.getWidth() - onMeter.getWidth(), slider.getHeight() - minSliderPos, onMeter.getWidth(), slider.getHeight()*0.95,   // dest
					0, 0, onMeter.getWidth(), onMeter.getHeight()); // source
				exclude(g, onMeter, sliderPos, maxSliderPos, minSliderPos, slider);
				g.drawFittedText(slider.getTextValueSuffix(),
					5, -20,
					slider.getWidth(), slider.getHeight(),
					Justification::centredTop, 10);
				

			}

		}
	void exclude(Graphics& a, Image onMeter, float sliderPos, float maxSliderPos, float minSliderPos, Slider& slider)
	{
		Rectangle<int> area = onMeter.getBounds();
		//area.setHeight((int)(onMeter.getHeight()*(sliderPos / maxSliderPos)));
		area.setHeight((int)(sliderPos));
		area.setWidth(slider.getWidth()+ 10);
		area.setPosition(slider.getWidth() - onMeter.getWidth()-5, 0);
		a.excludeClipRegion(area);
		a.drawImage(onMeter, // image    
			slider.getWidth() - onMeter.getWidth(), slider.getHeight() - minSliderPos, onMeter.getWidth(), slider.getHeight()*0.95,   // dest
			0, 0, onMeter.getWidth(), onMeter.getHeight()); // source
	}

	void drawTickBox(Graphics& g, Component& component,
		float x, float y, float w, float h,
		const bool ticked,
		const bool isEnabled,
		const bool isMouseOverButton,
		const bool isButtonDown)
	{
		const float boxSize = w * 0.7f;

		Image muteNormal;

		muteNormal = ImageCache::getFromMemory(BinaryData::normal2x_png, BinaryData::normal2x_pngSize);

		g.drawImage(muteNormal, // image    
			0, 0, muteNormal.getWidth(), muteNormal.getHeight(),   // dest
			0, 0, muteNormal.getHeight(), muteNormal.getHeight()); // source

		if (ticked)
		{

			Image muteSelected;

			muteSelected = ImageCache::getFromMemory(BinaryData::selected2x_png, BinaryData::selected2x_pngSize);

			g.drawImage(muteSelected, // image    
				0, 0, muteSelected.getWidth(), muteSelected.getHeight(),   // dest
				0, 0, muteSelected.getHeight(), muteSelected.getHeight()); // source
			Tick = true;
		}
		else{
		Tick = false;
		}
	}

		void drawToggleButton(Graphics& g, ToggleButton& button,
			bool isMouseOverButton, bool isButtonDown)
		{
			if (button.hasKeyboardFocus(true))
			{
				//g.setColour(button.findColour(TextEditor::focusedOutlineColourId));
				//g.drawRect(0, 0, button.getWidth(), button.getHeight());
			}

			float fontSize = jmin(15.0f, button.getHeight() * 0.75f);
			const float tickWidth = fontSize * 1.1f;
			const int textX = (int)tickWidth + 5;
			
			drawTickBox(g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
				tickWidth, tickWidth,
				button.getToggleState(),
				button.isEnabled(),
				isMouseOverButton,
				isButtonDown);
			g.setColour(Colours::black);
			g.setFont(15.0);
			g.drawFittedText(button.getButtonText(),
				5, 7,
				button.getWidth(), button.getHeight(),
				Justification::centredTop, 10);
			

		}
private:
	bool Tick = false;
	

};

class MainContentComponent   : public Component
{
public:
    MainContentComponent()
    {
		
		
		slider.setSliderStyle(Slider::LinearVertical);
		slider.setTextBoxStyle(Slider::TextBoxAbove, false, 0, 0);
		slider.setTextValueSuffix("1");
		addAndMakeVisible(slider);
        
		slider2.setSliderStyle(Slider::LinearVertical);
		slider2.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
		slider.setTextValueSuffix("2");
		addAndMakeVisible(slider2);

		slider.setLookAndFeel(&otherLookAndFeel);
		slider2.setLookAndFeel(&otherLookAndFeel);

		slider.setAlwaysOnTop(true);
		
		Mute.setButtonText("MUTE");
		addAndMakeVisible(Mute);
		Mute.setLookAndFeel(&otherLookAndFeel);

		Mute2.setButtonText("MUTE");
		addAndMakeVisible(Mute2);
		Mute2.setLookAndFeel(&otherLookAndFeel);

        setSize (250, 750);

		//muteButton = ImageCache::getFromMemory(BinaryData::normal2x_png, BinaryData::normal2x_pngSize);

    }
    
    ~MainContentComponent()
    {
    }

	void paint(Graphics& g) override
	{


	
		g.setColour(Colours::black);
		g.setColour(Colours::darkslategrey);
		g.fillRect(Channel);
		g.fillRect(Channel2);

		
		
		g.drawRect(Channel, 1.0F);
		g.drawRect(Channel2, 1.0F);
		g.setColour(Colours::dimgrey);
		g.fillRect(faderStrip);
		g.fillRect(faderStrip2);
		g.setColour(Colours::dimgrey);
		g.fillRect(muteBack);
		g.fillRect(muteBack2);
		g.setColour(Colours::black);
		g.drawLine(fader.getWidth() / 2 - meter.getWidth() + fader.getWidth() + 1, Channel.getY() * 2 + 10 + chanNum, fader.getWidth() / 2 - meter.getWidth() + fader.getWidth() + 1, slider.getHeight() + chanNum, 2.0F);// Vertical
		g.drawLine(fader.getWidth() / 2 - meter.getWidth() + fader.getWidth() + 1 + Channel.getWidth() + 10, Channel.getY() * 2 + 10 + chanNum, fader.getWidth() / 2 - meter.getWidth() + fader.getWidth() + 1 + Channel.getWidth() + 10, slider.getHeight(), 2.0F);// Vertical
		g.drawRect(faderStrip, 1.0F);
		g.drawRect(muteBack, 1.0F);
		g.drawRect(Channel, 1.0F);
		g.drawRect(faderStrip2, 1.0F);
		g.drawRect(muteBack2, 1.0F);
		g.drawRect(Channel2, 1.0F);
		g.setFont(30);
		g.drawText("1", Channel, Justification::centredTop);
		g.drawText("2", Channel2, Justification::centredTop);
		//}
    }
    
    void resized() override
    {
        //const int border = 10;
		
		muteButton = ImageCache::getFromMemory(BinaryData::normal2x_png, BinaryData::normal2x_pngSize);
		fader = ImageCache::getFromMemory(BinaryData::cap_png, BinaryData::cap_pngSize);
		meter = ImageCache::getFromMemory(BinaryData::on_png, BinaryData::on_pngSize);
		Rectangle<int> area;
		Rectangle<int> areaButton = muteButton.getBounds();
		//chanNum = 30;
		int channelHeight = meter.getHeight()+muteButton.getHeight();
		int channelWidth = muteButton.getWidth()*0.8;
		Channel.setHeight(channelHeight + border * 2 + chanNum);
		Channel.setWidth(channelWidth+border*2);

		Channel2.setHeight(channelHeight + border * 2 + chanNum);
		Channel2.setWidth(channelWidth + border * 2);

		Channel.setPosition(border, border);
		Channel2.setPosition(border*2 + Channel.getWidth(), border);


		area.setHeight(channelHeight);
		area.setWidth(muteButton.getWidth()+border);
			Rectangle<int> dialArea = area.removeFromTop(meter.getHeight());
			slider.setBounds(Channel.getX() + border, Channel.getY() + border + chanNum, muteButton.getWidth()*0.8, meter.getHeight() + border);
			slider2.setBounds((Channel.getX() + border) + Channel.getWidth() + border, Channel.getY() + border + chanNum, muteButton.getWidth()*0.8, meter.getHeight() + border);

			faderStrip.setBounds(slider.getX() - border / 2, slider.getY() - border / 2, slider.getWidth() + border, slider.getHeight() + border);
			muteBack.setBounds(faderStrip.getX(), Channel.getY() + faderStrip.getHeight() + chanNum + border, faderStrip.getWidth(), Channel.getHeight() - chanNum - faderStrip.getHeight() - border*1.5);
		

			faderStrip2.setBounds(slider.getX() - border / 2 + Channel.getWidth() + border, slider.getY() - border / 2, slider.getWidth() + border, slider.getHeight() + border);
			muteBack2.setBounds(faderStrip.getX() + Channel.getWidth() + border, Channel2.getY() + faderStrip.getHeight() + border + chanNum, faderStrip.getWidth(), Channel.getHeight() - chanNum - faderStrip.getHeight() - border*1.5);
        const int buttonHeight = 30;

		Mute.setBounds(slider.getX() - border, muteBack.getY(), muteButton.getWidth()*0.8, muteButton.getHeight()*0.8);
		Mute2.setBounds((slider.getX() - border) + Channel.getWidth() + border, muteBack.getY(), muteButton.getWidth()*0.8, muteButton.getHeight()*0.8);
		
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
	//LookAndFeel_V1_Custom otherLookAndFeelCustom;
	//Graphics &g;
	
	
	//Graphics& a;

	
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};




#endif  // MAINCOMPONENT_H_INCLUDED

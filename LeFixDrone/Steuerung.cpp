#include "Steuerung.h"

float Steuerung::calculateDegSecPitch(const float& inputP)
{
	return calculateDegSec(inputP, Settings::contRcRatePR.get(), Settings::contRateP.get(), Settings::contExpoPR.get());
}

float Steuerung::calculateDegSecRoll(const float& inputR)
{
	return calculateDegSec(inputR, Settings::contRcRatePR.get(), Settings::contRateR.get(), Settings::contExpoPR.get());
}

float Steuerung::calculateDegSecYaw(const float& inputY)
{
	return calculateDegSec(inputY, Settings::contRcRateY.get(), Settings::contRateY.get(), Settings::contExpoY.get());
}

float Steuerung::calculateDegSec(const float& input, const float& rcRate, const float& gRate, const float& expo) {
	//http://ultraesc.de/KISSFC/rates.html
	//And betaflight GUI (when set PR together or not)

	float RPY_useRates = 1.0f - abs(input)*gRate; // ?
	float input2 = ((input*input*input)*expo + input*(1 - expo)); //First order and thrid order

	return 200.0f / RPY_useRates * input2 * rcRate; //why 200? standard?
}
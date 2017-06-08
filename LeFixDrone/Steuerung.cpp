#include "Steuerung.h"

float Steuerung::calculateDegSecPitch(const float& inputP)
{
	return calculateDegSec(inputP, Settings::contRcRatePR, Settings::contRateP, Settings::contExpoPR);
}

float Steuerung::calculateDegSecRoll(const float& inputR)
{
	return calculateDegSec(inputR, Settings::contRcRatePR, Settings::contRateR, Settings::contExpoPR);
}

float Steuerung::calculateDegSecYaw(const float& inputY)
{
	return calculateDegSec(inputY, Settings::contRcRateY, Settings::contRateY, Settings::contExpoY);
}

float Steuerung::calculateDegSec(const float& input, const float& rcRate, const float& gRate, const float& expo) {
	//http://ultraesc.de/KISSFC/rates.html
	//And betaflight GUI (when set PR together or not)

	float RPY_useRates = 1.0f - abs(input)*gRate; // ?
	float input2 = ((input*input*input)*expo + input*(1 - expo)); //First order and thrid order

	return 200.0f / RPY_useRates * input2 * rcRate; //why 200? standard?
}